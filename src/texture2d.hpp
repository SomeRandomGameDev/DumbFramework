#ifndef _DUMB_FW_TEXTURE_2D_
#define _DUMB_FW_TEXTURE_2D_

#include <glm/glm.hpp>
#include "texture.hpp"

namespace Render  {

/**
 * Texture 2D Wrapper.
 */
class Texture2D
{
    public:
        /** Constructor. **/
        Texture2D();
        /**Destructor. **/
        ~Texture2D();
        /** 
         * Create texture.
         * @param [in] size   Texture size.
         * @param [in] format Texture format.
         */
        bool create(const glm::ivec2& size, Texture::PixelFormat format);
        /** 
         * Create from existing texture (no duplication).
         * @param [in] id   Texture id.
         */
        bool create(GLuint id);
        /**
         * Destroy texture.
         */
        void destroy();
        /**
         * Set texture data.
         * @param [in] data Image buffer.
         */        
        void setData(void* data);
        /**
         * Bind texture.
         */
        void bind();
        /**
         * Set texel magnification filter.
         * @param [in] filter Magnification filter.
         */
        void setMagFilter(Texture::MagFilter filter);
        /**
         * Set texel minification filter.
         * @param [in] filter Minification filter.
         */
        void setMinFilter(Texture::MinFilter filter);
        /**
         * Set texture coordinates wrap mode.
         * @param [in] s Wrap mode for s coordinate (1st).
         * @param [in] t Wrap mode for t coordinate (2nd).
         */
        void setWrap(Texture::Wrap s, Texture::Wrap t);
        /**
         * Set border color.
         * @param [in] color Border color (rgba).
         */
        void setBorderColor(const glm::vec4& color);
        /**
         * Generate mipmap from texture.
         */
        void buildMipmap();
        
    private:
        glm::ivec2 _size;               /**< Texture width and height. **/
        Texture::PixelFormat _format;   /**< Pixel format. **/
        
        GLuint _id;                     /**< Resource id. **/

        /** OpenGL texture informations. **/
        struct OpenGLTextureInfos
        {
            GLint  internalFormat; /**< Internal format. **/
            GLenum format;         /**< Pixel data format. **/
            GLenum type;           /**< Pixel data type. **/
            
            OpenGLTextureInfos(Texture::PixelFormat pixelFormat);
        } _infos;
};

}

#endif /* _DUMB_FW_TEXTURE_2D_ */
