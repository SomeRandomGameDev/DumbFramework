#include <windowhint.hpp>
#include <GL/glfw.h>

WindowHint::WindowHint() {
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
  bool result = glfwOpenWindow(_width, _height, _red,
                               _green, _blue, _alpha,
                               _depth, _stencil, _mode);
  if(result) {
    glfwSetWindowTitle(_title.c_str());
  }
  return result;
}
