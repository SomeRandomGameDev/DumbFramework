#ifndef _DUMB_FW_SPRITE_
#define _DUMB_FW_SPRITE_

#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <xml.hpp>

namespace Sprite {

/**
 * An animation frame consists in a texture quad coordinate, an offset,
 * a size and a timestamp.
 * It is a non-mutable object.
 */
class Frame {
public:
  /**
   * Constructor.
   * @param time Time of appearance in seconds.
   * @param offset Offset in pixels.
   * @param size Size in pixels.
   * @param top Top texture coordinates.
   * @param bottom Bottom texture coordinates.
   */
  Frame(double time,
        glm::ivec2 offset,
        glm::ivec2 size,
        glm::dvec2 top,
        glm::dvec2 bottom) : _time(time),
                            _offset(offset),
                            _size(size),
                            _top(top),
                            _bottom(bottom) {}

  // Accessors.

  /**
   * Provides appearance time.
   * @return Appearance time in seconds.
   */
  inline double getTime() { return _time; }

  /**
   * Provides frame offset.
   * @return A position in pixel (x and y).
   */
  inline glm::ivec2 getOffset() { return _offset; }

  /**
   * Provides the frame size.
   * @return Frame size in pixel (width and height).
   */
  inline glm::ivec2 getSize() { return _size; }

  /**
   * Provides upper left texture coordinates.
   * @return Texture coordinates (u and v).
   */
  inline glm::dvec2 getTop() { return _top; }

  /**
   * Provides lower right texture coordinates.
   * @return Texture coordinates (u and v).
   */
  inline glm::dvec2 getBottom() { return _bottom; }

private:
  /**
   * Time of appearance in seconds.
   */
  double _time;
  /**
   * Position offset.
   */
  glm::ivec2 _offset;
  /**
   * Size.
   */
  glm::ivec2 _size;
  /**
   * Texture coordinate top (upper left).
   */
  glm::dvec2 _top;
  /**
   * Texture coordinate bottom (lower right).
   */
  glm::dvec2 _bottom;
};

/**
 * Oversimple overridable container.
 */
template <typename T> class Container {
public:

  /**
   * Base constructor.
   */
  Container(unsigned int capacity) {
    _capacity = capacity;
    _data = new T[_capacity];
  }

  /**
   * Destructor.
   */
  virtual ~Container() {
    delete [] _data;
  }

  /**
   * @return Number of storable element.
   */
  unsigned int capacity() {
    return _capacity;
  }

  /**
   * @param index Element position.
   * @return Targeted element or 0 if out of range.
   */
  T *data(unsigned int index) {
    T* result = 0;
    if(index < _capacity) {
      result = _data + index;
    }
    return result;
  }

  /**
   * Unsecure accessor.
   * @param index Element position.
   * @return Copy of the targeted element.
   */
  T get(unsigned int index) {
    return _data[index];
  }

private:
  /**
   * Capacity.
   */
  unsigned int _capacity;

  /**
   * Data.
   */
  T * _data;

};


/**
 * An animation consists in an ordered list of frame. It is not mutable.
 */
typedef Container<Frame *> Animation;

/**
 * Sprite definition.
 * Consist in a list of animation. All materials concerning a sprite
 * must be hosted in the same texture.
 * It is not mutable.
 */
typedef Container<Animation *> Definition;

/**
 * Sprite Atlas. Maps all definitions to a texture.
 * It is not mutable.
 */
class Atlas {

friend class XML::Parser<Atlas>;

public:

  /**
   * Constructor.
   * @param path Path to description file.
   */
  Atlas(const char *path);

  /**
   * Destructor.
   */
  ~Atlas();

  /**
   * Access texture identifier.
   * @return OGL-wise texture identifier.
   */
  GLuint texture() { return _texture; }

  /**
   * Access to definitions.
   * @param index Definition number.
   * @return Definition.
   */
  Definition *get(unsigned int index) {
    return (4 == _state)?_definitions->get(index):0;
  }

  /**
   * Provide the number of definitions.
   * @return Number of definitions.
   */
  unsigned int count() { return _definitions->capacity(); }

protected:

  void startElement(const XML_Char *, const XML_Char **);
  void endElement(const XML_Char *);

private:

  /**
   * Definitions.
   */
  Container<Definition *> *_definitions;

  /**
   * Identifier to texture.
   */
  GLuint _texture;

  /**
   * State (Parsing, ready, etc...).
   */
  int _state;

  /**
   * Atlas width in pixels.
  */
  GLint _width;

  /**
   * Atlas height in pixels.
   */
  GLint _height;

  /**
   * Last definition in use.
   */
  Definition *_lastDefinition;

  /**
   * Last animation in use.
   */
  Animation *_lastAnimation;

  /**
   * Last defined frame identifier.
   */
  unsigned int _lastFrameId;
};

// []
// -> (position,
//     definition pointer,
//     animation identifier,
//     current frame,
//     current progression time) 

// Each progression step seek to update sprite instance with correct frame.
//


}

#endif
