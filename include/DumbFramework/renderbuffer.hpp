#ifndef _DUMB_FW_RENDERBUFFER_
#define _DUMB_FW_RENDERBUFFER_

#include <glm/glm.hpp>
#include <DumbFramework/texture.hpp>

namespace Framework {

/**
 * Renderbuffer.
 * @ingroup DUMB_FW_TEXTURE
 */
class Renderbuffer
{
    public:
        /** Constructor. **/
        Renderbuffer();
        /**Destructor. **/
        ~Renderbuffer();
        /** 
         * Create renderbuffer.
         * @param [in] size    Renderbuffer size.
         * @param [in] format  Renderbuffer format.
         * @param [in] samples Samples count (default=-0).
         * @return true if the renderbufer was succesfully created.
         */
        bool create(const glm::ivec2& size, Texture::PixelFormat format, size_t samples=0);
        /**
         * Destroy renderbuffer.
         */
        void destroy();
        /**
         * Tell if the renderbuffer is valid.
         * @return true if the renderbuffer is valid.
         */
        bool isValid() const;
        /**
         * Bind renderbuffer.
         */
        void bind() const;
        /**
         * Unbind renderbuffer.
         */
        void unbind() const;
        /**
         * Check if the renderbuffer is bound.
         * @return true if the renderbuffer is bound.
         */
        bool isBound() const;
        /**
         * Unbind currently bound renderbuffers.
         */
        static void unbindAll();
        /**
         * Get renderbuffer width and height
         * @return renderbuffer width and height as a glm::ivec2
         */
        const glm::ivec2& size() const;
        /**
         * Get renderbuffer sample count.
         * @return sample count.
         */
         size_t samples() const;
        /**
         * Get renderbuffer pixel format.
         * @return pixel format.
         */
        const Texture::PixelFormat& pixelFormat() const;
        /**
         * Get renderbuffer id.
         * @return renderbuffer id.
         */
        GLuint id() const;
    private:
        glm::ivec2 _size;             /**< Renderbuffer width and height. **/
        size_t     _samples;          /**< Renderbuffer sample count.     **/
        Texture::PixelFormat _format; /**< Pixel format.                  **/
        GLuint _id;                   /**< Resource id.                   **/
};

} // Framework

#endif /* _DUMB_FW_RENDERBUFFER_ */
