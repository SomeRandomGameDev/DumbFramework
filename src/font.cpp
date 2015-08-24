/*
 * Copyright 2015 Stoned Xander
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION

#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>

#include <DumbFramework/font.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DFE_POSITION_INDEX        0 // (GLfloat, GLfloat)
#define DFE_SIZE_INDEX            1 // (GLfloat, GLfloat)
#define DFE_TOP_TEX_INDEX         2 // (GLfloat, GLfloat)
#define DFE_DOWN_TEX_INDEX        3 // (GLfloat, GLfloat)
#define DFE_COLOR_INDEX           4 // (GLubyte, GLubyte, GLubyte, GLubyte)

namespace Dumb {
    namespace Font {
        // Program's index.
        // Shaders
        const char *s_dfe_fragmentShader = R"EOT(
#version 410 core

        layout (binding=0) uniform sampler2D un_texture;

        in vec2 fs_tex;
        in vec4 fs_color;

        layout (location=0) out vec4 color;

        void main()
        {
            color = texture(un_texture, vec2(fs_tex)).r * fs_color;
        }
        )EOT";

        const char *s_dfe_vertexShaderInstanced = R"EOT(
#version 410 core

        uniform mat4 un_matrix;

        layout (location=0) in vec2 vs_position;
        layout (location=1) in vec2 vs_dimension;
        layout (location=2) in vec2 vs_toptex;
        layout (location=3) in vec2 vs_bottomtex;
        layout (location=4) in vec4 vs_color;

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
            fs_color = vs_color / 255.0;

            gl_Position = un_matrix * vec4(vs_position + dimPt, 0.0, 1.0);
        }
        )EOT";

        std::vector<std::pair<unsigned int, Dumb::Render::Geometry::Attribute>> s_attributes =
            std::vector<std::pair<unsigned int, Dumb::Render::Geometry::Attribute>>(
                    {
                    { DFE_POSITION_INDEX,
                    Dumb::Render::Geometry::Attribute(
                            Dumb::Render::Geometry::ComponentType::FLOAT, 2, false,
                            DFE_BUFFER_STRIDE,                    0, 1) },
                    { DFE_SIZE_INDEX,
                    Dumb::Render::Geometry::Attribute(
                            Dumb::Render::Geometry::ComponentType::FLOAT, 2, false,
                            DFE_BUFFER_STRIDE, sizeof(GLfloat) *  2, 1) },
                    { DFE_TOP_TEX_INDEX,
                    Dumb::Render::Geometry::Attribute(
                            Dumb::Render::Geometry::ComponentType::FLOAT, 2, false,
                            DFE_BUFFER_STRIDE, sizeof(GLfloat) *  4, 1) },
                    { DFE_DOWN_TEX_INDEX,
                    Dumb::Render::Geometry::Attribute(
                            Dumb::Render::Geometry::ComponentType::FLOAT, 2, false,
                            DFE_BUFFER_STRIDE, sizeof(GLfloat) *  6, 1) },
                    { DFE_COLOR_INDEX,
                    Dumb::Render::Geometry::Attribute(
                            Dumb::Render::Geometry::ComponentType::UNSIGNED_BYTE, 4, false,
                            DFE_BUFFER_STRIDE, sizeof(GLfloat) *  8, 1) },
                    });

#define DFE_DECORATION_SPAN 0
#define DFE_DECORATION_FONT 1
#define DFE_DECORATION_COLOR 2

        //   ----------------------------------
        void Cache::computeDefaultDecoration() {
            const int length = _text.length();
            _decorations.clear();
            for(int i = 0; i < length; ++i) {
                _decorations.push_back(InnerDecoration(_font, _color));
            }
        }

        //   ------------------------
        void Cache::computeDecoration(std::initializer_list<Decoration> decoration) {
            computeDefaultDecoration();
            for(auto &i : decoration) {
                addDecoration(i, false);
            }
        }

        //   --------------------
        void Cache::fillVoidGlyph(GLfloat *ptr) {
            memset(ptr, 0, DFE_BUFFER_STRIDE);
        }

        //   --------------------
        void Cache::computeBuffer(unsigned int size) {
            const unsigned int length = _text.length();
            // Allocate the buffer.
            if(_capacity < length) {
                if(0 != _buffer) {
                    delete []_buffer;
                }
                _capacity = length;
                _buffer = new GLfloat[_capacity*DFE_BUFFER_ELEMENT_COUNT];
            }
            stbtt_aligned_quad quad;
            stbtt_packedchar *data;
            float xpos = static_cast<float>(_position.x);
            float ypos = static_cast<float>(_position.y);
            GLfloat *ptr = _buffer;
            // Second pass : Populate.
            unsigned int glyph = 0;
            _glyphs.clear();
            icu::StringCharacterIterator it(_text);
            for(it.setToStart(); it.hasNext(); ++glyph) {
                // Retrieve decoration.
                InnerDecoration &glyphDecoration = _decorations[glyph];
                const Wrapper *curFont = std::get<0>(glyphDecoration);
                glm::vec4 curColor = std::get<1>(glyphDecoration);
                UChar32 codepoint = it.next32PostInc();
                if(0 != curFont) {
                    UChar32 start = static_cast<UChar32>(curFont->getStartingCodePoint());
                    UChar32 last = start + static_cast<UChar32>(curFont->getGlyphsCount());
                    data = curFont->_data;
                    // Silently ignore out of range characters.
                    if((codepoint >= start) && (codepoint <= last)) {
                        _glyphs.push_back(glm::vec2(xpos, ypos));
                        stbtt_GetPackedQuad(data, size, size, codepoint - start, &xpos, &ypos, &quad, 0);
                        ptr[ 0] = quad.x0;
                        ptr[ 1] = quad.y0;
                        ptr[ 2] = quad.x1 - quad.x0;
                        ptr[ 3] = quad.y1 - quad.y0;
                        ptr[ 4] = quad.s0;
                        ptr[ 5] = quad.t0;
                        ptr[ 6] = quad.s1;
                        ptr[ 7] = quad.t1;
                        unsigned char *colorBuffer = (unsigned char *) (ptr + 8);
                        colorBuffer[0] = curColor.r;
                        colorBuffer[1] = curColor.g;
                        colorBuffer[2] = curColor.b;
                        colorBuffer[3] = curColor.a;
                    } else {
                        fillVoidGlyph(ptr);
                    }
                } else {
                    fillVoidGlyph(ptr);
                }
                ptr += DFE_BUFFER_ELEMENT_COUNT;
            }
        }

        // ---------
        Cache::Cache(const Wrapper *def,
                glm::vec2 pos,
                const icu::UnicodeString &text,
                glm::vec4 color,
                unsigned int size) : _buffer(0), _capacity(0),
        _position(pos), _font(def), _color(color), _text(text), _size(size) {
            computeDefaultDecoration();
            computeBuffer(size);
        }

        // ---------
        Cache::Cache(const Wrapper *def,
                glm::vec2 pos,
                const icu::UnicodeString &text,
                glm::vec4 color,
                std::initializer_list<Decoration> decoration,
                unsigned int size) : _buffer(0), _capacity(0),
        _position(pos), _font(def), _color(color), _text(text), _size(size) {
            computeDecoration(decoration);
            computeBuffer(size);
        }

        //   -------------
        void Cache::moveTo(glm::vec2 pos) {
            // First, compute the initial position.
            const unsigned int length = _text.length();
            if((0 != _buffer) && (length > 0)) {
                glm::vec2 diff = pos - _position;
                _position = pos;
                GLfloat *ptr = _buffer;
                for(unsigned int i = 0; i < length; ++i,
                        ptr += DFE_BUFFER_ELEMENT_COUNT) {
                    ptr[0] += diff.x;
                    ptr[1] += diff.y;
                }
            }
        }

        //        -----------------
        glm::vec4 Cache::computeBox() {
            const unsigned int length = _text.length();
            return computeBox(0, length);
        }

        //        ---------------------
        glm::vec4 Cache::computeBox(unsigned int offset, unsigned int length) {
            glm::vec4 result = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            if(0 != _buffer) {
                GLfloat *ptr = _buffer;
                GLfloat value;
                unsigned int size = _text.length();
                if(offset + length > size) {
                    if(offset > size) {
                        length = 0;
                    } else {
                        length = size - offset;
                    }
                }
                ptr = _buffer + (offset * DFE_BUFFER_ELEMENT_COUNT);
                result.x = ptr[0];
                result.y = ptr[1];
                result.z = ptr[2] + ptr[0];
                result.w = ptr[3] + ptr[1];
                for(unsigned int j = 0;
                        j < length;
                        ++j, ptr += DFE_BUFFER_ELEMENT_COUNT) {
                    if(ptr[0] < result.x) {
                        result.x = ptr[0];
                    }
                    if(ptr[1] < result.y) {
                        result.y = ptr[1];
                    }
                    value = ptr[2] + ptr[0];
                    if(value > result.z) {
                        result.z = value;
                    }
                    value = ptr[3] + ptr[1];
                    if(value > result.w) {
                        result.w = value;
                    }
                }
            }
            return result;
        }

        //   --------------
        void Cache::setText(const icu::UnicodeString &text, bool keep) {
            int oldTextLength = _text.length();
            _text = text;
            int length = _text.length();
            if(!keep) {
                oldTextLength = 0;
                _decorations.clear();
            }
            for(int i = oldTextLength; i < length; ++i) {
                _decorations.push_back(InnerDecoration(_font, _color));
            }
            _glyphs.clear();
            computeBuffer(_size);
        }

        //   -------------
        void Cache::append(const icu::UnicodeString &src) {
            icu::UnicodeString newText = _text;
            newText.append(src);
            setText(newText, true);
        }

        //   -------------
        void Cache::append(const UChar32 chr) {
            icu::UnicodeString newText = _text;
            newText.append(chr);
            setText(newText, true);
        }

        //   -------------
        void Cache::remove(int nb) {
            // This one is 'easy'. We just lower the glyph count.
            if(nb > 0) {
                int length = _text.length();
                length = std::max(0, length - nb);
                _text.truncate(length);
            }
        }

        //   --------------------
        void Cache::addDecoration(Decoration decoration, bool compute) {
            int length = _text.length();
            glm::ivec2 span = std::get<DFE_DECORATION_SPAN>(decoration);
            const Wrapper *font = std::get<DFE_DECORATION_FONT>(decoration);
            const glm::vec4 *coloration = std::get<DFE_DECORATION_COLOR>(decoration);
            glm::vec4 colApply;
            const Wrapper *fontApply;
            int start = std::max(0, span.x);
            int end = std::min(length, span.x + span.y);
            int size = _decorations.size();
            if(end > size) {
                for(int j = size; j < end; ++j) {
                    _decorations.push_back(InnerDecoration(_font, _color));
                }
            }
            for(int j = start; j < end; ++j) {
                if(0 == font) {
                    fontApply = std::get<0>(_decorations[j]);
                } else {
                    fontApply = font;
                }
                if(0 == coloration) {
                    colApply = std::get<1>(_decorations[j]);
                } else {
                    colApply = *coloration;
                }
                _decorations[j] = InnerDecoration(fontApply, colApply);
            }
            if(compute) {
                computeBuffer(_size);
            }
        }

        //   ----------------------
        void Cache::clearDecoration(bool compute, unsigned int offset, int length) {
            unsigned int size = _decorations.size();
            if(offset < size) {
                if(length < 0) {
                    length = size - offset;
                }
                unsigned int last = offset + length + 1;
                for(unsigned int i = offset; i < last; ++i) {
                    _decorations[i] = InnerDecoration(_font, _color);
                }
                if(compute) {
                    computeBuffer(_size);
                }
            }
        }

        //-----------
        Cache::~Cache() {
            if(0 != _buffer) {
                delete []_buffer;
            }
        }

        //     ----------------
        Cache &Cache::operator=(const Cache &orig) {
            if(&orig != this) {
                if(0 != _buffer) {
                    delete[] _buffer;
                }
                _capacity = orig._capacity;
                _buffer = new GLfloat[_capacity * DFE_BUFFER_ELEMENT_COUNT];
                memcpy(_buffer, orig._buffer, _capacity * DFE_BUFFER_STRIDE);
                _position = orig._position;
                _decorations = orig._decorations;
                _font = orig._font;
                _color = orig._color;
                _text = orig._text;
                _glyphs = orig._glyphs;
                _size = orig._size;
            }
            return *this;
        }

        //   ------------
        unsigned int Cache::fetch(void *dest, unsigned int size) const {
            unsigned int length = _text.length();
            if(size >= length) {
                length *= DFE_BUFFER_STRIDE;
                memcpy(dest, _buffer, length);
            } else {
                length = 0;
            }
            return length;
        }

        //  ---------------
        int aggregateCaches(const std::vector<const Cache *> &texts,
                void *buffer, int size) {
            unsigned int remaining = size;
            unsigned char *current = reinterpret_cast<unsigned char *>(buffer);
            std::vector<const Cache *>::const_iterator it;
            unsigned int count;
            unsigned int total = 0;
            for(it = texts.begin(); it != texts.end(); ++it) {
                count = (*it)->count();
                if(remaining >= count) {
                    total += count;
                    remaining -= count;
                    current += (*it)->fetch(current, remaining);
                } else {
                    break;
                }
            }
            return total;
        }

        //      ----------------
        GLsizei Delegate::update(void *ptr, GLsizei capacity,
                const std::vector<const Cache*> &texts) {
            GLsizei total = (GLsizei) aggregateCaches(texts, ptr, capacity);
            _lastCount = total;
            return total;
        }

        //      ----------------
        GLsizei Delegate::update(void *ptr, GLsizei capacity,
                const void *cache, unsigned int size) {
            GLsizei result = std::min((GLsizei) size, capacity);
            memcpy(ptr, cache, result * DFE_BUFFER_STRIDE);
            _lastCount = result;
            return result;
        }

        //      ----------------
        GLsizei Delegate::update(void *ptr, GLsizei capacity,
                const Cache &cache) {
            cache.fetch(ptr, capacity);
            GLsizei result = cache.count();
            _lastCount = result;
            return result;
        }

        //      ----------------
        GLsizei Delegate::update(void * /* ptr */, GLsizei /* capacity */) {
            return _lastCount;
        }

        //      ----------------
        GLsizei Delegate::update(void *ptr, GLsizei capacity,
                const Wrapper *def,
                glm::vec2 pos,
                const icu::UnicodeString &text,
                glm::vec4 color,
                std::initializer_list<Decoration> decoration) {
            Cache cache(def, pos, text, color, decoration, _size);
            return update(ptr, capacity, cache);
        }

        //      ----------------
        GLsizei Delegate::update(void *vptr, GLsizei capacity,
                const Wrapper *font, glm::vec2 pos, icu::UnicodeString text, glm::vec4 color) {
            GLsizei result = 0;
            if(0 != font) {
                GLfloat *ptr = reinterpret_cast<GLfloat *>(vptr);
                GLsizei count = 0; // Number of glyph to display.
                // Iterate on the text.
                UChar32 start = static_cast<UChar32>(font->getStartingCodePoint());
                UChar32 last = start + static_cast<UChar32>(font->getGlyphsCount());
                stbtt_aligned_quad quad;
                float xpos = static_cast<float>(pos.x);
                float ypos = static_cast<float>(pos.y);
                stbtt_packedchar *data = font->_data; // How handy ...
                icu::StringCharacterIterator it(text);
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
                        unsigned char *colorBuffer = (unsigned char *) (ptr + 8);
                        colorBuffer[0] = color.r;
                        colorBuffer[1] = color.g;
                        colorBuffer[2] = color.b;
                        colorBuffer[3] = color.a;
                        ptr += DFE_BUFFER_ELEMENT_COUNT;
                    }
                }
                result = count;
            }
            return result;
        }

        // ------------
        Delegate::~Delegate() {
            std::map<std::string, Wrapper *>::iterator it;
            for(it = _wrappers.begin(); it != _wrappers.end(); ++it) {
                Log_Info(Dumb::Module::App, "Flushing '%s'", it->first.c_str());
                delete it->second;
            }
        }

        //   ------------------
        void Delegate::viewport(GLfloat startX, GLfloat startY, GLfloat width, GLfloat height) {
            _matrix = glm::ortho(startX, startX + width, height + startY, startY, -1.0f, 1.0f);
        }

        //   ------------------------
        void Delegate::packOversample(stbtt_pack_context &context, const Oversample &oversample, char *font) {
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
                Log_Info(Dumb::Module::App, "Register '%s'", name.c_str());
                _wrappers.insert(std::pair<std::string, Wrapper *>(name, wrapper));
            }
            glm::vec2 ovr = oversample.getOversample();
            stbtt_PackSetOversampling(&context, (unsigned int) ovr.x, (unsigned int) ovr.y);
            if(stbtt_PackFontRanges(&context, reinterpret_cast<unsigned char*>(font),
                        0, packRange, count) == 0) {
                Log_Error(Dumb::Module::App, "Font range loading failure");
            }
            delete []packRange;
        }

        //   ------------------
        void Delegate::packFont(stbtt_pack_context &context, const Resource &resource) {
            std::ifstream fontFile;
            fontFile.open(resource.getPath().c_str(), std::ios::binary|std::ios::ate|std::ios::in);
            // Font file check.
            if(fontFile.is_open()) {
                Log_Info(Dumb::Module::App, "Loading '%s'", resource.getPath().c_str());
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
                Log_Error(Dumb::Module::App, "Failed to open '%s'", resource.getPath().c_str());
            }
        }

        //----------------
        Delegate::Delegate(const std::vector<Resource> &fonts,
                unsigned int size) : _size(size), _lastCount(0) {
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
        }

        //    ----------------------------------------------------------------
        std::vector<std::pair<Dumb::Render::Shader::Type, const char *> >
            Delegate::shaders() const {
                return std::vector<std::pair<Dumb::Render::Shader::Type, const char *> >(
                        {
                        { Dumb::Render::Shader::VERTEX_SHADER,   s_dfe_vertexShaderInstanced },
                        { Dumb::Render::Shader::FRAGMENT_SHADER, s_dfe_fragmentShader }
                        });
            }

        //    -----------------------------------------------------------------------
        std::vector<std::pair<unsigned int, Dumb::Render::Geometry::Attribute> >
            Delegate::attributes() const {
                return s_attributes;
            }


        //   --------------
        void Delegate::init(Dumb::Render::Program &program) {
            _uniformMatrix  = program.getUniformLocation("un_matrix");
            _uniformTexture = program.getUniformLocation("un_texture");
        }

        //   ----------------
        void Delegate::update(Dumb::Render::Program &program) {
            program.uniform(_uniformMatrix, false, _matrix);
            glBindTexture(GL_TEXTURE_2D, _atlas);
        }

        //   --------------------
        void Delegate::postRender() {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    } // 'Font' namespace.
} // 'Dumb' namespace.
