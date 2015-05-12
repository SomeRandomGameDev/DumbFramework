#include <runner.hpp>

#include <iostream>
#include <fstream>

#include <GL/glu.h>

/*
   g++ adviser.o font.cpp -I../src -I../../Libraries/stb -lglfw -lGL -lGLU -lm -o fonttest -std=c++11
 */

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>
#include <stb_truetype.h>

#include <string>
#include <vector>
#include <map>

/* Of course, we'll use ICU for text management. The std c++ string library is far from perfect.
   And the Dumb Framework won't use the Boost lib ! - Over my dead body ! - */
#include <unicode/unistr.h>

/**
 * Size of the font atlas.
 */
#define FONT_ATLAS_SIZE 1024

/**
 * Large font size (in pixels).
 */
#define LARGE_FONT_SIZE 72.0f

/**
 * Dumb Framework Font engine Default Starting Codepoint.
 */
#define DFF_START_CODEPOINT_DEFAULT 32U
/**
 * Dumb Framework Font engine Default number of glyphs to load.
 */
#define DFF_GLYPHS_COUNT_DEFAULT 95U
/**
 * Dumb Framework Font engine Default Oversampling on X-Axis.
 */
#define DFF_OVERSAMPLE_X_DEFAULT 1
/**
 * Dumb Framework Font engine Default Oversampling on Y-Axis.
 */
#define DFF_OVERSAMPLE_Y_DEFAULT 1
/**
 * Dumb Framework Font engine Default Font Size (in pixels).
 */
#define DFF_SIZE_DEFAULT 32.0
/**
 * Dumb Framework Font engine Default Texture Atlas Size.
 */
#define DFF_ATLAS_SIZE_DEFAULT 1024


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
                Range() : start(DFF_START_CODEPOINT_DEFAULT),
                count(DFF_GLYPHS_COUNT_DEFAULT),
                size(DFF_SIZE_DEFAULT) {}
                /**
                 * Full constructor.
                 * @param [in] id Font Range Identifier.
                 * @param [in] st Starting code point.
                 * @param [in] ct Glyphs count.
                 * @param [in] sz Size (in pixels).
                 */
                Range(const char *id, unsigned int st, unsigned int ct, double sz) :
                    identifier(id), start(st), count(ct), size(sz) {}

                /**
                 * @return Starting code point.
                 */
                inline unsigned int getStartingCodePoint() const {
                    return start;
                }

                /**
                 * @return Number of glyphs.
                 */
                inline unsigned int getGlyphsCount() const {
                    return count;
                }

                /**
                 * @return Size (in pixels).
                 */
                inline double getSize() const {
                    return size;
                }

                /**
                 * @return Human readable identifier.
                 */
                inline const std::string &getIdentifier() const {
                    return identifier;
                }
            private:
                /**
                 * Human readable identifier.
                 */
                std::string identifier;
                /**
                 * Starting code point.
                 */
                unsigned int start;
                /**
                 * Number of glyphs to load.
                 */
                unsigned int count;
                /**
                 * Size (in pixels).
                 */
                double size;
        };

        /**
         * Oversampled ranges of glyphs definition.
         */
        class Oversample {
            public:
                /**
                 * Default constructor.
                 */
                Oversample() : oversample(DFF_OVERSAMPLE_X_DEFAULT, DFF_OVERSAMPLE_Y_DEFAULT) {
                    // Nothing to do.
                }
                /**
                 * Complete constructor.
                 * @param [in] ovr Oversampling (in both axis).
                 * @param [in] rgs Font definition ranges.
                 */
                Oversample(glm::vec2 ovr, std::vector<Range> rgs) : oversample(ovr), ranges(rgs) {
                    // Nothing special to do.
                }
                /**
                 * @return Oversampling hint.
                 */
                inline glm::vec2 getOversample() const {
                    return oversample;
                }
                /**
                 * @return Ranges.
                 */
                inline std::vector<Range> getRanges() const {
                    return ranges;
                }
            private:
                /**
                 * Oversampling hint.
                 */
                glm::vec2 oversample;
                /**
                 * Ranges.
                 */
                std::vector<Range> ranges;
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
                Resource(std::string pt, std::vector<Oversample> spc) : path(pt), specs(spc) {
                    /* Nothing more to do. */
                }

                /**
                 * @return Font path.
                 */
                inline const std::string& getPath() const {
                    return path;
                }

                /**
                 * @return Font specs.
                 */
                inline std::vector<Oversample> getSpecs() const {
                    return specs; // Ugly copy, but safe. The cardinality justifies it.
                }

            private:
                /**
                 * Font path.
                 */
                std::string path;
                /**
                 * Font specs.
                 */
                std::vector<Oversample> specs;
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
            Wrapper() : data(0) { /* Nope */ }
            private:
            /**
             * Private constructor (used by the engine only).
             */
            Wrapper(Range originator, stbtt_packedchar *dt) :
                Range(originator), data(dt) {
                    /* Nothing special to be done. */
                }
            /**
             * Private destructor.
             */
            ~Wrapper() {
                if(0 != data) {
                    delete []data;
                }
            }
            /**
             * @return Packed char data information.
             */
            inline stbtt_packedchar *getData() { return data; }
            private:
            /**
             * Information about characters in the pack.
             */
            stbtt_packedchar *data;
        };

        /**
         * Text Area Identifier.
         */
        typedef unsigned long long int Area;

        /**
         * El Font Engine.
         */
        class Engine {
            public:
                /**
                 * Constructor.
                 * @param [in] fonts List of fonts to load.
                 * @param [in] size Size of the font atlas. The atlas is a square texture.
                 */
                Engine(const std::vector<Resource> &fonts,
                        unsigned int size);

                /**
                 * Gentle destructor.
                 */
                ~Engine();

                /**
                 * @return The font atlas identifier.
                 */
                inline GLuint getAtlas() {
                    return atlas;
                }

                /**
                 * Retrieve a font wrapper.
                 * @param id Identifier previously specified at engine initialisation.
                 */
                const Wrapper *getFont(const std::string &id) const {
                    std::map<std::string, Wrapper *>::const_iterator it = wrappers.find(id);
                    Wrapper *result = (it != wrappers.end())?it->second:0;
                    return result;
                }

                /**
                 * Simple text area creation.
                 * @param [in] defFont Default font.
                 * @param [in] pos Position in logical coordinate system.
                 * @param [in] text Starting text.
                 * @return Area identifier. This identifier can be '0' if something fails.
                 */
                Area createArea(Wrapper *defFont, glm::vec2 pos, icu::UnicodeString text);

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
                GLuint atlas;
                /**
                 * Font Wrappers.
                 */
                std::map<std::string, Wrapper*> wrappers;
        };
    } // 'Font' namespace.
} // 'Dumb' namespace.


