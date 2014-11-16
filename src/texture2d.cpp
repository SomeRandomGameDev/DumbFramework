#include <glm/gtc/type_ptr.hpp>
#include <DumbFramework/log.hpp>
#include <DumbFramework/texture2d.hpp>

namespace Framework {

/** @return OpengGL texture binding query according to texture target. **/
static GLenum QueryFromTextureTarget(GLenum target)
{
    switch(target)
    {
        case GL_TEXTURE_2D:
            return GL_TEXTURE_BINDING_2D;
        case GL_TEXTURE_2D_ARRAY:
            return GL_TEXTURE_BINDING_2D_ARRAY;
        default:
            return GL_NONE;
    }
}
/** @return Texture target name. **/
static char const* GetTextureTargetName(GLenum target)
{
    switch(target)
    {
        case GL_TEXTURE_2D:
            return "GL_TEXTURE_2D";
        case GL_TEXTURE_2D_ARRAY:
            return "GL_TEXTURE_2D_ARRAY";
        default:
            return "UNKNOWN TARGET";
    }
}
/**
 * Retrieve the id of the texture currently bound for a specific texture target. 
 * @param [in] target  Texture target (can be GL_TEXTURE_2D or GL_TEXTURE_2D_ARRAY).
 * @return id of the texture currently bound.
 */
static GLuint GetCurrentTextureId(GLenum target)
{
    if((GL_TEXTURE_2D != target) && (GL_TEXTURE_2D_ARRAY != target))
    {
        return 0;
    }
    GLuint texID;
    GLenum query = QueryFromTextureTarget(target);
    glGetIntegerv(query, (GLint*)&texID);
    return texID;
}

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
            type = GL_UNSIGNED_BYTE;
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
    , _target(GL_NONE)
    , _layers(0)
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
 * @param [in] layers Number of texture layer (default=-1).
 *             Any value equal or less than zero will force the creation of a single standard texture.
 * @return true if the texture was succesfully created.
 */
bool Texture2D::create(const glm::ivec2& size, Texture::PixelFormat format, int layers)
{
    if(_id)
    {
        destroy();
    }
    glGenTextures( 1, &_id );
    _format = format;
    _infos  = Texture2D::OpenGLTextureInfos(format);
    _size   = size;
    _layers = layers;
    
    if(_layers > 0)
    {
        _target = GL_TEXTURE_2D_ARRAY;
        glBindTexture(_target, _id);
        // OpenGL 4.2 only: glTexStorage3D(_target, 1, _infos.internalFormat, _size.x, _size.y, _layers);
    }
    else
    {
        _target = GL_TEXTURE_2D;
        glBindTexture(_target, _id);
        // OpenGL 4.2 only: glTexStorage2D(_target, 0, _infos.internalFormat, _size.x, _size.y);
    }
    glBindTexture(_target, 0);
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
 * Tell if the texture is valid.
 * @return true if the texture is valid.
 */
bool Texture2D::isValid() const
{
    return (0 != _id);
}
/**
 * Set texture data.
 * @param [in] data  Image buffer.
 * @param [in] layer Texture layer in which the data will be copied (default=-1).
 *                   Initialize all possible layers if the layer value is negative.
 */        
bool Texture2D::setData(void* data, int layer)
{
    glBindTexture(_target, _id);

    if(GL_TEXTURE_2D == _target)
    {
        glTexImage2D(_target, 0, _infos.internalFormat, _size.x, _size.y, 0, _infos.format, _infos.type, data);
    }
    else
    {
        if(layer < 0)
        {
            glTexImage3D(_target, 0, _infos.internalFormat, _size.x, _size.y, _layers, 0, _infos.format, _infos.type, data);
        }
        else if(layer < _layers)
        {
            glTexSubImage3D(_target, 0, 0, 0, layer, _size.x, _size.y, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            Log_Error(Module::Render, "Invalid layer %d, layer count is %d", layer, _layers);
            glBindTexture(_target, 0);
            return false;
        }
    }
    GLenum err = glGetError();
    bool ret = (GL_NO_ERROR == err);
    if(!ret)
    {
        Log_Error(Module::Render, "Unable to set texture data: %s", gluErrorString (err));
    }
    glBindTexture(_target, 0);
    return ret;
}
/**
 * Bind texture.
 */
void Texture2D::bind() const
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    if(!isValid())
    {
        Log_Warning(Module::Render, "You are trying to bind an invalid texture for target %s", GetTextureTargetName(_target));
    }
    else
    {
        GLuint texID = GetCurrentTextureId(_target);
        if(texID && (texID != _id))
        {
            Log_Warning(Module::Render, "Texture %d is currently bound for target %s", texID, GetTextureTargetName(_target));
        }
    }
#endif // SANITY_CHECK

    glBindTexture(_target, _id);
}
/**
 * Unbind texture.
 */
void Texture2D::unbind() const
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLuint texID = GetCurrentTextureId(_target);
    if(texID != _id)
    {
        char const* targetName = GetTextureTargetName(_target);
        if(texID)
        {
            Log_Warning(Module::Render, "You are trying to unbind texture %d whereas the current bound texture for target %s is %d", _id, texID, targetName);
            Log_Warning(Module::Render, "If you really want to unbind currently bound textures use Texture2D::unbindAll() (static) instead.");
        }
        else
        {
            Log_Warning(Module::Render, "No texture was bound for target %s", targetName);
        }
    }
#endif // SANITY_CHECK
    glBindTexture(_target, 0);
}
/**
 * Unbind currently bound textures.
 */
void Texture2D::unbindAll()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}
/**
 * Check if the texture is bound.
 * @return true if the texture is bound.
 */
