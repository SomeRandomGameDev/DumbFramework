#ifndef _DUMB_FW_WHINT_
#define _DUMB_FW_WHINT_

#include <string>


class WindowHint {
public:

  /**
   * Default constructor.
   * Fills hint with default values.
   */
  WindowHint();

  /**
   * Minimal constructor.
   * @param w Width.
   * @param h Height.
   * @param title Window title.
   */
  WindowHint(int w, int h, std::string &title);

  /**
   * Retrieve desired width.
   * @return Width in pixels.
   */
  inline int getWidth() { return _width; }

  /**
   * Retrieve desired height.
   * @return Height in pixels.
   */
  inline int getHeight() { return _height; }

  /**
   * Retrieve red channel depth.
   * @return Red channel depth in bit.
   */
  inline int getRed() { return _red; }

  /**
   * Retrieve blue channel depth.
   * @return Blue channel depth in bit.
   */
  inline int getBlue() { return _blue; }

  /**
   * Retrieve green channel depth.
   * @return Green channel depth in bit.
   */
  inline int getGreen() { return _green; }

  /**
   * Retrieve alpha channel depth.
   * @return Alpha channel depth in bit.
   */
  inline int getAlpha() { return _alpha; }

  /**
   * Retrieve depth buffer size.
   * @return Depth buffer size in bit.
   */
  inline int getDepth() { return _depth; }

  /**
   * Retrieve stencil buffer depth.
   * @return Stencil buffer depth in bit.
   */
  inline int getStencil() { return _stencil; }

  /**
   * Retrieve window mode.
   * @return Window mode (as described in GLFW documentation).
   */
  inline int getMode() { return _mode; }

  /**
   * Retrieve window title.
   * @return Window title.
   */
  inline std::string getTitle() { return _title; }

  /**
   * Open a window with the current settings.
   * @return <code>true</code> if the operation succeeded.
   */
  bool openWindow();

private:

  /**
   * Desired width in pixels.
   */
  int _width;

  /**
   * Desired height in pixels.
   */
  int _height;

  /**
   * Red channel size in bits.
   */
  int _red;

  /**
   * Blue channel size in bits.
   */
  int _blue;

  /**
   * Green channel size in bits.
   */
  int _green;

  /**
   * Alpha channel size in bits.
   */
  int _alpha;

  /**
   * Depth buffer size in bits.
   */
  int _depth;

  /**
   * Stencil buffer size in bits.
   */
  int _stencil;

  /**
   * Window mode (see GLFW documentation).
   */
  int _mode;

  /**
   * Window title.
   */
  std::string _title;
};

#endif
