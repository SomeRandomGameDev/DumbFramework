#include <runner.hpp>

#include <iostream>
#include <fstream>

/*
   g++ adviser.o font.cpp libDumbFramework.a -I../src -I../../Libraries/stb -I../../DumbFramework/include -licuuc -lGLEW -lglfw -lGL -lGLU -lpthread -lm -DGLM_FORCE_RADIANS -std=c++11 -o fonttest 
   g++ adviser.o font.cpp -I../src -I../../Libraries/stb -lglfw -lGL -lGLU -lm -o fonttest -std=c++11 -DGLM_FORCE_RADIANS
 */

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>
#include <stb_truetype.h>

#include <string>
#include <vector>
#include <map>
#include <array>

/* Of course, we'll use ICU for text management. The std c++ string library is far from perfect.
   And the Dumb Framework won't use the Boost lib ! - Over my dead body ! - */
#include <unicode/unistr.h>
#include <unicode/schriter.h>

#include <DumbFramework/render/vertexbuffer.hpp>
#include <DumbFramework/render/vertexstream.hpp>
#include <DumbFramework/render/program.hpp>
#include <DumbFramework/render/renderer.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * Dumb Font Engine default starting codepoint.
 */
#define DFE_START_CODEPOINT_DEFAULT 32U
/**
 * Dumb Font Engine default number of glyphs to load.
 */
#define DFE_GLYPHS_COUNT_DEFAULT 95U
/**
 * Dumb Font Engine default oversampling on X-Axis.
 */
#define DFE_OVERSAMPLE_X_DEFAULT 1
/**
 * Dumb Font Engine default oversampling on Y-Axis.
 */
#define DFE_OVERSAMPLE_Y_DEFAULT 1
/**
 * Dumb Font Engine default font size (in pixels).
 */
#define DFE_SIZE_DEFAULT 32.0
/**
 * Dumb Font Engine default texture atlas size.
 */
#define DFE_ATLAS_SIZE_DEFAULT 1024
/**
 * Dumb Font Engine default glyphs capacity.
 */
#define DFE_GLYPH_CAPACITY_DEFAULT 4096

namespace Dumb {
    namespace Font {
        /**
         * Font range. Defines size and glyphs to load.
         */
        class Range {
            public:
                /**
                 * Default constructor.
                 */
                Range() : _start(DFE_START_CODEPOINT_DEFAULT),
                _count(DFE_GLYPHS_COUNT_DEFAULT),
                _size(DFE_SIZE_DEFAULT) {}
                /**
                 * Full constructor.
                 * @param [in] id Font Range Identifier.
                 * @param [in] st Starting code point.
                 * @param [in] ct Glyphs count.
                 * @param [in] sz Size (in pixels).
                 */
                Range(const char *id, unsigned int st, unsigned int ct, double sz) :
                    _identifier(id), _start(st), _count(ct), _size(sz) {}

                /**
                 * @return Starting code point.
                 */
                inline unsigned int getStartingCodePoint() const {
                    return _start;
                }

                /**
                 * @return Number of glyphs.
                 */
                inline unsigned int getGlyphsCount() const {
                    return _count;
                }

                /**
                 * @return Size (in pixels).
                 */
                inline double getSize() const {
                    return _size;
                }

                /**
                 * @return Human readable identifier.
                 */
                inline const std::string &getIdentifier() const {
                    return _identifier;
                }
            private:
                /**
                 * Human readable identifier.
                 */
                std::string _identifier;
                /**
                 * Starting code point.
                 */
                unsigned int _start;
                /**
                 * Number of glyphs to load.
                 */
                unsigned int _count;
                /**
                 * Size (in pixels).
                 */
                double _size;
        };

        /**
         * Oversampled ranges of glyphs definition.
         */
        class Oversample {
            public:
                /**
                 * Default constructor.
                 */
                Oversample() : _oversample(DFE_OVERSAMPLE_X_DEFAULT, DFE_OVERSAMPLE_Y_DEFAULT) {
                    // Nothing to do.
                }
                /**
                 * Complete constructor.
                 * @param [in] ovr Oversampling (in both axis).
                 * @param [in] rgs Font definition ranges.
                 */
                Oversample(glm::vec2 ovr, std::vector<Range> rgs) : _oversample(ovr), _ranges(rgs) {
                    // Nothing special to do.
                }
                /**
                 * @return Oversampling hint.
                 */
                inline glm::vec2 getOversample() const {
                    return _oversample;
                }
                /**
                 * @return Ranges.
                 */
                inline std::vector<Range> getRanges() const {
                    return _ranges;
                }
            private:
                /**
                 * Oversampling hint.
                 */
                glm::vec2 _oversample;
                /**
                 * Ranges.
                 */
                std::vector<Range> _ranges;
        };