bool Texture2D::isBound() const
{
    GLuint texID;
    GLenum query = QueryFromTextureTarget(_target);
    glGetIntegerv(query, (GLint*)&texID);
    return (texID == _id);
}
/**
 * Set texel magnification filter.
 * @param [in] filter Magnification filter.
 * @note Texture must be bound before calling this method.
 */
void Texture2D::setMagFilter(Texture::MagFilter filter)
{
    glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, filter);
}
/**
 * Set texel minification filter.
 * @param [in] filter Minification filter.
 * @note Texture must be bound before calling this method.
 */
void Texture2D::setMinFilter(Texture::MinFilter filter)
{
    glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, filter);
}
/**
 * Set texture coordinates wrap mode.
 * @param [in] s Wrap mode for s coordinate (1st).
 * @param [in] t Wrap mode for t coordinate (2nd).
 * @note Texture must be bound before calling this method.
 */
void Texture2D::setWrap(Texture::Wrap s, Texture::Wrap t)
{
    glTexParameteri(_target, GL_TEXTURE_WRAP_S, s);
    glTexParameteri(_target, GL_TEXTURE_WRAP_T, t);
}
/**
 * Set border color.
 * @param [in] color Border color (rgba).
 * @note Texture must be bound before calling this method.
 */
void Texture2D::setBorderColor(const glm::vec4& color)
{
    glTexParameterfv(_target, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
}
/**
 * Generate mipmap from texture.
 */
void Texture2D::buildMipmap()
{
    glBindTexture(_target, _id);
        glGenerateMipmap(GL_TEXTURE_2D); // [todo] There may be some problems as we only defined 1 level during texture creation.
    glBindTexture(_target, 0);
}
/**
 * Get texture width and height
 * @return texture width and height as a glm::ivec2
 */
const glm::ivec2& Texture2D::size() const
{
    return _size;
}
/**
 * Get texture pixel format.
 * @return pixel format.
 */
const Texture::PixelFormat& Texture2D::pixelFormat() const
{
    return _format;
}
/**
* Get layer count.
* @return layer count.
*/
int Texture2D::layerCount() const
{
    return _layers;
}

} // Framework
