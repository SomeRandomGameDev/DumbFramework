#include <windowhint.hpp>
#include <GLFW/glfw3.h>

WindowHint::WindowHint() {
  _window = NULL;
  _width = 640;
  _height = 480;
  _red = _blue = _green = _alpha = 0;
  _stencil = _depth = 0;
  _mode = GLFW_WINDOW;
  _title.assign("DumbFramework");
}

WindowHint::WindowHint(int w, int h, std::string &title) {
  _width = w;
  _height = h;

  _title = title;

  _red = _blue = _green = _alpha = 0;
  _stencil = _depth = 0;
  _mode = GLFW_WINDOW;
}

bool WindowHint::openWindow() {
  _window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL); // [todo] monitor + share
  if(_window == NULL)
  {
	  return false;
  }
  return true;
}
