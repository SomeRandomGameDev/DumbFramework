// You'll never see an uglyiest test executable !

#include <GL/glew.h> // Hot fix : There's a big issue in the includes management.
#include <application.hpp>
#include <scene.hpp>
#include <sprengine.hpp>
#include <iostream>

class TestEngine : public Scene {
public:
  TestEngine(int, int);
  Scene *output();

  void handleKeyAction(int, int, int, int);
  void handleWindowSize(int, int);

  void handleMouseButtonAction(int, int);
  void handleMousePositionAction(double, double);

  void handleMouseWheelAction(double, double);

  void resume(GLFWwindow *);
  void pause();

private:
  Sprite::Atlas *_atlas;
  Sprite::Engine *_engine;

  Sprite::Identifier _identifier;

  Sprite::Identifier _evilTwin;

  bool _init;
  bool _quit;

  double _start;
  double _elapsed;

  bool _pressed;

  float _initX;
  float _initY;

  float _lastX;
  float _lastY;

  float _centerX;
  float _centerY;

  int _width;
  int _height;

  float _scale;
  float _evilScale;

  int _lastWheelPosition;

  bool _rightPressed;
  bool _middlePressed;
  float _rotate;
};

#define SCALE_FACTOR 0.2f

TestEngine::TestEngine(int w, int h) : _atlas(0), _engine(0), _centerX(0), _centerY(0),
                           _width(w), _height(h),
                           _scale(1.0), _evilScale(1.0),
                           _lastWheelPosition(0), _rightPressed(false),
                           _rotate(0) { _pressed = _init = _quit = false; }

Scene *TestEngine::output() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  double current = glfwGetTime();
  _elapsed += current - _start;
  _start = current;

  //_engine->move(_identifier, glm::vec2(100, 100 + 50*sin(_elapsed)));
  _engine->render();
  return _quit?0:this;
}

void TestEngine::handleMouseWheelAction(double, double wheel) {
  int diff = wheel - _lastWheelPosition;
  _lastWheelPosition = wheel;
  if(_rightPressed) {
     _rotate += (diff * 5);
     _engine->rotate(_identifier, (_rotate * 3.1415)/180.0f);
  } else {
    float factor = 1;
    if(diff < 0) {
      factor -= SCALE_FACTOR;
    } else if(diff > 0) {
      factor += SCALE_FACTOR;
    }
    if(_pressed) {
      _evilScale *= factor;
    } else {
      _scale *= factor;
    }
    if(_init) {
      if(_pressed) {
        _engine->scale(_evilTwin, _evilScale);
      } else {
        _engine->zoom(_initX, _initY, _scale);
      }
    }
  }
}

void TestEngine::handleMousePositionAction(double x, double y) {
  if(_pressed) {
    _centerX = _lastX + (_initX - x) * _scale;
    _centerY = _lastY + (_initY - y) * _scale;
    if(_init) _engine->viewport(_centerX, _centerY, _width, _height, _scale);
  } else {
    _initX = x;
    _initY = y;
    _lastX = _centerX;
    _lastY = _centerY;
  }
}

void TestEngine::handleMouseButtonAction(int button, int action) {
  _pressed = (GLFW_PRESS == action) && (GLFW_MOUSE_BUTTON_LEFT == button);
  _rightPressed = (GLFW_PRESS == action) && (GLFW_MOUSE_BUTTON_RIGHT == button);
  _middlePressed = (GLFW_PRESS == action) && (GLFW_MOUSE_BUTTON_MIDDLE == button);
}

void TestEngine::handleKeyAction(int key, int /* scancode */, int action, int /* mods */) {
  _quit = (GLFW_PRESS == action) && (GLFW_KEY_ESCAPE == key);

  if((GLFW_PRESS == action) && (GLFW_KEY_SPACE == key)) {
    _engine->copy(_evilTwin, _identifier);
  }

  if((GLFW_PRESS == action) && (GLFW_KEY_RIGHT_CONTROL == key)) {
    Sprite::Identifier fantom = _engine->clone(_evilTwin);
    if(fantom >= 0) {
      _engine->move(fantom, glm::vec2(0, 0));
    }
  }
}

void TestEngine::handleWindowSize(int width, int height) {
  _width = width;
  _height = height;
  if(0 == _atlas) {
    _atlas = new Sprite::Atlas("test.xml");
    _engine = new Sprite::Engine(_atlas, 8);
    _init = true;
  }
  _engine->viewport(_centerX, _centerY, _width, _height);
  glViewport(0, 0, _width, _height);
}

void TestEngine::resume(GLFWwindow * /* window */) {
  if(0 == _atlas) {
    _atlas = new Sprite::Atlas("test.xml");
    _engine = new Sprite::Engine(_atlas, 8);
    _init = true;
  }
  _engine->viewport(_centerX, _centerY, _width, _height);
  glViewport(0, 0, _width, _height);

  _identifier = _engine->create(0, glm::vec2(100, 100), 0);
  _evilTwin = _engine->create(0, glm::vec2(-100, 0), 0);
  _start = glfwGetTime();
  _elapsed = 0;
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_ALPHA_TEST);
}

void TestEngine::pause() {
  _engine->destroy(_identifier);
  _engine->destroy(_evilTwin);
  delete _engine;
  delete _atlas;

  _engine = 0;
  _atlas = 0;
}

int main(void) {
  std::string title("Sprite EngineTest");
  TestEngine testScene(640, 480);
  WindowHint hint(640, 480, title);
  Application application;

  application.start(hint, &testScene);

  std::cout << "End ..." << std::endl;
  return 0;
}
