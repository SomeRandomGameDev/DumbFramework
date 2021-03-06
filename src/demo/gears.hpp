#ifndef _DUMB_FW_DEMO_GEARS_
#define _DUMB_FW_DEMO_GEARS_

#include <DumbFramework/scene.hpp>
#include <GLFW/glfw3.h>

class GearScene : public Scene {
public:
  void handleKeyAction(int key, int scancode, int action, int mods);

  void handleMouseButtonAction(int button, int action);

  void handleMousePositionAction(double x, double y);

  void handleMouseWheelAction(double x, double y);

  void handleWindowSize(int width, int height);

  Scene* output();
 
  void pause();

  void resume(GLFWwindow *);

  GearScene();

  virtual ~GearScene() {}

private:
  GLfloat _viewRotX;
  GLfloat _viewRotY;
  GLfloat _viewRotZ;
  GLfloat _angle;
  GLint _gear1;
  GLint _gear2;
  GLint _gear3;
  bool _quit;
  bool _init;
  int _width;
  int _height;
  int _frame;
  double _time;
  double _start;
};

#endif
