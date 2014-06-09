#include <glm/gtc/type_ptr.hpp>
#include "texture2d.hpp"

namespace Render  {

/** Construct from pixel format. **/
Texture2D::OpenGLTextureInfos::OpenGLTextureInfos(Texture::PixelFormat pixelFormat)
{
    switch(pixelFormat)
    {
        case Texture::PixelFormat::UNKNOWN:
            break;
        case Texture::PixelFormat::RGB_8:
            internalFormat = GL_RGB8;
            format = GL_RGB;
            type = GL_UNSIGNED_BYTE;
            break;
        case Texture::PixelFormat::RGBA_8:
            internalFormat = GL_RGBA8;
            format = GL_RGBA;
            type = GL_UNSIGNED_INT_8_8_8_8_REV;
            break;
        case Texture::PixelFormat::LUMINANCE_8:
            internalFormat = GL_R8;
            format = GL_RED;
            type = GL_UNSIGNED_BYTE;
            break;
        case Texture::PixelFormat::LUMINANCE_16:        
            internalFormat = GL_R16;
            format = GL_RED;
            type = GL_UNSIGNED_SHORT;
            break;
    };
}

/** Constructor. **/
Texture2D::Texture2D()
    : _size(0)
    , _format(Texture::PixelFormat::UNKNOWN)
    , _id(0)
    , _infos(Texture::PixelFormat())
{}

/**Destructor. **/
Texture2D::~Texture2D()
{
    destroy();
}

/** 
 * Create texture.
 * @param [in] size   Texture size.
 * @param [in] format Texture format.
 */
bool Texture2D::create(const glm::ivec2& size, Texture::PixelFormat format)
{
    if(_id)
    {
        destroy();
    }
    glGenTextures( 1, &_id );
    _format = format;
    _infos  = Texture2D::OpenGLTextureInfos(format);

    setData(NULL);
    
    return true;
}
/**
 * Destroy texture.
 */
void Texture2D::destroy()
{
    if(_id)
    {
        glDeleteTextures(1, &_id);
        _id = 0;
    }
}
/**
 * Set texture data.
 * @param [in] data Image buffer.
 */
void Texture2D::setData(void* data)
{
    glTexImage2D(GL_TEXTURE_2D, 0, _infos.internalFormat, _size.x, _size.y, 0, _infos.format, _infos.type, NULL);
}
/**
 * Bind texture.
 */
void Texture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D, _id);
}

/**
 * Set texel magnification filter.
 * @param [in] filter Magnification filter.
 */
void Texture2D::setMagFilter(Texture::MagFilter filter)
{
    static const GLenum glMagFilter[] = { GL_NEAREST, GL_LINEAR };
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMagFilter[filter]);
}
/**
 * Set texel minification filter.
 * @param [in] filter Minification filter.
 */
void Texture2D::setMinFilter(Texture::MinFilter filter)
{
    static const GLenum glMinFilter[] = 
    { 
        GL_NEAREST, GL_LINEAR,
        GL_NEAREST_MIPMAP_NEAREST,
        GL_NEAREST_MIPMAP_LINEAR,
        GL_LINEAR_MIPMAP_NEAREST,
        GL_LINEAR_MIPMAP_LINEAR
    };
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinFilter[filter]);    
}
/**
 * Set texture coordinates wrap mode.
 * @param [in] s Wrap mode for s coordinate (1st).
 * @param [in] t Wrap mode for t coordinate (2nd).
 */
void Texture2D::setWrap(Texture::Wrap s, Texture::Wrap t)
{
    static const GLenum glWrapMode[] =
    {
        GL_REPEAT,
        GL_CLAMP_TO_EDGE,
        GL_CLAMP_TO_BORDER,
        GL_MIRRORED_REPEAT,
        GL_MIRROR_CLAMP_TO_EDGE
    };
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrapMode[s]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrapMode[t]);
}
/**
 * Set border color.
 * @param [in] color Border color (rgba).
 */
void Texture2D::setBorderColor(const glm::vec4& color)
{
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
}
/**
 * Generate mipmap from texture.
 */
void Texture2D::buildMipmap()
{
    glBindTexture(GL_TEXTURE_2D, _id);
    glGenerateMipmap(GL_TEXTURE_2D);
}

}
