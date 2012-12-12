#include <application.hpp>
#include <iostream>
#include <string.h>
#include <GL/glfw.h>

class MyScene : public Scene {
public:

  void handleKeyAction(int key, int action);
  void handleMouseButtonAction(int button, int action);
  void handleMousePositionAction(int x, int y);
  void handleMouseWheelAction(int pos);
  void handleWindowSize(int width, int height);
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

void MyScene::handleWindowSize(int width, int height) {
  std::cout << "Window Resized to " << width << " x " << height << std::endl;
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
  std::string title("A simple example");
  WindowHint hint(800, 600, title);
  Application application;
  MyScene myScene;

  application.start(hint, &myScene);

  return 0;
}
