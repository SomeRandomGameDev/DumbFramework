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
#ifndef _DUMBFRAMEWORK_FONT_
#define _DUMBFRAMEWORK_FONT_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

#include <stb_rect_pack.h>
#include <stb_truetype.h>

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <initializer_list>

/* Of course, we'll use ICU for text management. The std c++ string library is far from perfect.
   And the Dumb Framework won't use the Boost lib ! - Over my dead body ! - */
#include <unicode/unistr.h>
#include <unicode/schriter.h>

#include <DumbFramework/render/vertexbuffer.hpp>
#include <DumbFramework/render/vertexstream.hpp>
#include <DumbFramework/render/program.hpp>
#include <DumbFramework/render/renderer.hpp>

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
/**
 * Default printing color.
 */
#define DFE_COLOR_DEFAULT        glm::fvec3(1.0f, 1.0f, 1.0f)


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
        // Cache forward declaration.
        class Cache;

        /**
         * Font wrapper. Addressable font descriptor for printing usage.
         */
        class Wrapper : public Range {
            friend class Engine;
            friend class Cache;

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
         * Decoration type.
         * A tuple consisting in (offset-length, font, color, underline-flag, stroke-flag).
         */
        typedef std::tuple<glm::ivec2, const Wrapper *, const glm::fvec3 *, bool, bool> Decoration;

        /**
         * Decoration type for internal use only.
         */
        typedef std::tuple<const Wrapper *, glm::fvec3, bool, bool> InnerDecoration;

        /**
         * Caching class. Allows precomputing text decoration and positionning.
         */
        class Cache {
            public:

                // TODO Add a constructor with size constraint (restraining box).

                /**
                 * Constructor.
                 * @param [in] def Default font to be used.
                 * @param [in] pos Position in logical coordinate system.
                 * @param [in] text Starting text.
                 * @param [in] color Default color.
                 * @param [in] decoration A list of decoration hints. A decoration hint is
                 * a tuple containing the following : A start and end index, a font wrapper, a color,
                 * a flag to underlining, a flag for stroke.
                 * @param [in] size Font atlas size.
                 */
                Cache(const Wrapper *def,
                        glm::vec2 pos,
                        const icu::UnicodeString &text,
                        glm::vec3 color,
                        std::initializer_list<Decoration> decoration,
                        unsigned int size);

                /**
                 * Constructor for simple text.
                 * @param [in] font Font to be used.
                 * @param [in] pos Position in logical coordinate system.
                 * @param [in] text Starting text.
                 * @param [in] color Color.
                 * @param [in] size Font atlas size.
                 */
                Cache(const Wrapper *def,
                        glm::vec2 pos,
                        const icu::UnicodeString &text,
                        glm::vec3 color,
                        unsigned int size);

                /**
                 * Copy constructor.
                 * @param [in] orig Origin.
                 */
                Cache(const Cache &orig) {
                    _buffer = 0;
                    *this = orig;
                }

                /**
                 * Copy operator.
                 * @param [in] orig Origin.
                 */
                Cache &operator=(const Cache &orig);

                /**
                 * Destructor.
                 */
                ~Cache();

                /**
                 * Fetch the computed buffer.
                 * @param [out] dest Destination buffer.
                 * @param [in] size Size of the destination buffer.
                 */
                void fetch(GLfloat *dest, unsigned int size) const;

                /**
                 * @return The number of glyphs.
                 */
                inline unsigned int count() const { return _text.length(); }

                /**
                 * Change the text position.
                 * @param [in] pos New position.
                 */
                void moveTo(glm::vec2 pos);

                /**
                 * Compute the current bounding box.
                 * @return The text bounding box.
                 */
                glm::vec4 computeBox();

                /**
                 * Reset text.
                 * @param [in] text Text to set.
                 * @param [in] keep If 'true', keep the current decoration, else, reset it
                 * with default values (default : false).
                 */
                void setText(const icu::UnicodeString &text, bool keep = false);

                /**
                 * Append a text.
                 * @param [in] src Text to append.
                 */
                void append(const icu::UnicodeString &src);

                /**
                 * Append a character.
                 * @param [in] chr Character to append.
                 */
                void append(const UChar32 chr);

                /**
                 * Remove last characters.
                 * @param [in] nb Number of character to remove (default : 1).
                 */
                void remove(int nb = 1);

                /**
                 * Add a decoration.
                 * @param [in] decoration Decoration to add.
                 * @param [in] compute Recompute the whole text (default : true).
                 */
                void addDecoration(Decoration decoration, bool compute = true);

                /**
                 * Clear the decoration.
                 * @param [in] compute Recompute the whole text (default : true).
                 * @param [in] offset Start of the cleansing (default = 0).
                 * @param [in] length Length of the cleansing (default = -1, i.e
                 * to the end of the text.
                 */
                void clearDecoration(bool compute = true,
                        unsigned int offset = 0, int length = -1);

            private:
                /**
                 * Compute default decoration (according to input text).
                 */
                void computeDefaultDecoration();

                /**
                 * Compute the decoration based on external declaration.
                 * @param [in] decoration Decoration ranges.
                 */
                void computeDecoration(std::initializer_list<Decoration> decoration);

                /**
                 * (Re-)compute the drawing buffer.
                 * @param [in] size Font atlas size.
                 */
                void computeBuffer(unsigned int size);

                /**
                 * Fill void glyph (in case of invalid character or null font).
                 * @param [in] ptr Entry buffer.
                 */
                void fillVoidGlyph(GLfloat *ptr);
            private:
                /**
                 * Buffer content.
                 */
                GLfloat *_buffer;

                /**
                 * Glyphs capacity.
                 * The buffer can be wider than the current usage.
                 */
                unsigned int _capacity;

                /**
                 * Attended position.
                 */
                glm::vec2 _position;

                /**
                 * Current decoration cache.
                 */
                std::vector<InnerDecoration> _decorations;

                /**
                 * Default font.
                 */
                const Wrapper *_font;

                /**
                 * Default color.
                 */
                glm::vec3 _color;

                /**
                 * Displayed text.
                 */
                icu::UnicodeString _text;

                /**
                 * Glyphs position.
                 */
                std::vector<glm::vec2> _glyphs;

                /**
                 * Atlas font size.
                 */
                unsigned int _size;
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
                        unsigned int capacity = DFE_GLYPH_CAPACITY_DEFAULT,
                        unsigned int size = DFE_ATLAS_SIZE_DEFAULT);

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
                 * @param [in] color Text Color (RGB, [0..1]).
                 */
                void print(const Wrapper *font,
                        glm::vec2 pos,
                        icu::UnicodeString text,
                        glm::vec3 color = DFE_COLOR_DEFAULT);

                /**
                 * Decorated text printing.
                 * @param [in] def Default font to be used.
                 * @param [in] pos Position in logical coordinate system.
                 * @param [in] text Starting text.
                 * @param [in] color Default color.
                 * @param [in] decoration A list of decoration hints. A decoration hint is
                 * a tuple containing the following : A start and end index, a font wrapper, a color,
                 * a flag to underlining, a flag for stroke.
                 */
                void print(const Wrapper *def,
                        glm::vec2 pos,
                        const icu::UnicodeString &text,
                        glm::vec3 color,
                        std::initializer_list<Decoration> decoration);

                /**
                 * Print a precomputed text.
                 * @param [in] cache Precomputed text.
                 */
                void print(const Cache &cache);

                /**
                 * Print a list of precomputed texts.
                 * @param [in] texts List of precomputed texts.
                 */
                void print(const std::vector<const Cache*> &texts);

                /**
                 * Set the viewport. This method is pretty straightforward as we only
                 * specify the viewport dimension in pixels. The coordinate system is meant
                 * to have (0, 0) in top-left corner and (width, height) in bottom-right
                 * corner.
                 * @param [in] width Width (not only pixels).
                 * @param [in] height Height (not only pixels).
                 */
                void setViewport(GLfloat width, GLfloat height);

                /**
                 * @return Font atlas size.
                 */
                inline unsigned int size() { return _size; }

                /* TODO Serious business here :
                   - Logical viewport management.
                   - VAO management.
                   - Text Areas storage and maintenance.
                   - Rendering process. */
            private:
                /**
                 * Private copy constructor.
                 */
                Engine(const Engine &) : _atlas(0) {}

                /**
                 * Private copy operator.
                 */
                Engine &operator=(const Engine &) { return *this; }

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

#pragma GCC diagnostic pop

#endif
