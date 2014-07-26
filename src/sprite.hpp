#ifndef _DUMB_FW_SPRITE_
#define _DUMB_FW_SPRITE_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>

#include <program.hpp>
#include <texture2d.hpp>

namespace Sprite {

/**
 * An animation frame consists in a texture quad coordinate, an offset,
 * a size and a timestamp.
 * It is a non-mutable object.
 */
struct Frame
{
        double       time;    /**< Time of appearance in seconds. **/
        glm::ivec2   offset;  /**< Position offset. **/
        glm::ivec2   size;    /**< Size. **/
        glm::dvec2   top;     /**< Texture coordinate top (upper left). **/
        glm::dvec2   bottom;  /**< Texture coordinate bottom (lower right). **/
        unsigned int layer;   /**< Texture layer in the texture array. **/
};

/**
 * An animation consists in an ordered list of frame.
 */
class Animation
{
    public:
        /** Default constructor. **/
        Animation();
        /** Destructor. **/
        ~Animation();
        
        /**
         * Create animation.
         * @param [in] id    Animation identifier.
         * @param [in] count Frame count (optional) (default=0).
         */
        void create(unsigned int id, size_t count=0);
        
        /**
         * Get animation id.
         * @return Animation identifier.
         */
        unsigned int id() const;
        
        /**
         * Add frame to animation.
         * Frames are sorted by increasing time.
         * @param [in] frame  Frame to be added.
         */
        void add(Frame const& frame);
        
        /**
         * Get frame count.
         * @return Number of frames in animation.
         */
        size_t frameCount() const;
        
        /**
         * Retrieve a given frame.
         * @param [in] index  Frame number.
         * @return A const pointer to the specified frame or NULL if the
         *         index is out of bound.
         */
        Frame const* getFrame(size_t offset) const;
        
    private:
        unsigned int _id;  /** Animation id.   **/ 
        std::vector<Frame> _frames; /** Frames sorted by increasing time. **/
};

/**
 * Sprite definition.
 * Consist in a list of animation. All materials concerning a sprite
 * must be hosted in the same texture.
 */
class Definition
{
    public:
        /** Default constructor. **/
        Definition();
        /** Destructor. **/
        ~Definition();
        
        /**
         * Create sprite definition.
         * @param [in] id    Identifier.
         * @param [in] count Animation count. 
         */
        void create(unsigned int id, size_t count);
        
        /**
         * Get id.
         * @return Identifier.
         */
        unsigned int id() const;
        
        /**
         * Get animation count.
         * @return Number of stored animations.
         */
        size_t animationCount() const;
        
        /**
         * Retrieve a given animation.
         * @param [in] index  Animation number.
         * @return A const pointer to the specified animation or NULL if the
         *         index is out of bound.
         */
        Animation const* getAnimation(size_t offset) const;
        /**
         * Retrieve a given animation.
         * @param [in] index  Animation number.
         * @warning Unsafe and non const.
         * @return Reference to the specified animation.
         */
        Animation& getAnimation(size_t offset);
        
    private:
        unsigned int _id; /** Sprite definition id. **/
        std::vector<Animation> _animations; /** Animation list. **/
};

/**
 * Sprite Atlas. Maps all definitions to a texture.
 * It is not mutable.
 */
class Atlas
{
    public:
        /** Constructor. **/
        Atlas();
        /** Destructor. **/
        ~Atlas();

        /**
         * Read atlas from a XML file.
         * @param [in] filename  XML filename.
         * @return true if the atlas was succesfully read.
         */
        bool read(std::string const& filename);

        /**
         * Access texture.
         * @return texture object.
         */
        Framework::Texture2D const& texture() const;

        /**
         * Access to definitions.
         * @param index Definition number.
         * @return Definition.
         */
        Definition *get(unsigned int index);

        /**
         * Provide the number of definitions.
         * @return Number of definitions.
         */
        unsigned int count() const;

    private:

        /**
         * Load images from files and create a multiple layered texture out of them.
         * @param [in] filename  Image filenames list. Filenames are separated by a semi-colon.
         * @return true if the texture was succesfully created.
         */
        bool loadTextures(const char *filename);

    private:
        std::vector<Definition> _definitions; /**< Sprite definitions. **/
        Framework::Texture2D _texture; /**< Texture. **/
        glm::ivec2 _size; /**< Atlas size in pixels. **/
};

// []
// -> (position,
//     definition pointer,
//     animation identifier,
//     current frame,
//     current progression time) 

// Each progression step seek to update sprite instance with correct frame.
//

} // Sprite

#endif /* _DUMB_FW_SPRITE_ */