        /**
         * Resource class describe where and how to load a font.
         */
        class Resource {
            public:
                /**
                 * Default constructor.
                 */
                Resource() { /* Nothing to do ... Empty path, no specs ... */ }

                /**
                 * Full constructor.
                 * @param [in] pt Path to font.
                 * @param [in] spc List of font specs.
                 */
                Resource(std::string pt, std::vector<Oversample> spc) : _path(pt), _specs(spc) {
                    /* Nothing more to do. */
                }

                /**
                 * @return Font path.
                 */
                inline const std::string& getPath() const {
                    return _path;
                }

                /**
                 * @return Font specs.
                 */
                inline std::vector<Oversample> getSpecs() const {
                    return _specs; // Ugly copy, but safe. The cardinality justifies it.
                }

            private:
                /**
                 * Font path.
                 */
                std::string _path;
                /**
                 * Font specs.
                 */
                std::vector<Oversample> _specs;
        };

        // Engine forward declaration.
        class Engine;

        /**
         * Font wrapper. Addressable font descriptor for printing usage.
         */
        class Wrapper : public Range {
            friend class Engine;

            public:
            /**
             * Default constructor.
             */
            Wrapper() : _data(0) { /* Nope */ }
            private:
            /**
             * Private constructor (used by the engine only).
             */
            Wrapper(Range originator, stbtt_packedchar *dt) :
                Range(originator), _data(dt) {
                    /* Nothing special to be done. */
                }
            /**
             * Private destructor.
             */
            ~Wrapper() {
                if(0 != _data) {
                    delete []_data;
                }
            }
            /**
             * @return Packed char data information.
             */
            inline stbtt_packedchar *getData() { return _data; }
            private:
            /**
             * Information about characters in the pack.
             */
            stbtt_packedchar *_data;
        };

        /**
         * El Dumb Font Engine.
         */
        class Engine {
            public:
                /**
                 * Constructor.
                 * @param [in] fonts List of fonts to load.
                 * @param [in] capacity Number of displayable glyphs.
                 * @param [in] size Size of the font atlas. The atlas is a square texture.
                 */
                Engine(const std::vector<Resource> &fonts,
                        unsigned int capacity,
                        unsigned int size);

                /**
                 * Gentle destructor.
                 */
                ~Engine();

                /**
                 * @return The font atlas identifier.
                 */
                inline GLuint getAtlas() {
                    return _atlas;
                }

                /**
                 * Retrieve a font wrapper.
                 * @param id Identifier previously specified at engine initialisation.
                 */
                const Wrapper *getFont(const std::string &id) const {
                    std::map<std::string, Wrapper *>::const_iterator it = _wrappers.find(id);
                    Wrapper *result = (it != _wrappers.end())?it->second:0;
                    return result;
                }

                /**
                 * Simple text printing.
                 * @param [in] font Font to be used.
                 * @param [in] pos Position in logical coordinate system.
                 * @param [in] text Starting text.
                 */
                void print(const Wrapper *font, glm::vec2 pos, icu::UnicodeString text);

                /**
                 * Set the viewport. This method is pretty straightforward as we only
                 * specify the viewport dimension in pixels. The coordinate system is meant
                 * to have (0, 0) in top-left corner and (width, height) in bottom-right
                 * corner.
                 * @param [in] width Width (not only pixels).
                 * @param [in] height Height (not only pixels).
                 */
                void setViewport(GLfloat width, GLfloat height);

