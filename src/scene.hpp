#ifndef _DUMB_FW_SCENE_
#define _DUMB_FW_SCENE_

/**
 * Scene interface.
 */
class Scene {
public:

  /**
   * Handle key event.
   * @param key Key identifier.
   * @param action GLFW_PRESS or GLFW_RELEASE.
   */
  virtual void handleKeyAction(int key, int action) {}

  /**
   * Handle mouse button action.
   * @param button Button identifier : GLFW_MOUSE_BUTTON_n with n from 1 to
   * the maximum number of available button or LEFT/RIGHT/MIDDLE.
   * @param action GLFW_PRESS or GLFW_RELEASE.
   */
  virtual void handleMouseButtonAction(int button, int action) {}

  /**
   * Handle mouse cursor movement.
   * @param x Coordinate of the mouse cursor on the X Axis in the window.
   * @param y Coordinate of the mouse cursor on the Y Axis in the window.
   */
  virtual void handleMousePositionAction(int x, int y) {}

  /**
   * Handle mouse wheel movement.
   * @param pos Absolute position of the wheel.
   */
  virtual void handleMouseWheelAction(int pos) {}

  /**
   * Handle window size change.
   * @param width New window width.
   * @param height New window height.
   */
  virtual void handleWindowSize(int width, int height) {}

  /**
   * Output the scene.
   * @return The new scene to display. If null, the application ends.
   */
  virtual Scene* output() {}

  /**
   * Pause the scene. Occurs if the scene must be
   * changed or initialized.
   */
  virtual void pause() {}

  /**
   * Start/Resume the scene.
   */
  virtual void resume() {}

  /**
   * Destructor.
   */
  virtual ~Scene() {}

private:

};

#endif
