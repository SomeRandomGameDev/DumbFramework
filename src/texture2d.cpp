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

/// @todo
void Texture2D::setMagFilter(Texture::MagFilter filter)
{}
/// @todo
void Texture2D::setMinFilter(Texture::MinFilter filter)
{}
/// @todo
void Texture2D::setWrap(Texture::Wrap s, Texture::Wrap t)
{}
/// @todo
void Texture2D::setBorderColor(const glm::vec4& color)
{}
/// @todo
void Texture2D::buildMipmap()
{}

}