                /* TODO Serious business here :
                   - Logical viewport management.
                   - VAO management.
                   - Text Areas storage and maintenance.
                   - Rendering process. */
            private:
                /**
                 * Pack a font and all its specs.
                 * @param [in] context STB TrueType context.
                 * @param [in] resource Resource.
                 */
                void packFont(stbtt_pack_context &context, const Resource &resource);
                /**
                 * Pack an oversampled set of font ranges.
                 * @param [in] context STB TrueType context.
                 * @param [in] oversample Oversampled set.
                 * @param [in] font Font file data.
                 */
                void packOversample(stbtt_pack_context &context, const Oversample &oversample, char *font);
            private:
                /**
                 * Font atlas texture identifier.
                 */
                GLuint _atlas;

                /**
                 * Font Wrappers.
                 */
                std::map<std::string, Wrapper*> _wrappers;

                /**
                 * Vertex Stream.
                 */
                Framework::Render::VertexStream _stream;

                /**
                 * Vertex Buffer Object.
                 */
                Framework::Render::VertexBuffer _buffer;

                /**
                 * GLSL Program identifier.
                 */
                Framework::Render::Program _program;

                /**
                 * Texture uniform binding.
                 */
                GLuint _uniformTexture;

                /**
                 * Projection Matrix uniform binding.
                 */
                GLuint _uniformMatrix;

                /**
                 * Projection Matrix.
                 */
                glm::mat4 _matrix;

                /**
                 * Glyph capacity.
                 */
                unsigned int _capacity;

                /**
                 * Atlas size (in texels).
                 */
                unsigned int _size;
        };
    } // 'Font' namespace.
} // 'Dumb' namespace.


//######## IMPLEMENTATION ################

namespace Dumb {
    namespace Font {
        // Program's index.
#define DFE_POSITION_INDEX        0
#define DFE_SIZE_INDEX            1
#define DFE_TOP_TEX_INDEX         2
#define DFE_DOWN_TEX_INDEX        3
#define DFE_COLOR_INDEX           4
#define DFE_BUFFER_ELEMENT_COUNT 11
#define DFE_BUFFER_STRIDE        (DFE_BUFFER_ELEMENT_COUNT * sizeof(GLfloat))
#define DFE_DEFAULT_COLOR        glm::fvec3(1.0f, 1.0f, 1.0f)

        // Shaders
        const char *s_dfe_fragmentShader =
            "#version 410 core\n"
            "layout (binding=0) uniform sampler2D un_texture;"
            "in vec2 fs_tex;"
            "in vec4 fs_color;"
            "out vec4 out_Color;"
            "void main() {"
            "out_Color = texture(un_texture, vec2(fs_tex)) * fs_color;"
            " }";

        const char *s_dfe_vertexShaderInstanced =
            "#version 410 core\n"
            "uniform mat4 un_matrix;"
            "layout (location=0) in vec2 vs_position;"
            "layout (location=1) in vec2 vs_dimension;"
            "layout (location=2) in vec2 vs_toptex;"
            "layout (location=3) in vec2 vs_bottomtex;"
            "layout (location=4) in vec3 vs_color;"
            "flat out vec4 fs_color;"
            "out vec2 fs_tex;"
            "const vec2 quad[4] = { vec2(0, 0),"
            "                       vec2(0, 1),"
            "                       vec2(1, 1),"
            "                       vec2(1, 0) };"
            "void main() {"
            "vec2 point = quad[gl_VertexID];"
            "vec2 dimPt = vs_dimension * point;"
            "fs_tex = mix(vs_toptex, vs_bottomtex, point);"
            "fs_color = vec4(vs_color, 1);"
            "gl_Position = un_matrix * vec4(vs_position + dimPt, 0.0, 1.0);"
            "}";


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

            glBindTexture(0, _atlas); // FIXME not 'glBindTexture(GL_TEXTURE_2D, _atlas)' ?
            _program.begin();
            _program.uniform(_uniformMatrix, false, _matrix);
            _program.uniform(_uniformTexture, 0);

