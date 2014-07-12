#ifndef _DUMB_FW_WRAPPER_
#define _DUMB_FW_WRAPPER_

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
        Wrapper(T *dlg) : delegate(dlg) {}

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

};

// Implementation **********************************************

template <typename T> bool Wrapper<T>::start() {
    bool result = glfwInit();
    if(result) {
        GLFWwindow *window = _delegate->createWindow();
        if(0 == window) {
            // TODO Log Error on window opening.
            glfwTerminate();
            result = false;
        } else {
            glfwSetWindowUserPoint(window, this);
            GLenum glewError = glewInit();
            if(GLEW_OK == glError) {       
                glfwSetKeyCallback(window, Wrapper<T>::handleKey);
                glfwSetMouseButtonCallback(window, Wrapper<T>::handleMouseButton);
                glfwSetCursorPosCallback(window, Wrapper<T>::handleMousePosition);
                glfwSetScrollCallback(window, Wrapper<T>::handleMouseWheel);
                glfwSetWindowSizeCallback(window, Wrapper<T>::handleWindowSize);
                glfwSetWindowCloseCallback(window, Wrapper<T>::handleWindowClose);
                _delegate->init();
                while(!glfwWindowShouldClose(window)) {
                    _delegate->render();
                    glfwPollEvents();
                    glfwSwapBuffers();
                }
                _delegate->destroyWindow(window);
                glfwTerminate();
                result = true;
            } else {
                // TODO Log Error on GLEW init.
                glfwTerminate();
                result = false;
            }
        }
    } else {
        // TODO Log Error for GLFW init.
    }

    return result;
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

#endif
