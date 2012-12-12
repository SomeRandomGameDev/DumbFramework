#include <application.hpp>
#include <GL/glfw.h>

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
  glfwSetKeyCallback(0);
  glfwSetMouseButtonCallback(0);
  glfwSetMousePosCallback(0);
  glfwSetMouseWheelCallback(0);
  glfwSetWindowSizeCallback(0);
  glfwSetWindowCloseCallback(0);
  _application = 0; // THIS is not thread safe.
  _initialized = false;
}

bool Application::start(WindowHint hint, Scene *scene) {
  bool result = ((scene != 0) && glfwInit());
  _scene = scene;
  if((_application != 0) || (scene == 0)) {
    return false; // (Terribly) Poor protection against concurrent runs.
  }
  _application = this;
  if(result) {
    _initialized = true;
    result = hint.openWindow();
    if(result) {
      glfwSetKeyCallback(Application::handleKey);
      glfwSetMouseButtonCallback(Application::handleMouseButton);
      glfwSetMousePosCallback(Application::handleMousePosition);
      glfwSetMouseWheelCallback(Application::handleMouseWheel);
      glfwSetWindowSizeCallback(Application::handleWindowSize);
      glfwSetWindowCloseCallback(Application::handleWindowClose);
      _running = true;
      Scene *next = 0;
      _scene->resume();
      while(_running) {
         next = _scene->output();
         if(next != _scene) {
           _scene->pause();
           _running &= 0 != next;
           if(_running) {
             next->resume();
           }
           _scene = next;
         }
         glfwSwapBuffers();
      }
      clear();
    }
    _initialized = false;
    glfwTerminate();
  }
  _application = 0;
  return result;
}

void Application::stop() {
  clear();
}

void GLFWCALL Application::handleKey(int key, int action) {
  _application->_scene->handleKeyAction(key, action);
}

void GLFWCALL Application::handleMouseButton(int button, int action) {
  _application->_scene->handleMouseButtonAction(button, action);
}

void GLFWCALL Application::handleMousePosition(int x, int y) {
  _application->_scene->handleMousePositionAction(x, y);
}

void GLFWCALL Application::handleMouseWheel(int pos) {
  _application->_scene->handleMouseWheelAction(pos);
}

void GLFWCALL Application::handleWindowSize(int width, int height) {
  _application->_scene->handleWindowSize(width, height);
}

int GLFWCALL Application::handleWindowClose() {
  _application->_running = false;
                    // The other accessor (in the main loop) is additive :
                    // Let's consider this as  thread-safe.
  return GL_TRUE;
}

