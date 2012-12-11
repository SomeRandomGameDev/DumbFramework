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
    if(_initialized) {
      _scene->pause();
    }
    _scene = 0;
  }
}

bool Application::start(WindowHint hint, Scene *scene) {
  bool result = ((scene != 0) && glfwInit());
  _scene = scene;
  _application = this; // TODO Mutex
  if(result) {
    _initialized = true;
    result = glfwOpenWindow(hint.width, hint.height, hint.red,
                            hint.green, hint.blue, hint.alpha,
                            hint.depth, hint.stencil, hint.mode);
    glfwSetKeyCallback(Application::handleKey);
    glfwSetMouseButtonCallback(Application::handleMouseButton);
    glfwSetMousePosCallback(Application::handleMousePosition);
    glfwSetMouseWheelCallback(Application::handleMouseWheel);

    if(result) {
      Scene *next = 0;
      _scene->resume();
      while(0 != _scene) {
         next = _scene->output();
         if(next != _scene) {
           _scene->pause();
           if(0 != next) {
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
  return result;
}

void Application::stop() {
  clear();
}

void GLFWCALL Application::handleKey(int key, int action) {
  if((_application != 0) && (_application->_scene != 0)) {
    _application->_scene->handleKeyAction(key, action);
  }
}

void GLFWCALL Application::handleMouseButton(int button, int action) {
  if((_application != 0) && (_application->_scene != 0)) {
    _application->_scene->handleMouseButtonAction(button, action);
  }
}

void GLFWCALL Application::handleMousePosition(int x, int y) {
  if((_application != 0) && (_application->_scene != 0)) {
    _application->_scene->handleMousePositionAction(x, y);
  }
}

void GLFWCALL Application::handleMouseWheel(int pos) {
  if((_application != 0) && (_application->_scene != 0)) {
    _application->_scene->handleMouseWheelAction(pos);
  }
}