            renderer.setActiveTextureUnit(0);
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
                std::cout << "0x" << std::hex << codepoint << std::dec << " ";
                // Silently ignore out of range characters.
                if((codepoint >= start) && (codepoint <= last)) {
                    ++count;
                    stbtt_GetPackedQuad(data, _size, _size, codepoint - start, &xpos, &ypos, &quad, 0);
                    ptr[0] = quad.x0;
                    ptr[1] = quad.y0;
                    ptr[2] = quad.x1 - quad.x0;
                    ptr[3] = quad.y1 - quad.y0;
                    ptr[4] = quad.s0;
                    ptr[5] = quad.t0;
                    ptr[6] = quad.s1;
                    ptr[7] = quad.t1;
                    ptr[8] = color.r;
                    ptr[9] = color.g;
                    ptr[10] = color.b;
                    ptr += DFE_BUFFER_ELEMENT_COUNT;
                }
            }
            _buffer.unmap();
            _buffer.unbind();
            std::cout << std::endl;
            // Send VAO.
            _stream.bind();
            std::cout << "Send " << count << " instance to the pipe." << std::endl;
            glDrawArraysInstanced (GL_TRIANGLE_STRIP, 0, 4, count);
            _stream.unbind();
            _program.end();
            glBindTexture(0, 0);
            renderer.depthBufferWrite(true);
        }

        // ------------
        Engine::~Engine() {
            std::map<std::string, Wrapper *>::iterator it;
            for(it = _wrappers.begin(); it != _wrappers.end(); ++it) {
                std::cout << "Flushing '" << it->first << "'" << std::endl; // TODO Logging.
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
                std::cout << "Size : " << range->font_size << "px , Start : " << range->first_unicode_char_in_range
                    << ", Count : " << range->num_chars_in_range << std::endl;
                range->chardata_for_range = new stbtt_packedchar[range->num_chars_in_range];
                Wrapper *wrapper = new Wrapper(spec, range->chardata_for_range);
                const std::string &name = spec.getIdentifier();
                std::cout << "Register '" << name << "'" << std::endl; // TODO Logging.
                _wrappers.insert(std::pair<std::string, Wrapper *>(name, wrapper));
            }
            glm::vec2 ovr = oversample.getOversample();
            stbtt_PackSetOversampling(&context, (unsigned int) ovr.x, (unsigned int) ovr.y);
            if(stbtt_PackFontRanges(&context, reinterpret_cast<unsigned char*>(font),
                        0, packRange, count) == 0) {
                std::cerr << "Font range loading failure" << std::endl; // TODO Logging
            }
            delete []packRange;
        }

        //   ----------------
        void Engine::packFont(stbtt_pack_context &context, const Resource &resource) {
            std::ifstream fontFile;
            fontFile.open(resource.getPath().c_str(), std::ios::binary|std::ios::ate|std::ios::in);
            // Font file check.
            if(fontFile.is_open()) {
                std::cout << "Loading '" << resource.getPath() << "'" << std::endl; // TODO Logging
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
                std::cerr << "Failed to open '" << resource.getPath() << "'" << std::endl; // TODO Logging
            }
        }

        //------------
        Engine::Engine(const std::vector<Resource> &fonts,
                unsigned int capacity = DFE_GLYPH_CAPACITY_DEFAULT,
                unsigned int size = DFE_ATLAS_SIZE_DEFAULT) : _capacity(capacity), _size(size) {
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
                    size, size, 0, GL_ALPHA, GL_UNSIGNED_BYTE, buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            delete []buffer;

            // We're not over. Let's initialize VBO/VAO/Shaders/Program.
            _buffer.create(DFE_BUFFER_STRIDE*capacity);
            _stream.create();
            _stream.add(&_buffer,
                    {
                    { DFE_POSITION_INDEX, Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT, 2, DFE_BUFFER_STRIDE, 0,                  1) },
                    { DFE_SIZE_INDEX,     Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT, 2, DFE_BUFFER_STRIDE, sizeof(GLfloat) *  2, 1) },
                    { DFE_TOP_TEX_INDEX,  Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT, 2, DFE_BUFFER_STRIDE, sizeof(GLfloat) *  4, 1) },
                    { DFE_DOWN_TEX_INDEX, Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT, 2, DFE_BUFFER_STRIDE, sizeof(GLfloat) *  6, 1) },
                    { DFE_COLOR_INDEX,    Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT, 3, DFE_BUFFER_STRIDE, sizeof(GLfloat) *  8, 1) },
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


/**
 * Sample application : Minimal example.
 */
class Example {
    DECLARE_WRAPPER_METHODS
    public:
        /**
         * Default Constructor.
         */
        Example() : _closeFlag(1), _engine(0) {
            // Nothing special to do.
        }

