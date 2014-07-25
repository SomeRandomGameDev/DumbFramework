#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <application.hpp>

#include <iostream>

void SimpleErrorCallback(int errorCode, const char *description) {
    std::cerr << "GLFW Error(" << errorCode << ") " << description << std::endl;
}

Application::Application() {
    _scene = 0;
    _initialized = false;
}

Application::~Application() {
    clear();
    if(_initialized) {
        glfwTerminate();
        _initialized = false;
    }
}

void Application::clear() {
    if(0 != _scene) {
        if(_initialized) { // The application could have never been started.
            _scene->pause();
        }
        _scene = 0;
    }
    _initialized = false;
}

bool Application::start(Framework::WindowHint hint, Scene *scene) {
    glfwSetErrorCallback(SimpleErrorCallback);
    bool result = ((scene != 0) && glfwInit());
    _scene = scene;
    if(scene == 0) {
        std::cerr << "Concurrent access" << std::endl;
        return false; // (Terribly) Poor protection against concurrent runs.
    }
    _hint = hint;
    if(result) {
        _initialized = true;
        result = _hint.openWindow();
        if(result) {
            GLFWwindow* window = _hint.getWindow();
            glfwSetWindowUserPointer(window, this);
            GLenum glewError = glewInit();
            if(GLEW_OK != glewError) {
                std::cerr << "Can't initialize GLEW" << std::endl;
                glfwTerminate();
                // TODO : Error Message.
                return false;
            }
            glfwSetKeyCallback(window, Application::handleKey);
            glfwSetMouseButtonCallback(window, Application::handleMouseButton);
            glfwSetCursorPosCallback(window, Application::handleMousePosition);
            glfwSetScrollCallback(window, Application::handleMouseWheel);
            glfwSetWindowSizeCallback(window, Application::handleWindowSize);
            glfwSetWindowCloseCallback(window, Application::handleWindowClose);
            _running = true;
            Scene *next = 0;
            _scene->resume(window);
            while((0 != _scene) && (!glfwWindowShouldClose(window))) {
                next = _scene->output();
                if(next != _scene) {
                    _scene->pause();
                    if(0 == next) {
                        std::cout << "Quitting ..." << std::endl;
                    }
                    _running &= 0 != next;
                    if(_running) {
                        next->resume(window);
                    }
                    _scene = next;
                }
                glfwPollEvents();
                glfwSwapBuffers(window);
            }
            clear();
        } else {
            std::cerr << "Can't open window" << std::endl;
        }
        _initialized = false;
        glfwTerminate();
    } else {
        std::cerr << "Can't initialize GLFW" << std::endl;
    }
    return result;
}

void Application::stop() {
    clear();
}

void Application::handleKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    app->_scene->handleKeyAction(key, scancode, action, mods);
}

void Application::handleMouseButton(GLFWwindow* window, int button, int action, int) {
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    app->_scene->handleMouseButtonAction(button, action);
}

void Application::handleMousePosition(GLFWwindow* window, double x, double y) {
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    app->_scene->handleMousePositionAction(x, y);
}

void Application::handleMouseWheel(GLFWwindow* window, double x, double y) {
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    app->_scene->handleMouseWheelAction(x, y);
}

void Application::handleWindowSize(GLFWwindow* window, int width, int height) {
    Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    app->_scene->handleWindowSize(width, height);
}

void Application::handleWindowClose(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GL_TRUE);
}

