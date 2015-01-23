#include <DumbFramework/log.hpp>
#include <DumbFramework/render/renderbuffer.hpp>

namespace Framework {
namespace Render    {

/** Constructor. **/
Renderbuffer::Renderbuffer()
    : _size(0)
    , _samples(0)
    , _format(Texture::PixelFormat::UNKNOWN)
    , _id(0)
{}
/**Destructor. **/
Renderbuffer::~Renderbuffer()
{
    destroy();
}
/** 
 * Create renderbuffer.
 * @param [in] size    Renderbuffer size.
 * @param [in] format  Renderbuffer format.
 * @param [in] samples Samples count (default=-0).
 * @return true if the renderbuffer was succesfully created.
 */
bool Renderbuffer::create(const glm::ivec2& size, Texture::PixelFormat format, size_t samples)
{
    if(_id)
    {
        destroy();
    }
    glGenRenderbuffers(1, &_id);

    _format  = format;
    _size    = size;
    _samples = samples;
    
    glBindRenderbuffer(GL_RENDERBUFFER, _id);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, _samples, _format.internalFormat(), _size.x, _size.y);
    GLenum err = glGetError();
    bool ret = (GL_NO_ERROR == err);
    if(!ret)
    {
        Log_Error(Module::Render, "Failed to create renderbuffer: %s", gluErrorString (err));
    }
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    return ret;
}
/**
 * Destroy renderbuffer.
 */
void Renderbuffer::destroy()
{
    _size    = glm::ivec2(0);
    _samples = 0;
    _format  = Texture::PixelFormat::UNKNOWN;
    if(_id)
    {
        glDeleteRenderbuffers(1, &_id);
        _id = 0;
    }
}
/**
 * Tell if the renderbuffer is valid.
 * @return true if the renderbuffer is valid.
 */
bool Renderbuffer::isValid() const
{
    return (0 != _id);
}
/**
 * Bind renderbuffer.
 */
void Renderbuffer::bind() const
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    if(!isValid())
    {
        Log_Warning(Module::Render, "You are trying to bind an invalid texture for target %s", GetTextureTargetName(_target));
    }
    else
    {
        GLuint bound;
        glGetIntegerv(GL_RENDERBUFFER_BINDING, (GLint*)&bound);
        if(bound && (bound != _id))
        {
            Log_Warning(Module::Render, "Renderbuffer %d is currently bound.", bound);
        }
    }
#endif // SANITY_CHECK
    glBindRenderbuffer(GL_RENDERBUFFER, _id);
}
/**
 * Unbind renderbuffer.
 */
void Renderbuffer::unbind() const
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLuint bound;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, (GLint*)&bound);
    if(bound != _id)
    {
        if(bound)
        {
            Log_Warning(Module::Render, "You are trying to unbind renderbuffer %d whereas the current bound renderbuffer is %d", _id, bound);
            Log_Warning(Module::Render, "If you really want to unbind the currently bound renderbuffer use Renderbuffer::unbindAll() (static) instead.");
        }
        else
        {
            Log_Warning(Module::Render, "No renderbuffer bound.");
        }
    }
#endif // SANITY_CHECK
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
/**
 * Check if the renderbuffer is bound.
 * @return true if the renderbuffer is bound.
 */
bool Renderbuffer::isBound() const
{
    GLuint bound;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, (GLint*)&bound);
    return (bound == _id);
}
/**
 * Unbind currently bound renderbuffers.
 */
void Renderbuffer::unbindAll()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
/**
 * Get renderbuffer width and height
 * @return renderbuffer width and height as a glm::ivec2
 */
const glm::ivec2& Renderbuffer::size() const
{
    return _size;
}
/**
 * Get renderbuffer sample count.
 * @return sample count.
 */
size_t Renderbuffer::samples() const
{
    return _samples;
}
/**
 * Get renderbuffer pixel format.
 * @return pixel format.
 */
const Texture::PixelFormat& Renderbuffer::pixelFormat() const
{
    return _format;
}
/**
 * Get renderbuffer id.
 * @return renderbuffer id.
 */
GLuint Renderbuffer::id() const
{
    return _id;
}

} // Render
} // Framework