        /**
         * The mandatory destructor.
         */
        ~Example() {
            if(0 != _engine) {
                delete _engine;
            }
        }
    private:
        /**
         * If 0, close the app.
         */
        int _closeFlag;

        /**
         * Font engine.
         */
        Dumb::Font::Engine *_engine;

        /**
         * Screen size (in pixels).
         */
        glm::vec2 _screenSize;
};

void Example::init(Dumb::Core::Application::Adviser *adviser) {
    Dumb::Core::Application::Video::Monitor monitor = adviser->getPrimaryMonitor();
    adviser->setMonitor(monitor);
    Dumb::Core::Application::Video::Mode current = monitor.getCurrentMode();
    adviser->setVideoMode(current);
    _screenSize = current.getResolution();
    adviser->setTitle("Font Test");
}

void Example::postInit() {
    using namespace Dumb::Font;
    // Now, load the font and build the font atlas.
    std::vector<Range> range;
    std::vector<Oversample> oversample;
    std::vector<Resource> resource;
    // Load Vera.
    range.push_back(Range("Vera-12", 32, 95, 12.0));
    range.push_back(Range("Vera-24", 32, 95, 24.0));
    oversample.push_back(Oversample(glm::vec2(1, 1), range));
    range.clear();
    range.push_back(Range("Vera-12-ovr", 32, 95, 12.0));
    range.push_back(Range("Vera-24-ovr", 32, 95, 24.0)); 
    oversample.push_back(Oversample(glm::vec2(2, 3), range));
    resource.push_back(Resource("Vera.ttf", oversample));
    // Load FreeSans.
    range.clear();
    oversample.clear();
    range.push_back(Range("SansSmall", 32, 95, 12.0));
    range.push_back(Range("SansBig", 32, 95, 24.0));
    oversample.push_back(Oversample(glm::vec2(1, 1), range));
    resource.push_back(Resource("FreeSans.ttf", oversample));

    // Guess what ? There's no rendering for japanese fonts yet in stb_truetype !!
    range.clear();
    oversample.clear();
    range.push_back(Range("Japan1", 0x5bc2, 6, 32.0));
    range.push_back(Range("Japan2", 0x5be4, 6, 32.0));
    range.push_back(Range("Japan3", 0x5c38, 10, 32.0));
    range.push_back(Range("Japan4", 0x7dab, 11, 64.0));
    oversample.push_back(Oversample(glm::vec2(1, 1), range));
    resource.push_back(Resource("DroidSansJapanese.ttf", oversample));

    // Another japanese font.
    range.clear();
    oversample.clear();
    range.push_back(Range("BigJapan", 0x7cfb, 128, 64));
    range.push_back(Range("LotsOfJapan", 0x5bc2, 64, 32.0));
    oversample.push_back(Oversample(glm::vec2(1, 1), range));
    resource.push_back(Resource("TakaoPMincho.ttf", oversample));

    _engine = new Engine(resource, 1200);
}

int Example::render() {
    glViewport(0, 0, _screenSize.x, _screenSize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    Framework::Render::Renderer& renderer = Framework::Render::Renderer::instance();
    renderer.depthTest(true);

    renderer.texture2D(true);
    renderer.blend(true);
    renderer.blendFunc(Framework::Render::BlendFunc::SRC_ALPHA, Framework::Render::BlendFunc::ONE_MINUS_SRC_ALPHA);
    _engine->setViewport(_screenSize.x, _screenSize.y);
    icu::UnicodeString toPrint("This is a test");
    const Dumb::Font::Wrapper *font = _engine->getFont("Vera-24-ovr");
    _engine->print(font, glm::vec2(50, 50), toPrint);
    return _closeFlag;
}

void Example::handleUnicodeCharacter(unsigned int) {}
void Example::handleUnicodeModifierCharacter(unsigned int,int) {}
void Example::handleKey(int,int,int,int) {
    _closeFlag = 0;
}
void Example::handleMousePosition(double xp,double yp) {}
void Example::handleMouseButton(int,int,int) {}
void Example::handleMouseScroll(double,double) {}
void Example::handleWindowClose() {}
void Example::handleWindowSize(int, int) {}
void Example::handleWindowIconify(int) {}
void Example::close() {}



SIMPLE_APP(Example)
