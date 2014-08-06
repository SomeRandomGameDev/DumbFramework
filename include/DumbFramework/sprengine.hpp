#ifndef _DUMB_FW_SPRENGINE_
#define _DUMB_FW_SPRENGINE_

#include <DumbFramework/sprite.hpp>
#include <DumbFramework/vertexbuffer.hpp>
#include <DumbFramework/program.hpp>

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
   * Layer information.
   */
  unsigned int _layer;
  /**
   * Reserve identification in lookup table.
   */
  unsigned int _reverse;
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

  /**
   * Rotation angle.
   */
  GLfloat _angle;

  /**
   * Sprite scale.
   */
  GLfloat _scale;

  /**
   * Texture layer in which the sprite actually resides in the array.
   */
  GLuint _layer;
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
    * @param angle Initial sprite rotation angle.
    * @param scale Initial sprite scale factor.
    * @param layer Sprite layer.
    * @return An identifier to sprite instance or a negative value in case of
    * invalid parameters.
    */
   Identifier create(unsigned int definitionId, glm::vec2 pos, unsigned int firstAnim = 0,
                     bool cycle=true, float angle=0.0f, float scale=1.0f, unsigned int layer=0.0f);

   /**
    * Destroy/Remove a sprite instance for the engine.
    * @param id Identifier of the instance to remove.
    */
   void destroy(Identifier id);

   /**
    * Copy a sprite state into another.
    * @param destination Destination sprite slot.
    * @param source Source sprite slot.
    */
   void copy(Identifier destination, Identifier source);

   /**
    * Create a copy/clone of an existing sprite instance.
    * @param source Source sprite instance.
    * @return An identifier is the operation is legit.
    */
   Identifier clone(Identifier source);

   /**
    * Set the sprite current animation.
    * @param Id Sprite identifier.
    * @param pos Position.
    * @param animId Animation identifier.
    * @param cycle Does the animation need to cycle ?
    * @param progress Time progression in current animation.
    * @param angle Sprite rotation angle.
    * @return <code>false</code> in case of invalid parameters.
    */
   bool set(Identifier id, glm::vec2 pos, unsigned int animId, bool cycle=true,
            double progress=0, float angle=0.0f, float scale=1.0f, unsigned int layer=0.0f);

   /**
    * Move a sprite to the specified position.
    * @param pos Position in pixel.
    */
   void move(Identifier id, glm::vec2 pos);

   /**
    * Rotate a sprite to the specified angle.
    * @param id Sprite instance identifier.
    * @param angle Angle.
    */
   void rotate(Identifier id, float angle);

   /**
    * Set the sprite layer.
    * @param id Sprite instance identifier.
    * @param layer Sprite depth.
    */
   void setLayer(Identifier id, unsigned int layer);

   /**
    * Apply a scale factor to the specified sprite instance.
    * @param id Sprite instance identifier.
    * @param scale Scaling ratio.
    */
   void scale(Identifier id, float scale);

   /**
    * Set engine drawing viewport.
    * @param x Center origin coordinate on X axis in pixel.
    * @param y Center origin coordinate on Y axis in pixel.
    * @param width Width in pixel.
    * @param height Height in pixel.
    * @param scale Scale.
    */
   void viewport(float x, float y,
                 unsigned int width, unsigned int height,
                 float scale = 1.0f);

   /**
    * Translate the viewport given a logical displacement.
    * @param relX Displacement in world coordinate (X coordinate).
    * @param relY Displacement in world coordinate (Y coordinate).
    */
   void translate(float relX, float relY);

   /**
    * Translate the viewport given a pixel displacement.
    * @param relX Number of pixel in the X axis.
    * @param relY Number of pixel in the Y axis.
    */
   void translate(int relX, int relY);

   /**
    * Adjust the viewport to the specified zooming scale at the
    * targetd position.
    * @param x Zoom center of interest (in pixels).
    * @param y Zoom center of interest (in pixels).
    * @param scale Scaling factor.
    */
   void zoom(int x, int y, float scale);

   /**
    * Render the sprites.
    */
   void render();

private:
  /**
   * Debug : Display lookup table.
   */
  void displayTable();

  /**
   * Copy frame content to a cell.
   * @param frame Source frame.
   * @param cell Destination cell.
   * @param x Display position of the frame (on x axis).
   * @param y Display position of the frame (on y axis)
   * @param angle Sprite angle.
   * @param scale Sprite scaling factor.
   * @param layer Sprite layer.
   */
  inline void assignFrameToCell(Frame const* frame, Cell *cell,
                 double x, double y, float angle, float scale);

  /**
   * Animate sprites.
   */
  void animate();

  /**
   * Swap instances.
   * @param id Look-up table identifier.
   * @param a First instance.
   * @param b Second instance.
   */
  void swapInstances(Identifier id, Identifier a, Identifier b);

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
   * Vertex Buffer Object.
   */
  Framework::VertexBuffer _buffer;

  /**
   * GLSL Program identifier.
   */
  Framework::Program _program;

  /**
   * Sprite Atlas texture.
   */
  Framework::Texture2D const* _texture;

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

  /**
   * Center (X-coordinate).
   */
  float _centerX;

  /**
   * Center (y-coordinate).
   */
  float _centerY;

  /**
   * Scale.
   */
  float _scale;

  /**
   * Viewport width.
   */
  float _width;

  /**
   * Viewport height.
   */
  float _height;
};

}
#endif