//######## IMPLEMENTATION ################

namespace Dumb {
    namespace Font {

        //   ------------------
        Area Engine::createArea(Wrapper *defFont, glm::vec2 pos, icu::UnicodeString text) {
            /* TODO For the moment, it's simple. But in further devs, this method will be
               a facade for a more extensive method :
               Area createArea(Parameters params);
               Where 'Parameters' is one hell of a class containing tons of informations about
               the area to create/maintain, etc. */
            return 0;
        }

        // ------------
        Engine::~Engine() {
            std::map<std::string, Wrapper *>::iterator it;
            for(it = wrappers.begin(); it != wrappers.end(); ++it) {
                std::cout << "Flushing '" << it->first << "'" << std::endl; // TODO Logging.
                delete it->second;
            }
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
                wrappers.insert(std::pair<std::string, Wrapper *>(name, wrapper));
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
        Engine::Engine(const std::vector<Resource> &fonts, unsigned int size = DFF_ATLAS_SIZE_DEFAULT) {
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
            glGenTextures(1, &atlas);
            glBindTexture(GL_TEXTURE_2D, atlas);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
                    size, size, 0, GL_ALPHA, GL_UNSIGNED_BYTE, buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            delete []buffer;
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
        Example() : closeFlag(1), engine(0) {
            // Nothing special to do.
        }

        /**
         * The mandatory destructor.
         */
        ~Example() {
            if(0 != engine) {
                delete engine;
            }
        }
    private:
        /**
         * If 0, close the app.
         */
        int closeFlag;

        /**
         * Font engine.
         */
        Dumb::Font::Engine *engine;

        /**
         * Screen size (in pixels).
         */
        glm::vec2 screenSize;
};

void Example::init(Dumb::Core::Application::Adviser *adviser) {
    Dumb::Core::Application::Video::Monitor monitor = adviser->getPrimaryMonitor();
    adviser->setMonitor(monitor);
    Dumb::Core::Application::Video::Mode current = monitor.getCurrentMode();
    adviser->setVideoMode(current);
    screenSize = current.getResolution();
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

    engine = new Engine(resource, 1200);
}

int Example::render() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glViewport(0,0,screenSize.x,screenSize.y);
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,screenSize.x,screenSize.y,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, engine->getAtlas());
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 1);
    // Center the texture.
    int offX;
    int offY;
    int sz;
    if(screenSize.x > screenSize.y) {
        sz = screenSize.y;
        offY = 0;
        offX = (screenSize.x - screenSize.y) / 2;
    } else {
        sz = screenSize.x;
        offX = 0;
        offY = (screenSize.y - screenSize.x) / 2;
    }
    glTexCoord2f(0, 0); glVertex2f(offX, offY);
    glTexCoord2f(1, 0); glVertex2f(offX + sz, offY);
    glTexCoord2f(1, 1); glVertex2f(offX + sz, offY + sz);
    glTexCoord2f(0, 1); glVertex2f(offX, offY + sz);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    return closeFlag;
}
void Example::handleUnicodeCharacter(unsigned int) {}
void Example::handleUnicodeModifierCharacter(unsigned int,int) {}
void Example::handleKey(int,int,int,int) {
    closeFlag = 0;
}
void Example::handleMousePosition(double xp,double yp) {}
void Example::handleMouseButton(int,int,int) {}
void Example::handleMouseScroll(double,double) {}
void Example::handleWindowClose() {}
void Example::handleWindowSize(int, int) {}
void Example::handleWindowIconify(int) {}
void Example::close() {}



SIMPLE_APP(Example)
