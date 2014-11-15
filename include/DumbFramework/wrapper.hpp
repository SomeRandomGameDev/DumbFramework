#ifndef _DUMB_FW_WRAPPER_
#define _DUMB_FW_WRAPPER_

#include <DumbFramework/config.hpp>
#include <DumbFramework/module.hpp>

#ifndef Log_Error
#include <stdio.h>
namespace Framework {
namespace Module {
    int Base = 0;
} // Module
} // Framework

#define Log_Info(module, format, ...) printf("[INFO] <%s:%d in %s> " format "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#define Log_Warning(module, format, ...) fprintf(stderr, "[WARN] <%s:%d in %s> " format "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#define Log_Error(module, format, ...) fprintf(stderr, "[FAIL] <%s:%d in %s> " format "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);

#endif

/**
 * GLFW error callback
 * @param [in] errorCode      Error code.
 * @param [in] description    UTF-8 encoded string describing the error.
 */
static void Log_GLFWErrorCallback(int errorCode, const char* description)
{
     Log_Error(Framework::Module::Base, "GLFW failed with error code %d: %s", errorCode, description);
}

/*
 * The delegated concept shall implement:
 * GLFWwindow *createWindow();
 * void destroyWindow(GLFWwindow *);
 * void init();
 * void render();
 * void handleKeyAction(int key, int scancode, int action, int mods);
 * void handleMouseButtonAction(int button, int action, int mods);
 * void handleMousePositionAction(double x, double y);
 * void handleMouseWheelAction(double x, double y);
 * void handleWindowSize(int x, int y);
 * void handleChar(unsigned int unicodeChar);
 * void handleCursorEnter(int flag);
 */

/**
 * Wrapper for simple GLFW application.
 * @param <T> Delegate class.
 */
template <typename T> class Wrapper {
    public:
        /**
         * Delegate instance.
         */
        T* _delegate;
    public:

        /**
         * Constructor.
         * @param dlg Delegate class.
         */
        Wrapper(T *dlg) : _delegate(dlg) {}

        /**
         * Start the wrapper.
         */
        bool start();
    private:

        /**
         * Wrapper to key event callback.
         */
        static void handleKey(GLFWwindow*, int, int, int, int);

        /**
         * Wrapper to mouse button event callback.
         */
        static void handleMouseButton(GLFWwindow*, int, int, int);

        /**
         * Wrapper to mouse position event callback.
         */
        static void handleMousePosition(GLFWwindow*, double, double);

        /**
         * Wrapper to mouse wheel event callback.
         */
        static void handleMouseWheel(GLFWwindow*, double, double);

        /**
         * Wrapper to window closing event callback.
         */
        static void handleWindowClose(GLFWwindow *);

        /**
         * Wrapper to window resizing event callback.
         */
        static void handleWindowSize(GLFWwindow *, int, int);

        /**
         * Wrapper to Unicode character input.
         */
        static void handleChar(GLFWwindow *, unsigned int);

        /**
         * Wrapper to cursor enter/leave window.
         */
        static void handleCursorEnter(GLFWwindow *, int);

};

// Implementation **********************************************

template <typename T> bool Wrapper<T>::start()
{
    // Initialize glfw and set error callback.
    glfwSetErrorCallback(Log_GLFWErrorCallback);
    bool result = glfwInit();
    if(!result)
    {
        return false;
    }
    // Query delegate to create window.
    GLFWwindow *window = _delegate->createWindow();
    if(0 == window)
    {
        Log_Error(ModuleID::Base, "Failed to create window.");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    // Set window user data.
    glfwSetWindowUserPointer(window, this);

    // Initialize OpenGL extensions.
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if(GLEW_OK != glewError)
    {
        Log_Error(ModuleID::Base, "Failed to initialize OpenGL extensions: %s.", glewGetErrorString(glewError));
        glfwTerminate();
        return false;
    }
    
    // Set window callbacks.
    glfwSetKeyCallback(window, Wrapper<T>::handleKey);
    glfwSetMouseButtonCallback(window, Wrapper<T>::handleMouseButton);
    glfwSetCursorPosCallback(window, Wrapper<T>::handleMousePosition);
    glfwSetScrollCallback(window, Wrapper<T>::handleMouseWheel);
    glfwSetWindowSizeCallback(window, Wrapper<T>::handleWindowSize);
    glfwSetWindowCloseCallback(window, Wrapper<T>::handleWindowClose);
    glfwSetCharCallback(window, Wrapper<T>::handleChar);
    glfwSetCursorEnterCallback(window, Wrapper<T>::handleCursorEnter);

    // Delegate init.
    _delegate->init();
    
    // This is the application loop.
    while(!glfwWindowShouldClose(window))
    {
        _delegate->render();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    // Destroy window.
    _delegate->destroyWindow(window);
    glfwTerminate();
    
    return true;
}

template <typename T> void Wrapper<T>::handleKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleKeyAction(key, scancode, action, mods);
}

template <typename T> void Wrapper<T>::handleMouseButton(GLFWwindow* window, int button, int action, int mods) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleMouseButtonAction(button, action, mods);
}

template <typename T> void Wrapper<T>::handleMousePosition(GLFWwindow* window, double x, double y) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleMousePositionAction(x, y);
}

template <typename T> void Wrapper<T>::handleMouseWheel(GLFWwindow* window, double x, double y) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleMouseWheelAction(x, y);
}

template <typename T> void Wrapper<T>::handleWindowSize(GLFWwindow* window, int width, int height) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleWindowSize(width, height);
}

template <typename T> void Wrapper<T>::handleWindowClose(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GL_TRUE);
}

template <typename T> void Wrapper<T>::handleChar(GLFWwindow *window, unsigned int chr) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleChar(chr);
}

template <typename T> void Wrapper<T>::handleCursorEnter(GLFWwindow *window, int flag) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleCursorEnter(flag);
}


#endif
