#include <application.hpp>
#include <iostream>
#include <GL/glfw.h>

class MyScene : public Scene {
public:

  void handleKeyAction(int key, int action);
  void handleMouseButtonAction(int button, int action);
  void handleMousePositionAction(int x, int y);
  void handleMouseWheelAction(int pos);
  Scene* output();
  void pause();
  void resume();

private:
  bool quit;
};


void MyScene::handleKeyAction(int key, int action) {
  std::cout << "Key : " << key << ", " << action << std::endl;
  quit |= (key == GLFW_KEY_ESC);
}

void MyScene::handleMouseButtonAction(int button, int action) {
  std::cout << "Mouse Button : " << button << ", " << action << std::endl;
}

void MyScene::handleMousePositionAction(int x, int y) {
  std::cout << "Mouse Position : " << x << ", " << y << std::endl;
}

void MyScene::handleMouseWheelAction(int pos) {
  std::cout << "Mouse Wheel : " << pos << std::endl;
}

void MyScene::resume() {
  std::cout << "Resume" << std::endl;
  quit = false;
}

void MyScene::pause() {
}

Scene *MyScene::output() {
  return quit?0:this;
}



/**
 * Simple example.
 */
int main(void) {
  WindowHint hint;
  Application application;
  MyScene myScene;

  hint.width = 640;
  hint.height = 480;
  hint.red = hint.blue = hint.green = 0;
  hint.alpha = hint.depth = hint.stencil = 0;
  hint.mode = GLFW_WINDOW;

  application.start(hint, &myScene);

  return 0;
}
