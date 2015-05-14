#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION

#include <DumbFramework/font.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DFE_POSITION_INDEX        0
#define DFE_SIZE_INDEX            1
#define DFE_TOP_TEX_INDEX         2
#define DFE_DOWN_TEX_INDEX        3
#define DFE_COLOR_INDEX           4
#define DFE_BUFFER_ELEMENT_COUNT 11
#define DFE_BUFFER_STRIDE        (DFE_BUFFER_ELEMENT_COUNT * sizeof(GLfloat))
#define DFE_DEFAULT_COLOR        glm::fvec3(1.0f, 1.0f, 1.0f)


namespace Dumb {
    namespace Font {
        // Program's index.
        // Shaders
        const char *s_dfe_fragmentShader = R"EOT(
#version 410 core

layout (binding=0) uniform sampler2D un_texture;

in vec2 fs_tex;
in vec4 fs_color;

out vec4 out_Color;

void main()
{
    out_Color = texture(un_texture, vec2(fs_tex)).r * fs_color;
}
)EOT";

        const char *s_dfe_vertexShaderInstanced = R"EOT(
#version 410 core

uniform mat4 un_matrix;

layout (location=0) in vec2 vs_position;
layout (location=1) in vec2 vs_dimension;
layout (location=2) in vec2 vs_toptex;
layout (location=3) in vec2 vs_bottomtex;
layout (location=4) in vec3 vs_color;

flat out vec4 fs_color;
flat out vec2 fs_tex;

const vec2 quad[4] = { vec2(0, 0),
                       vec2(0, 1),
                       vec2(1, 0),
                       vec2(1, 1) };

void main()
{
    vec2 point = quad[gl_VertexID];
    vec2 dimPt = vs_dimension * point;

    fs_tex = mix(vs_toptex, vs_bottomtex, point);
    fs_color = vec4(vs_color, 1);

    gl_Position = un_matrix * vec4(vs_position + dimPt, 0.0, 1.0);
}
)EOT";


        //   ------------------
        void Engine::print(const Wrapper *font, glm::vec2 pos, icu::UnicodeString text) {
            using namespace Framework;
            /* TODO For the moment, it's simple. But in further devs, this method will be
               a facade for a more extensive method :
               Area createArea(Parameters params);
               Where 'Parameters' is one hell of a class containing tons of informations about
               the area to create/maintain, etc. */
            Render::Renderer& renderer = Render::Renderer::instance();

            // Compoute the content of _cell and the number of glyph to print.
            // Retrieve buffer and memcpy.
            renderer.depthBufferWrite(false);

            _buffer.bind();
            GLfloat *ptr = reinterpret_cast<GLfloat *>(_buffer.map(Render::BufferObject::Access::Policy::WRITE_ONLY));
            GLsizei count = 0; // Number of glyph to display.
            // Iterate on the text.
            UChar32 start = static_cast<UChar32>(font->getStartingCodePoint());
            UChar32 last = start + static_cast<UChar32>(font->getGlyphsCount());
            stbtt_aligned_quad quad;
            float xpos = static_cast<float>(pos.x);
            float ypos = static_cast<float>(pos.y);
            stbtt_packedchar *data = font->_data; // How handy ...
            icu::StringCharacterIterator it(text);
            glm::fvec3 color = DFE_DEFAULT_COLOR;
            for(it.setToStart(); it.hasNext();) {
                UChar32 codepoint = it.next32PostInc();
                // Silently ignore out of range characters.
                if((codepoint >= start) && (codepoint <= last)) {
                    ++count;
                    stbtt_GetPackedQuad(data, _size, _size, codepoint - start, &xpos, &ypos, &quad, 0);
                    ptr[ 0] = quad.x0;
                    ptr[ 1] = quad.y0;
                    ptr[ 2] = quad.x1 - quad.x0;
                    ptr[ 3] = quad.y1 - quad.y0;
                    ptr[ 4] = quad.s0;
                    ptr[ 5] = quad.t0;
                    ptr[ 6] = quad.s1;
                    ptr[ 7] = quad.t1;
                    ptr[ 8] = color.r;
                    ptr[ 9] = color.g;
                    ptr[10] = color.b;
                    
                    Log_Info(Framework::Module::App, "#%04x", codepoint);
                    Log_Info(Framework::Module::App, "x0 %f, y0 %f", quad.x0, quad.y0);
                    Log_Info(Framework::Module::App, "x1 %f, y1 %f", quad.x1, quad.y1);
                    Log_Info(Framework::Module::App, "s0 %f, t0 %f", quad.s0, quad.t0);
                    Log_Info(Framework::Module::App, "s1 %f, t1 %f", quad.s1, quad.t1);
                    Log_Info(Framework::Module::App, "r  %f, g  %f, b  %f", color.r, color.g, color.b);
                
                    ptr += DFE_BUFFER_ELEMENT_COUNT;
                }
                Log_Info(Framework::Module::App, "----------------");
                
            }
            _buffer.unmap();
            _buffer.unbind();

            _program.begin();
            _program.uniform(_uniformMatrix, false, _matrix);

            renderer.setActiveTextureUnit(0);
            glBindTexture(GL_TEXTURE_2D, _atlas);

            // Send VAO.
            _stream.bind();
            glDrawArraysInstanced (GL_TRIANGLE_STRIP, 0, 4, count);
            _stream.unbind();
            _program.end();
            glBindTexture(GL_TEXTURE_2D, 0);
            renderer.depthBufferWrite(true);
        }

        // ------------
        Engine::~Engine() {
            std::map<std::string, Wrapper *>::iterator it;
            for(it = _wrappers.begin(); it != _wrappers.end(); ++it) {
                Log_Info(Framework::Module::App, "Flushing '%s'", it->first.c_str());
                delete it->second;
            }
        }

        //   -------------------
        void Engine::setViewport(GLfloat width, GLfloat height) {
            _matrix = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
        }

        //   ----------------------
        void Engine::packOversample(stbtt_pack_context &context, const Oversample &oversample, char *font) {
            std::vector<Range> specs = oversample.getRanges();
            std::vector<Range>::size_type count = specs.size();
            stbtt_pack_range *packRange = new stbtt_pack_range[count];

            for(std::vector<Range>::size_type i = 0; i != count; ++i) {
                Range spec = specs[i];
                stbtt_pack_range *range = packRange + i;
                range->font_size = spec.getSize();
                range->first_unicode_char_in_range = spec.getStartingCodePoint();
                range->num_chars_in_range = spec.getGlyphsCount();
                range->chardata_for_range = new stbtt_packedchar[range->num_chars_in_range];
                Wrapper *wrapper = new Wrapper(spec, range->chardata_for_range);
                const std::string &name = spec.getIdentifier();
                Log_Info(Framework::Module::App, "Register '%s'", name.c_str());
                _wrappers.insert(std::pair<std::string, Wrapper *>(name, wrapper));
            }
            glm::vec2 ovr = oversample.getOversample();
            stbtt_PackSetOversampling(&context, (unsigned int) ovr.x, (unsigned int) ovr.y);
            if(stbtt_PackFontRanges(&context, reinterpret_cast<unsigned char*>(font),
                        0, packRange, count) == 0) {
                Log_Error(Framework::Module::App, "Font range loading failure");
            }
            delete []packRange;
        }

        //   ----------------
        void Engine::packFont(stbtt_pack_context &context, const Resource &resource) {
            std::ifstream fontFile;
            fontFile.open(resource.getPath().c_str(), std::ios::binary|std::ios::ate|std::ios::in);
            // Font file check.
            if(fontFile.is_open()) {
                Log_Info(Framework::Module::App, "Loading '%s'", resource.getPath().c_str());
                // Font file is ok. Let's load it.
                std::streampos size = fontFile.tellg();
                char *fontFileContent = new char[size];
                fontFile.seekg(0, std::ios::beg);
                fontFile.read(fontFileContent, size);
                fontFile.close();
                for(auto &i : resource.getSpecs()) {
                    packOversample(context, i, fontFileContent);
                }
                // We're done here.
                delete []fontFileContent;
            } else {
                Log_Error(Framework::Module::App, "Failed to open '%s'", resource.getPath().c_str());
            }
        }

        //------------
        Engine::Engine(const std::vector<Resource> &fonts,
                unsigned int capacity,
                unsigned int size) : _capacity(capacity), _size(size) {
            using namespace Framework;
            // Let the fun begins !
            // First, build a temporary buffer for the texture.
            unsigned char *buffer = new unsigned char[size * size];
            // Then, create a context for STB TrueType.
            stbtt_pack_context context;
            // Make the atlas.
            stbtt_PackBegin(&context, buffer, size, size, 0, 1, 0);
            for(auto &i : fonts) {
                packFont(context, i);
            }
            stbtt_PackEnd(&context);
            // Create the GL Texture.
            glGenTextures(1, &_atlas);
            glBindTexture(GL_TEXTURE_2D, _atlas);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,
                    size, size, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            delete []buffer;

            // We're not over. Let's initialize VBO/VAO/Shaders/Program.
            _buffer.create(DFE_BUFFER_STRIDE*capacity);
            _stream.create();
            _stream.add(&_buffer,
                    {
                    { DFE_POSITION_INDEX, Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT, 2, false, DFE_BUFFER_STRIDE,                    0, 1) },
                    { DFE_SIZE_INDEX,     Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT, 2, false, DFE_BUFFER_STRIDE, sizeof(GLfloat) *  2, 1) },
                    { DFE_TOP_TEX_INDEX,  Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT, 2, false, DFE_BUFFER_STRIDE, sizeof(GLfloat) *  4, 1) },
                    { DFE_DOWN_TEX_INDEX, Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT, 2, false, DFE_BUFFER_STRIDE, sizeof(GLfloat) *  6, 1) },
                    { DFE_COLOR_INDEX,    Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT, 3, false, DFE_BUFFER_STRIDE, sizeof(GLfloat) *  8, 1) },
                    });
            _stream.compile();

            // Create program.
            std::array<Render::Shader, 2> shaders;
            shaders[0].create(Render::Shader::VERTEX_SHADER,   s_dfe_vertexShaderInstanced);
            shaders[1].create(Render::Shader::FRAGMENT_SHADER, s_dfe_fragmentShader);

            _program.create();
            for(unsigned long i=0; i<shaders.size(); i++)
            {
                shaders[i].infoLog(Severity::Info);
                _program.attach(shaders[i]);
            }

            _program.link();
            _program.infoLog(Severity::Info);
            _uniformMatrix  = _program.getUniformLocation("un_matrix");
            _uniformTexture = _program.getUniformLocation("un_texture");
        }

    } // 'Font' namespace.
} // 'Dumb' namespace.
