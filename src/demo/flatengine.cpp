#include <application.hpp>
#include <scene.hpp>
#include <sprengine.hpp>
#include <iostream>

class TestEngine : public Scene {
public:
  TestEngine();
  Scene *output();

  void handleKeyAction(int, int);
  void handleWindowSize(int, int);

  void handleMouseButtonAction(int, int);
  void handleMousePositionAction(int, int);

  void handleMouseWheelAction(int);

  void resume();
  void pause();

private:
  Sprite::Atlas *_atlas;
  Sprite::Engine *_engine;

  Sprite::Identifier _identifier;

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

  int _lastWheelPosition;
};

#define SCALE_FACTOR 0.2f

TestEngine::TestEngine() : _atlas(0), _engine(0), _centerX(0), _centerY(0),
                           _scale(1.0), _lastWheelPosition(0) { _pressed = _init = _quit = false; }

Scene *TestEngine::output() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  double current = glfwGetTime();
  _elapsed += current - _start;
  _start = current;

  //_engine->move(_identifier, glm::vec2(100, 100 + 50*sin(_elapsed)));
  _engine->render();
  return _quit?0:this;
}

void TestEngine::handleMouseWheelAction(int wheel) {
  int diff = wheel - _lastWheelPosition;
  _lastWheelPosition = wheel;
  float factor = 1;
  if(diff < 0) {
    factor -= SCALE_FACTOR;
  } else {
    factor += SCALE_FACTOR;
  }
  _scale *= factor;
  if(_init) _engine->viewport(_centerX, _centerY, _width, _height, _scale);
}

void TestEngine::handleMousePositionAction(int x, int y) {
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
}

void TestEngine::handleKeyAction(int, int) { _quit = true; }

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

void TestEngine::resume() {
  _identifier = _engine->create(0, glm::vec2(100, 100), 0);
  _start = glfwGetTime();
  _elapsed = 0;
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
}

void TestEngine::pause() {
  _engine->destroy(_identifier);
  delete _engine;
  delete _atlas;

  _engine = 0;
  _atlas = 0;
}

int main(void) {
  std::string title("Sprite EngineTest");
  TestEngine testScene;
  WindowHint hint(640, 480, title);
  Application application;

  application.start(hint, &testScene);

  std::cout << "End ..." << std::endl;
  return 0;
}
