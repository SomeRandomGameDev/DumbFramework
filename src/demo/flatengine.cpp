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

  void resume();
  void pause();

private:
  Sprite::Atlas *_atlas;
  Sprite::Engine *_engine;

  Sprite::Identifier _identifier;

  bool _quit;

  double _start;
  double _elapsed;
};

TestEngine::TestEngine() : _atlas(0), _engine(0) { _quit = false; }

Scene *TestEngine::output() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  double current = glfwGetTime();
  _elapsed += current - _start;
  _start = current;

  _engine->move(_identifier, glm::vec2(100, 100 + 50*sin(_elapsed)));
  _engine->render();
  return _quit?0:this;
}

void TestEngine::handleKeyAction(int, int) { _quit = true; }

void TestEngine::handleWindowSize(int width, int height) {
  if(0 == _atlas) {
    _atlas = new Sprite::Atlas("test.xml");
    _engine = new Sprite::Engine(_atlas, 8);
  }
  _engine->viewport(0, 0, width, height);
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
