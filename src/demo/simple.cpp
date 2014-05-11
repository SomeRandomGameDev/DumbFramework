#include <application.hpp>
#include <iostream>
#include <string.h>
#include <GLFW/glfw3.h>

class MyScene : public Scene {
public:

  void handleKeyAction(int key, int scancode, int action, int mods);
  void handleMouseButtonAction(int button, int action);
  void handleMousePositionAction(double x, double y);
  void handleMouseWheelAction(double xpos, double ypos);
  void handleWindowSize(int width, int height);
  Scene* output();
  void pause();
  void resume(GLFWwindow *);

private:
  bool quit;
};


void MyScene::handleKeyAction(int key, int scancode, int action, int mods) {
  std::cout << "Key : " << key << ", Action : " << action << std::endl;
  std::cout << "ScanCode : " << scancode << ", Modifiers : " << mods << std::endl;
  quit |= (key == GLFW_KEY_ESCAPE);
}

void MyScene::handleMouseButtonAction(int button, int action) {
  std::cout << "Mouse Button : " << button << ", " << action << std::endl;
}

void MyScene::handleMousePositionAction(double x, double y) {
  std::cout << "Mouse Position : " << x << ", " << y << std::endl;
}

void MyScene::handleMouseWheelAction(double xpos, double ypos) {
  std::cout << "Mouse Wheel : (" << xpos << ", " << ypos << ")" << std::endl;
}

void MyScene::handleWindowSize(int width, int height) {
  std::cout << "Window Resized to " << width << " x " << height << std::endl;
}

void MyScene::resume(GLFWwindow * /* window */) {
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
