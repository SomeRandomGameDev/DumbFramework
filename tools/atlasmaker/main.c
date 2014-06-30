#include <AntTweakBar.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUMBER_OF_TEXTURE 8
#define TEXTURE_FILENAME_SIZE 255
/*
 * Control structure.
 */
typedef struct {
    char _textureFile[TEXTURE_FILENAME_SIZE][MAX_NUMBER_OF_TEXTURE];
    unsigned int _textureCount;
} ControlStructure;


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
    if(!TwEventCharGLFW((int) chr, GLFW_PRESS)) {
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


/*
 * Main Procedure/Entry Point.
 */
int main() {
    GLFWwindow *window;
    TwBar *mainBar;
    ControlStructure *controlStructure = (ControlStructure *) malloc(sizeof(ControlStructure));
    int initW;
    int initH;

    glfwSetErrorCallback(glfwSimpleErrorCallback);

    if(!glfwInit()) {
        fprintf(stderr, "##! Error while initializing GLFW\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(800, 600, "Atlas Maker",
            glfwGetPrimaryMonitor(), NULL);

    if(0 == window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, glfwMainKeyCallback);
    glfwSetCharCallback(window, glfwMainCharCallback);
    glfwSetMouseButtonCallback(window, glfwMainMouseClickCallback);
    glfwSetScrollCallback(window, glfwMainMouseWheelCallback);
    glfwSetCursorPosCallback(window, glfwMainMousePositionCallback);
    glfwGetWindowSize(window, &initW, &initH);

    TwInit(TW_OPENGL, 0);
    TwWindowSize(initW, initH);
    mainBar = TwNewBar("Main Control");
    TwDefine("GLOBAL help='Main control'");
    TwAddVarRW(mainBar, "Texture 1",
            TW_TYPE_CSSTRING(TEXTURE_FILENAME_SIZE),
            controlStructure->_textureFile[0], "");


    while(!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        TwDraw();
        glfwSwapBuffers(window);
        glfwPollEvents();

        printf("Texture 1 : %s\n", controlStructure->_textureFile[0]);
    }

    TwTerminate();
    glfwTerminate();

    return 0;
}
