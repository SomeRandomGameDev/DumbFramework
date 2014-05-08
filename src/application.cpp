#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <application.hpp>

#include <iostream>

Application* Application::_application = 0;

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
    GLFWwindow *window = _hint.getWindow();
    glfwSetKeyCallback(window, 0);
    glfwSetMouseButtonCallback(window, 0);
    glfwSetCursorPosCallback(window, 0);
    glfwSetScrollCallback(window, 0);
    glfwSetWindowSizeCallback(window, 0);
    glfwSetWindowCloseCallback(window, 0);
    _initialized = false;
}

bool Application::start(WindowHint hint, Scene *scene) {
    bool result = ((scene != 0) && glfwInit());
    _scene = scene;
    if((_application != 0) || (scene == 0)) {
        std::cerr << "Concurrent access" << std::endl;
        return false; // (Terribly) Poor protection against concurrent runs.
    }
    _hint = hint;
    _application = this;
    if(result) {
        _initialized = true;
        result = _hint.openWindow();
        if(result) {
            std::cout << "Renderer : " << glGetString(GL_RENDERER) << std::endl;
            std::cout << "Version : " << glGetString(GL_VERSION) << std::endl;
            GLFWwindow* window = _hint.getWindow();
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
            _scene->resume();
            while(_running) {
                next = _scene->output();
                if(next != _scene) {
                    _scene->pause();
                    if(0 == next) {
                        std::cout << "Quitting ..." << std::endl;
                    }
                    _running &= 0 != next;
                    if(_running) {
                        next->resume();
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
    _application = 0;
    return result;
}

void Application::stop() {
    clear();
}

void Application::handleKey(GLFWwindow* /*window*/, int key, int action) {
    _application->_scene->handleKeyAction(key, action);
}

void Application::handleMouseButton(GLFWwindow* /*window*/, int button, int action) {
    _application->_scene->handleMouseButtonAction(button, action);
}

void Application::handleMousePosition(GLFWwindow* /*window*/, double x, double y) {
    _application->_scene->handleMousePositionAction(x, y);
}

void Application::handleMouseWheel(GLFWwindow* /*window*/, double x, double y) {
    _application->_scene->handleMouseWheelAction(x, y);
}

void Application::handleWindowSize(GLFWwindow* /*window*/, int width, int height) {
    _application->_scene->handleWindowSize(width, height);
}

void Application::handleWindowClose(GLFWwindow* /*window*/) {
    _application->_running = false;
    // The other accessor (in the main loop) is additive :
    // Let's consider this as  thread-safe.
}

