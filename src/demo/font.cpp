#include <runner.hpp>

#include <iostream>
#include <fstream>

#include <GL/glu.h>

/*
   g++ adviser.o font.cpp -I../src -I../../Libraries/stb -lglfw -lGL -lGLU -lm
 */

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>
#include <stb_truetype.h>

/**
 * Size of the font atlas.
 */
#define FONT_ATLAS_SIZE 1024

/**
 * Large font size (in pixels).
 */
#define LARGE_FONT_SIZE 72.0f

/**
 * Sample application : Minimal example.
 */
class Example {
    DECLARE_WRAPPER_METHODS
    public:
        /**
         * Default Constructor.
         */
        Example() : closeFlag(1), chardata(0), texture(32) {
            // Nothing special to do.
        }

        /**
         * The mandatory destructor.
         */
        ~Example() {
            if(0 != chardata) {
                delete []chardata;
            }
        }
    private:
        /**
         * If 0, close the app.
         */
        int closeFlag;

        /**
         * Packed character information.
         */
        stbtt_packedchar *chardata;

        /**
         * Atlas Texture.
         */
        GLuint texture;

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
    // Now, load the font and build the font atlas.
    // For test purpose, the font is located at "./Vera.ttf".
    std::ifstream fontFile;
    fontFile.open("Vera.ttf", std::ios::binary|std::ios::ate|std::ios::in);
    if(fontFile.is_open()) {
        std::streampos size = fontFile.tellg();
        char *fontFileContent = new char[size];
        fontFile.seekg(0, std::ios::beg);
        fontFile.read(fontFileContent, size);
        fontFile.close();

        std::cout << "Font file is " << size << " bytes long." << std::endl;
        // Build the atlas using stb_truetype.
        stbtt_pack_context context;

        unsigned char *buffer = new unsigned char[FONT_ATLAS_SIZE * FONT_ATLAS_SIZE];

        // TODO Complete call explanation.
        stbtt_PackBegin(&context, buffer, FONT_ATLAS_SIZE, FONT_ATLAS_SIZE, 0, 1, NULL);
        stbtt_pack_range singleRange;
        singleRange.font_size = LARGE_FONT_SIZE;
        singleRange.first_unicode_char_in_range = 32;
        singleRange.num_chars_in_range = 95;
        chardata = new stbtt_packedchar[95];
        singleRange.chardata_for_range = chardata;
        // TODO What about chardata_for_range ?
        stbtt_PackFontRanges(&context, reinterpret_cast<unsigned char*>(fontFileContent), 0, &singleRange, 1);
        stbtt_PackEnd(&context);

        // Create the GL Texture.
        glGenTextures(1, &texture);
        std::cout << "Allocate texture '" << texture << "'" << std::endl;
        if(glIsTexture(texture) != GL_TRUE) {
            std::cerr << "Error allocating the texture !" << std::endl;
        }
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
                FONT_ATLAS_SIZE, FONT_ATLAS_SIZE, 0, GL_ALPHA, GL_UNSIGNED_BYTE, buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        GLenum glErr = glGetError();
        switch(glErr) {
            case GL_NO_ERROR:
                std::cout << "No Error" << std::endl;
                break;
            case GL_INVALID_OPERATION:
                std::cout << "Invalid Operation" << std::endl;
                break;
            default:
                std::cout << "Other error : " << glErr << std::endl;
                break;
        }


        // The texture is done. We can trash the temporary buffer.
        delete []buffer;
        // Finished with the font file.
        delete []fontFileContent;
    } else {
        std::cerr << "Unable to open the font file." << std::endl;
    }
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

    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 1);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(screenSize.x, 0);
    glTexCoord2f(1, 1); glVertex2f(screenSize.x, screenSize.y);
    glTexCoord2f(0, 1); glVertex2f(0, screenSize.y);
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
