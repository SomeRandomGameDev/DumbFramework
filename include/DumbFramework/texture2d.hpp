#ifndef _DUMB_FW_TEXTURE_2D_
#define _DUMB_FW_TEXTURE_2D_

#include <glm/glm.hpp>
#include <DumbFramework/texture.hpp>

namespace Framework {

/**
 * Texture 2D Wrapper.
 * @ingroup DUMB_FW_TEXTURE
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
         * @param [in] layers Number of texture layer (default=-1).
         *             Any value equal or less than zero will force the creation of a single standard texture.
         * @return true if the texture was succesfully created.
         */
        bool create(const glm::ivec2& size, Texture::PixelFormat format, int layers=-1);
        /**
         * Destroy texture.
         */
        void destroy();
        /**
         * Tell if the texture is valid.
         * @return true if the texture is valid.
         */
        bool isValid() const;
        /**
         * Set texture data.
         * @param [in] data  Image buffer.
         * @param [in] layer Texture layer in which the data will be copied (default=-1).
         *                   Initialize all possible layers if the layer value is negative.
         */        
        bool setData(void* data, int layer=-1);
        /**
         * Bind texture.
         */
        void bind() const;
        /**
         * Unbind texture.
         */
        void unbind() const;
        /**
         * Check if the texture is bound.
         * @return true if the texture is bound.
         */
        bool isBound() const;
        /**
         * Unbind currently bound textures.
         */
        static void unbindAll();
        /**
         * Set texel magnification filter.
         * @param [in] filter Magnification filter.
         * @note Texture must be bound before calling this method.
         */
        void setMagFilter(Texture::MagFilter filter);
        /**
         * Set texel minification filter.
         * @param [in] filter Minification filter.
         * @note Texture must be bound before calling this method.
         */
        void setMinFilter(Texture::MinFilter filter);
        /**
         * Set texture coordinates wrap mode.
         * @param [in] s Wrap mode for s coordinate (1st).
         * @param [in] t Wrap mode for t coordinate (2nd).
         * @note Texture must be bound before calling this method.
         */
        void setWrap(Texture::Wrap s, Texture::Wrap t);
        /**
         * Set border color.
         * @param [in] color Border color (rgba).
         * @note Texture must be bound before calling this method.
         */
        void setBorderColor(const glm::vec4& color);
        /**
         * Generate mipmap from texture.
         */
        void buildMipmap();
        /**
         * Get texture width and height
         * @return texture width and height as a glm::ivec2
         */
        const glm::ivec2& size() const;
        /**
         * Get texture pixel format.
         * @return pixel format.
         */
        const Texture::PixelFormat& pixelFormat() const;
        /**
         * Get layer count.
         * @return layer count.
         */
        int layerCount() const;
        
    private:
        glm::ivec2 _size;               /**< Texture width and height. **/
        Texture::PixelFormat _format;   /**< Pixel format.             **/
        GLuint _id;                     /**< Resource id.              **/
        GLenum _target;                 /**< Texture binding target.   **/
        int    _layers;                 /**< Number of layers.         **/
        
        /** OpenGL texture informations. **/
        struct OpenGLTextureInfos
        {
            GLint  internalFormat; /**< Internal format.   **/
            GLenum format;         /**< Pixel data format. **/
            GLenum type;           /**< Pixel data type.   **/
            
            OpenGLTextureInfos(Texture::PixelFormat pixelFormat);
        } _infos;
};

} // Framework

#endif /* _DUMB_FW_TEXTURE_2D_ */
