#ifndef _DUMB_FW_APPLICATION_
#define _DUMB_FW_APPLICATION_

#include <scene.hpp>

/**
 * Initialisation structure.
 */
typedef struct {
  // TODO
  int width;
  int height;
  int red;
  int blue;
  int green;
  int alpha;
  int depth;
  int stencil;
  int mode;
} WindowHint;

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
  bool start(WindowHint hint, Scene *scene);

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

private:

  static Application *_application;

  static void handleKey(int, int);

  static void handleMouseButton(int, int);

  static void handleMousePosition(int, int);

  static void handleMouseWheel(int);

  // TODO Window Event
};

#endif
