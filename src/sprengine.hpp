#ifndef _DUMB_FW_SPRENGINE_
#define _DUMB_FW_SPRENGINE_

#include <sprite.hpp>

namespace Sprite {

/**
 * Sprite instance identifier.
 * Negative values are for invalid identifiers.
 */
typedef int Identifier;

/**
 * Lookup table item.
 */
typedef struct {
  /**
   * Previous item (free or used).
   */
  int _previous;
  /**
   * Next item (free or used).
   */
  int _next;
  /**
   * Targeted sprite slot.
   */
  Identifier _target;
  /**
   * Helper flag for quick status retrieval.
   */
  bool _free;
} LookupItem;

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
   * Cycle flag.
   */
  bool _cycle;

  /**
   * Still flag. Useful to quickly manage animation with only one frame.
   */
  bool _still;
} Instance;

/**
 * Cell to copy into 'V-Ram'.
 */
typedef struct {
  /**
   * Position.
   */
  GLfloat _posX;
  GLfloat _posY;

  /**
   * Offset.
   */
  GLfloat _offsetX;
  GLfloat _offsetY;

  /**
   * Size.
   */
  GLfloat _sizeX;
  GLfloat _sizeY;

  /**
   * Top texel coordinate.
   */
  GLfloat _topU;
  GLfloat _topV;

  /**
   * Bottom texel coordinate.
   */
  GLfloat _bottomU;
  GLfloat _bottomV;
} Cell;

/**
 * The Sprite Engine.
 * \o_ VBO and Geometry Shader Extravaganza _o/
 * The engine is NOT thread safe. Multi-thread safety must
 * be ensured outside the engine as it depends on user strategy.
 */
class Engine {
public:
   /**
    * Constructor.
    * @param atlas Backed atlas.
    * @param capacity Maximum number of manageable sprites.
    */
   Engine(Atlas *atlas, unsigned int capacity);

   /**
    * Destructor.
    */
   ~Engine();

   /**
    * Create a new sprite instance.
    * @param definitionId Sprite definition identifier.
    * @patam pos Initial position.
    * @param firstAnim Animation to launch.
    * @param cycle if <code>true</code>, cycle the first animation.
    * @return An identifier to sprite instance or a negative value in case of
    * invalid parameters.
    */
   Identifier create(unsigned int definitionId, glm::vec2 pos, unsigned int firstAnim = 0, bool cycle=true);

   /**
    * Destroy/Remove a sprite instance for the engine.
    * @param id Identifier of the instance to remove.
    */
   void destroy(Identifier id);

   /**
    * Set the sprite current animation.
    * @param Id Sprite identifier.
    * @param pos Position.
    * @param animId Animation identifier.
    * @param cycle Does the animation need to cycle ?
    * @param progress Time progression in current animation.
    * @return <code>false</code> in case of invalid parameters.
    */
   bool set(Identifier id, glm::vec2 pos, unsigned int animId, bool cycle=true, double progress=0);

   /**
    * Move a sprite to the specified position.
    * @param pos Position in pixel.
    */
   void move(Identifier id, glm::vec2 pos);

   /**
    * Set engine drawing viewport.
    * @param x Center origin coordinate on X axis in pixel.
    * @param y Center origin coordinate on Y axis in pixel.
    * @param width Width in pixel.
    * @param height Height in pixel.
    */
   void viewport(int x, int y, unsigned int width, unsigned int height);

   // TODO Zoom and zoom with sprite size preservation.
   // TODO Viewport rotation.

   /**
    * Render the sprites.
    */
   void render();

private:
  /**
   * Debug : Display lookup table.
   */
  void displayTable();

private:

  /**
   * Associated atlas.
   */
  Atlas *_atlas;

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
  double _time;

  /**
   * Elapsed time since first call.
   */
  double _elapsed;

  /**
   * Number of manageable sprite instance.
   */
  unsigned int _capacity;

  /**
   * Number of managed sprites.
   */
  unsigned int _count;

  /**
   * Sprites instances.
   */
  Instance *_instance;

  /**
   * Sprites data.
   */
  Cell *_cell;

  /**
   * Lookup table.
   */
  LookupItem *_table;

  /**
   * Pointer to first used slot.
   */
  int _used;

  /**
   * Pointer to first free slot.
   */
  int _free;

  /**
   * Pointer to lastly allocated slot.
   */
  int _last;
};

}
#endif
