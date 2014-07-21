#ifndef _DUMB_FW_TEXTURE_2D_
#define _DUMB_FW_TEXTURE_2D_

#include <glm/glm.hpp>
#include <texture.hpp>

namespace Framework {

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
         * @param [in] layers Number of texture layer (default=1).
         * @return true if the texture was succesfully created.
         */
        bool create(const glm::ivec2& size, Texture::PixelFormat format, int layers=1);
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
        void setData(void* data, int layer=-1);
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
