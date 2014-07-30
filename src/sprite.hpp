#ifndef _DUMB_FW_SPRITE_
#define _DUMB_FW_SPRITE_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>
#include <unordered_map>

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
    friend class XMLAnimationReader;
    
    public:
        /** Default constructor. **/
        Animation();
        /** Destructor. **/
        ~Animation();
        /**
         * Create animation.
         * @param [in] name  Animation name.
         */
        void create(std::string const& name);
        /**
         * Get animation name.
         * @return Animation name.
         */
        std::string const& name() const;
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
        std::string        _name;   /**< Animation name. **/
        std::vector<Frame> _frames; /**< Frames sorted by increasing time. **/
};

/**
 * Sprite definition.
 * Consist in a list of animation. All materials concerning a sprite
 * must be hosted in the same texture.
 */
class Definition
{
    friend class XMLDefinitionReader;
    public:
        /** Default constructor. **/
        Definition();
        /** Destructor. **/
        ~Definition();
        /**
         * Create sprite definition.
         * @param [in] name  Identifier.
         * @param [in] count Animation count. 
         */
        void create(std::string const& name, size_t count);
        /**
         * Get name.
         * @return name.
         */
        std::string const& name() const;
        /**
         * Get animation count.
         * @return Number of stored animations.
         */
        size_t animationCount() const;
        
        /**
         * Retrieve a given animation by its offset.
         * @param [in] index  Animation number.
         * @return A const pointer to the specified animation or NULL if the
         *         index is out of bound.
         */
        Animation const* getAnimation(size_t offset) const;
        /**
         * Retrieve a given animation by its offset.
         * @param [in] index  Animation number.
         * @return A pointer to the specified animation or NULL if the
         *         index is out of bound.
         */
        Animation* getAnimation(size_t offset);
        /**
         * Retrieve a given animation by its name.
         * @param [in] name  Animation name.
         * @return A const pointer to the animation with the corresponding
         *         name, or NULL if no matching animation was found.
         */
        Animation const* getAnimation(std::string const& name) const;
        /**
         * Retrieve a given animation by its name.
         * @param [in] name  Animation name.
         * @return A pointer to the animation with the corresponding
         *         name, or NULL if no matching animation was found.
         */
        Animation* getAnimation(std::string const& name);
        
    private:
        /** Sprite definition name. **/
        std::string _name;
        /** Animation list. **/
        std::vector<Animation> _animations;
        /** 
         * Animation dictionary. 
         * For a given name we have its index in the animation storage.
         */
        std::unordered_map<std::string, unsigned int > _dict;
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
         * @brief Access to definitions.
         * Return a pointer to the definition stored at @a index, or
         * NULL if the index is out of storage bound.
         * @param index Definition number.
         * @return Definition pointer or NULL if the index is out of
         *         storage bound..
         */
        Definition *get(unsigned int index);
        /**
         * @brief Access to definitions (const version).
         * Return a pointer to the definition stored at @a index, or
         * NULL if the index is out of storage bound.
         * @param index Definition number.
         * @return Definition pointer or NULL if the index is out of
         *         storage bound..
         */
        Definition const* get(unsigned int index) const;
        /**
         * Retrieve a given definition by its name.
         * @param [in] name  Definition name.
         * @return A pointer to the definition with the corresponding
         *         name, or NULL if no matching definition was found.
         */
        Definition* get(std::string const& name);
        /**
         * Retrieve a given definition by its name.
         * @param [in] name  Definition name.
         * @return A const pointer to the definition with the corresponding
         *         name, or NULL if no matching definition was found.
         */
        Definition const* get(std::string const& name) const;

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
        /** Sprite definitions. **/
        std::vector<Definition> _definitions;
        /** 
         * Sprite definition dictionary. 
         * For a given name we have its index in the sprite definition storage.
         */
        std::unordered_map<std::string, unsigned int > _dict;
        /** Texture. **/
        Framework::Texture2D _texture;
        /** Atlas size in pixels. **/
        glm::ivec2 _size;
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
