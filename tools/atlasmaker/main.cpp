#include <AntTweakBar.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Ultra speed coding !
 */

#define MAX_NUMBER_OF_TEXTURE 8
#define TEXTURE_FILENAME_SIZE 4096

/*
 * Control structure.
 */
class ControlStructure {
public:
    char         _textureToLoad[TEXTURE_FILENAME_SIZE];
    char**       _textures;
    unsigned int _textureCount;
    TwBar*       _mainBar;
    char *       _active;

public:
    ControlStructure() {
        _active = new char[MAX_NUMBER_OF_TEXTURE];
        _textureCount = 0;
        _textures = new char*[MAX_NUMBER_OF_TEXTURE];
    }

    ~ControlStructure() {
        delete []_active;
        for(unsigned int i = 0; i < _textureCount; ++i) {
            delete []_textures[i];
        }
        delete []_textures;
        // ## What about the bar ?
    }
};


/*
 * GLFW Error callback.
 */
void glfwSimpleErrorCallback(int errorCode, const char *description) {
    fprintf(stderr, "##! GLFW Error(%d) : %s\n", errorCode, description);
}

/*
 * GLFW Main Key callback.
 */
void glfwMainKeyCallback(GLFWwindow *window, int key, int scancode,
        int action, int mods) {
    if(!TwEventKeyGLFW(key, action)) {
        if(GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
}

/*
 * GLFW Main Mouse position callback.
 */
void glfwMainMousePositionCallback(GLFWwindow *window, double x, double y) {
    if(!TwEventMousePosGLFW((int) x, (int) y)) {
        /* TODO */
    }
}

/*
 * GLFW Main Mouse click callback.
 */
void glfwMainMouseClickCallback(GLFWwindow *window, int button, int action, int mods) {
    if(!TwEventMouseButtonGLFW(button, action)) {
        /* TODO */
    }
}

/*
 * GLFW Main char callback.
 */
void glfwMainCharCallback(GLFWwindow *window, unsigned int chr) {
    int press = TwEventCharGLFW((int) chr, GLFW_PRESS);
    int release = TwEventCharGLFW((int) chr, GLFW_RELEASE);

    if(press != 0 && release != 0) {
        /* TODO */
    }
}

/*
 * GLFW Main Mouse Whell callback.
 */
void glfwMainMouseWheelCallback(GLFWwindow *window, double xOffset, double yOffset) {
    static double position = 0;
    position += yOffset;
    if(!TwEventMouseWheelGLFW((int) position)) {
        /* TODO */
    }
}


// ## Add Callback for texture removal.

void TW_CALL setTextureToLoad(const void *value, void *cD)
{
  ControlStructure *clientData = (ControlStructure *) cD;
  const char *src = (const char *)value;
  strncpy(clientData->_textureToLoad, src, TEXTURE_FILENAME_SIZE);
  clientData->_textureToLoad[TEXTURE_FILENAME_SIZE-1] = '\0';

  printf("Let's load texture %s\n", clientData->_textureToLoad);
  if((clientData->_textureCount < MAX_NUMBER_OF_TEXTURE) && (clientData->_textureToLoad[0] != 0)) {
        char properties[128 + TEXTURE_FILENAME_SIZE];
        char name[64];
        snprintf(name, 64, "texture%d", clientData->_textureCount);
        snprintf(properties, 128 + TEXTURE_FILENAME_SIZE, " group=Load true=Enabled false=Disabled label='%s'",
            clientData->_textureToLoad);
        printf("Add '%s'\n", name);
        TwAddVarRW(clientData->_mainBar, name, TW_TYPE_BOOL8, clientData->_active + clientData->_textureCount,
            properties );
        ++clientData->_textureCount;
  }
  
}

void TW_CALL getTextureToLoad(void *value, void *cD)
{
  ControlStructure *clientData = (ControlStructure *) cD;
  char *dest = (char *)value;
  strncpy(dest, clientData->_textureToLoad, TEXTURE_FILENAME_SIZE-1);
  dest[TEXTURE_FILENAME_SIZE-1] = '\0';
}


/*
 * Main Procedure/Entry Point.
 */
int main() {
    GLFWwindow *window;
    ControlStructure *controlStructure = new ControlStructure();
    int initW;
    int initH;

    glfwSetErrorCallback(glfwSimpleErrorCallback);

    if(!glfwInit()) {
        fprintf(stderr, "##! Error while initializing GLFW\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(800, 600, "Atlas Maker",
            NULL, NULL);

    if(0 == window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(window, controlStructure);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, glfwMainKeyCallback);
    glfwSetCharCallback(window, glfwMainCharCallback);
    glfwSetMouseButtonCallback(window, glfwMainMouseClickCallback);
    glfwSetScrollCallback(window, glfwMainMouseWheelCallback);
    glfwSetCursorPosCallback(window, glfwMainMousePositionCallback);
    glfwGetWindowSize(window, &initW, &initH);

    TwInit(TW_OPENGL, 0);
    TwWindowSize(initW, initH);
    controlStructure->_mainBar = TwNewBar("Main");
    TwDefine(" Main label='Main control'");

    TwAddVarCB(controlStructure->_mainBar, "TextureToLoad",
            TW_TYPE_CSSTRING(TEXTURE_FILENAME_SIZE),
            setTextureToLoad,
            getTextureToLoad,
            controlStructure, " group=Load");

    TwDefine(" Main/Load label='Texture Management'");
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        TwDraw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    TwTerminate();
    glfwTerminate();

    return 0;
}
