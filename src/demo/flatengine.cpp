#include <application.hpp>
#include <scene.hpp>
#include <sprengine.hpp>
#include <iostream>

class TestEngine : public Scene {
public:
  TestEngine();
  Scene *output();

  void handleKeyAction(int, int);


  void resume();
  void pause();

private:
  bool _quit;
};

TestEngine::TestEngine() { _quit = false; }

Scene *TestEngine::output() { return _quit?0:this; }

void TestEngine::handleKeyAction(int, int) { _quit = true; }

void TestEngine::resume() {
  Sprite::Atlas atlas("test.xml");

  Sprite::Engine engine(&atlas, 8);

  Sprite::Identifier id = engine.create(0, glm::vec2(0, 0), 0);
  Sprite::Identifier other = engine.create(0, glm::vec2(0,0),0);
  Sprite::Identifier lastOne = engine.create(0, glm::vec2(0,0), 0);

  engine.destroy(other);

  engine.destroy(id);

  other = engine.create(0, glm::vec2(0,0), 0);

  engine.destroy(lastOne);

  lastOne = engine.create(0, glm::vec2(0,0), 0);
}

void TestEngine::pause() {
}

int main(void) {
  std::string title("Sprite EngineTest");
  TestEngine testScene;
  WindowHint hint(640, 480, title);
  Application application;

  application.start(hint, &testScene);

  return 0;
}
