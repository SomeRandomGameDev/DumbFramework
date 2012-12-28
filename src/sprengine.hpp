#ifndef _DUMB_FW_SPRENGINE_
#define _DUMB_FW_SPRENGINE_

#include <glm/glm.hpp>
#include <sprite.hpp>

namespace Sprite {

/**
 * Sprite instance description structure.
 */
typedef struct {
  /**
   * Attached definition.
   */
  Definition *_definition;
  /**
   * Animation identifier.
   */
  unsigned int _animation;
  /**
   * Last drawn frame.
   */
  unsigned int _frame;
  /**
   * Elapsed time since animation start.
   */
  double _elapsed;

  /**
   * Position in logic space.
   */
  glm::vec2 _position;

  /**
   * Next sprite instance.
   */
  int _next;
} _Instance;

/**
 * The Sprite Engine.
 * \o_ VBO and Geometry Shader Extravaganza _o/
 */
class Engine {
public:

private:

  /**
   * Vertex Array Object identifier.
   */
  GLuint _vao;

  /**
   * Vertex Buffer Object identifier.
   */
  GLuint _vbo;

  /**
   * Vertex Shader identifier.
   */
  GLuint _vertexShader;

  /**
   * Fragment Shader identifier.
   */
  GLuint _fragmentShader;

  /**
   * Geometry Shader identifier.
   */
  GLuint _geometryShader;

  /**
   * GLSL Program identifier.
   */
  GLuint _program;

  /**
   * Sprite Atlas identifier.
   */
  GLuint _texture;

  /**
   * Sprite Atlas uniform binding.
   */
  GLuint _uniformTexture;

  /**
   * Projection Matrix uniform binding.
   */
  GLuint _uniformMatrix;

  /**
   * Projection Matrix.
   */
  glm::mat4 _matrix;

  /**
   * Timestamp of last drawing call (in seconds).
   */
  double _last;

  /**
   * Elapsed time since first call.
   */
  double _elapsed;

  /**
   * Number of manageable sprite instance.
   */
  unsigned int _capacity;

  /**
   * Sprites instances.
   */
  _Instance *_instances;
 
};

}
#endif
