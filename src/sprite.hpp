#ifndef _DUMB_FW_SPRITE_
#define _DUMB_FW_SPRITE_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <xml.hpp>

#include <container.hpp>
#include <program.hpp>

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
         * @param ind Texture index.
         */
        Frame(double time,
              const glm::ivec2& offset, const glm::ivec2& size,
              const glm::dvec2& top, const glm::dvec2& bottom,
              unsigned int ind);

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
        inline const glm::ivec2& getOffset() const { return _offset; }

        /**
         * Provides the frame size.
         * @return Frame size in pixel (width and height).
         */
        inline const glm::ivec2& getSize() const { return _size; }

        /**
         * Provides upper left texture coordinates.
         * @return Texture coordinates (u and v).
         */
        inline const glm::dvec2& getTop() const { return _top; }

        /**
         * Provides lower right texture coordinates.
         * @return Texture coordinates (u and v).
         */
        inline const glm::dvec2& getBottom() const { return _bottom; }

        /**
         * Provides texture index.
         * @return Texture index in the texture array.
         */
        inline GLuint getTexture() const { return static_cast<GLuint>(_texture); }

    private:
        double       _time;    /**< Time of appearance in seconds. **/
        glm::ivec2   _offset;  /**< Position offset. **/
        glm::ivec2   _size;    /**< Size. **/
        glm::dvec2   _top;     /**< Texture coordinate top (upper left). **/
        glm::dvec2   _bottom;  /**< Texture coordinate bottom (lower right). **/
        unsigned int _texture; /**< Texture layer in the texture array. **/
};

/**
 * An animation consists in an ordered list of frame. It is not mutable.
 */
typedef Framework::Container<Frame *> Animation;

/**
 * Sprite definition.
 * Consist in a list of animation. All materials concerning a sprite
 * must be hosted in the same texture.
 * It is not mutable.
 */
typedef Framework::Container<Animation *> Definition;

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
        GLuint texture() const;

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

    protected:

        void startElement(const XML_Char *, const XML_Char **);
        void endElement(const XML_Char *);

    private:

        /**
         * Load textures into a texture array.
         * @param filename Paths to the textures. It is filenames separated by a semi-colon.
         */
        void loadTextures(const char *filename);

        /**
         * Flush/Free the memory occupied by previously loaded textures,
         * display an error message and set the parser to FAULTED.
         * @param msg Error message to display.
         * @param cnt Number of textures to free.
         * @param textures Array of textures to free.
         */
        void setFaulted(const char *msg,
            unsigned int cnt, unsigned char **textures);

    private:
        Framework::Container<Definition *> *_definitions; /**< Definitions. **/
        
        GLuint _texture; /**< Identifier to texture. **/
        
        int _state; /**< State (Parsing, ready, etc...). **/
        GLint _width;  /**< Atlas width in pixels.  **/
        GLint _height; /**< Atlas height in pixels. **/

        Definition  *_lastDefinition; /**< Last definition in use. **/
        Animation   *_lastAnimation;  /**< Last animation in use. **/
        unsigned int _lastFrameId;    /**< Last defined frame identifier. **/
};

// []
// -> (position,
//     definition pointer,
//     animation identifier,
//     current frame,
//     current progression time) 

// Each progression step seek to update sprite instance with correct frame.
//

class Entity
{
    public:
        Entity();
        Entity(const Entity& entity);
        ~Entity();

        Entity& operator=(const Entity& entity);

        const glm::vec3& GetPosition() const;
        void SetPosition(const glm::vec3& position);

        const glm::quat& GetOrientation() const;
        void SetOrientation(const glm::quat& orientation);

        // void set/get program

    private:
        glm::vec3         m_position;
        glm::quat         m_orientation;
        Render::Program   m_program;
        // Materials
        // Mesh
        // Bones
};

}

#endif
