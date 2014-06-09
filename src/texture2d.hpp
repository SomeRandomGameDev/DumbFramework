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
        Texture2D();
        ~Texture2D();
   
        bool create(const glm::ivec2& size, Texture::PixelFormat format);
        void destroy();
        
        void setData(void* data);

        void bind();
        
        void setMagFilter(Texture::MagFilter filter);
        void setMinFilter(Texture::MinFilter filter);
        void setWrap(Texture::Wrap s, Texture::Wrap t);
        void setBorderColor(const glm::vec4& color);

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
