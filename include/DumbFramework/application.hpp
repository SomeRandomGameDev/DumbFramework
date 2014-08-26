#ifndef _DUMB_FW_APPLICATION_
#define _DUMB_FW_APPLICATION_

#include <DumbFramework/scene.hpp>
#include <DumbFramework/windowhint.hpp>

/**
 * Application base.
 */
class Application {
public:
  /**
   * Constructor.
   */
  Application();

  /**
   * Destructor.
   */
  virtual ~Application();

  /**
   * Start the application.
   * Only one application can be started/run simultaneously.
   * @param hint Window hint.
   * @param scene First scene to display.
   * @return <code>true</code> if the application has been
   * successfully started.
   */
  bool start(Framework::WindowHint hint, Scene *scene);

  /**
   * Close the application.
   */
  void stop();

private:

  /**
   * Clear current scene.
   */
  void clear();

private:
  /**
   * Current scene.
   */
  Scene *_scene;

  /**
   * Initialisation flag.
   */
  bool _initialized;

  /**
   * Running flag.
   */
  bool _running;

  /**
   * Copy of the initial window hints.
   */
  Framework::WindowHint _hint;
private:

  /**
   * Wrapper to key event callback.
   */
  static void handleKey(GLFWwindow*, int, int, int, int);

  /**
   * Wrapper to mouse button event callback.
   */
  static void handleMouseButton(GLFWwindow*, int, int, int);

  /**
   * Wrapper to mouse position event callback.
   */
  static void handleMousePosition(GLFWwindow*, double, double);

  /**
   * Wrapper to mouse wheel event callback.
   */
  static void handleMouseWheel(GLFWwindow*, double, double);

  /**
   * Wrapper to window closing event callback.
   */
  static void handleWindowClose(GLFWwindow *);

  /**
   * Wrapper to window resizing event callback.
   */
  static void handleWindowSize(GLFWwindow *, int, int);

};

#endif
