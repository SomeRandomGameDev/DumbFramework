#include <cstring>

#include <DumbFramework/log.hpp>
#include <DumbFramework/render/rendercontext.hpp>

namespace Framework {
namespace Render    {

Context::Context()
    : _state(Context::UNITIALIZED)
    , _framebuffer(0)
    , _colorAttachmentCount(0)
    , _colorAttachments(nullptr)
    , _targetCount(0)
    , _targetIndex(nullptr)
    , _targets(nullptr)
{}

Context::~Context()
{
    destroy();
}

bool Context::create()
{
    if(Context::UNITIALIZED != _state)
    {
        Log_Error(Dumb::Module::Render, "Invalid context state (expected: %d, current: %d).", Context::UNITIALIZED, _state);
        return false;
    }
    
    glGenFramebuffers(1, &_framebuffer);
    GLenum err = glGetError(); 
    if(GL_NO_ERROR != err)
    {
        Log_Error(Dumb::Module::Render, (const char*)gluErrorString(err));
        return false;
    }
    
    GLint maxColorAttachments;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
    
    _targetCapacity = maxColorAttachments+3; // color + depth + stenctil + depth_stencil
    _targetCount = 0;
    _targetIndex = new int[_targetCapacity];
    memset(_targetIndex, -1, sizeof(int)*_targetCapacity);
    _targets = new Context::TargetParameter[_targetCapacity];
    memset(_targets, 0, sizeof(Context::TargetParameter)*_targetCapacity);
    
    _colorAttachmentCount = 0;
    _colorAttachments = new GLenum[maxColorAttachments];
    memset(_colorAttachments, 0, sizeof(GLenum)*maxColorAttachments);
    
    _state = Context::COMPILATION_NEEDED;
    return true;
}

void Context::destroy()
{
    _state = Context::UNITIALIZED;
    if(_framebuffer)
    {
        glDeleteFramebuffers(1, &_framebuffer);
    }
    _colorAttachmentCount = 0;
    if(nullptr != _colorAttachments)
    {
        delete [] _colorAttachments;
        _colorAttachments = nullptr;
    }
    _targetCount = 0;
    _targetCapacity = 0;
    if(nullptr != _targetIndex)
    {
        delete [] _targetIndex;
        _targetIndex = nullptr;
    }
    if(nullptr != _targets)
    {
        delete [] _targets,
        _targets = nullptr;
    }
}

bool Context::compile()
{
    if(Context::COMPILATION_NEEDED != _state)
    {
        Log_Error(Dumb::Module::Render, "Invalid context state (expected: %d, current: %d).", Context::UNITIALIZED, _state);
        return false;
    }
    
    for(size_t i=0; i<_targetCount; ++i)
    {
        if(false == _targets[i].complete())
        {
            Log_Error(Dumb::Module::Render, "There is no target for output %d.", i);
            return false;
        }
    }
    _colorAttachmentCount = 0;
    for(size_t i=0; i<_targetCount; ++i)
    {
        if(_targets[i].attachment >= GL_COLOR_ATTACHMENT0)
        {
            _colorAttachments[_colorAttachmentCount++] = _targets[i].attachment;
        }
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    for(size_t i=0; i<_targetCount; ++i)
    {
        _targets[i].attach();
#ifdef SANITY_CHECK
        GLenum err = glGetError();
        if(GL_NO_ERROR != err)
        {
            Log_Error(Dumb::Module::Render, "Unable to attach output %d : %s", i, (const char*)gluErrorString(err));
        }
#endif // SANITY_CHECK
     }

    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(GL_FRAMEBUFFER_COMPLETE != status)
    {
        GLenum err = glGetError();
        Log_Error(Dumb::Module::Render, "%s",(const char*)gluErrorString(err));
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    _state = Context::READY;
    return true;
}

bool Context::bind()
{
    if(Context::COMPILATION_NEEDED == _state)
    {
        if(false == compile())
        {
            return false;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    glDrawBuffers(_colorAttachmentCount, _colorAttachments);
    return true;
}

void Context::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
}

bool Context::addNewAttachment(Attachment point,  glm::ivec2 const& size)
{
    if(point >= _targetCapacity)
    {
        Log_Error(Dumb::Module::Render, "Invalid attachment point.");
        return false;
    }

    bool empty = true;
    glm::ivec2 contextSize(0);
    if(nullptr != _targets[0].texture)
    {
        empty = false;
        contextSize = _targets[0].texture->size();
    }
    else if(nullptr != _targets[0].renderbuffer)
    {
        empty = false;
        contextSize = _targets[0].renderbuffer->size();
    }
    
    if((false == empty) && (size != contextSize))
    {
        Log_Error(Dumb::Module::Render, "Attachment dimension mismatch. All outputs must have the same size (%d,%d).", contextSize.x, contextSize.y);
        return false;
    }

    size_t index = point;
    size_t pos;
    if(_targetIndex[index] != -1)
    {
        Log_Warning(Dumb::Module::Render, "Something is already attached to attachment %d! It will replaced.", point.to());
        pos = _targetIndex[index];
    }
    else
    {
        pos = _targetCount++;
    }
    
    _targetIndex[index] = pos;
    return true;
}

bool Context::attach(Attachment point, Texture2D *texture, int level, int layer)
{
    bool ret =  addNewAttachment(point, texture->size());
    if(false == ret)
    {
        return false;
    }
    int pos = _targetIndex[point];
    _targets[pos].attachment   = point.to();
    _targets[pos].texture      = texture;
    _targets[pos].renderbuffer = nullptr;
    _targets[pos].level        = level;
    _targets[pos].layer        = layer;
    return true;
}

bool Context::attach(Attachment point, Renderbuffer *renderbuffer)
{
    bool ret =  addNewAttachment(point, renderbuffer->size());
    if(false == ret)
    {
        return false;
    }
    int pos = _targetIndex[point];
    _targets[pos].attachment   = point.to();
    _targets[pos].texture      = nullptr;
    _targets[pos].renderbuffer = renderbuffer;
    _targets[pos].level        = 0;
    _targets[pos].layer        = 0;
    return true;
}

Texture2D*    Context::getTexture(Attachment point)
{
    size_t index = point.to();
    if(-1 == _targetIndex[index])
    {
        return nullptr;
    }
    return _targets[_targetIndex[index]].texture;
}

Renderbuffer* Context::getRenderbuffer(Attachment point)
{
    size_t index = point.to();
    if(-1 == _targetIndex[index])
    {
        return nullptr;
    }
    return _targets[_targetIndex[index]].renderbuffer;
}

glm::ivec2 Context::size() const
{
    if(nullptr != _targets[0].texture)
    {
        return _targets[0].texture->size();
    }
    if(nullptr != _targets[0].renderbuffer)
    {
        return _targets[0].renderbuffer->size();
    }
    return glm::ivec2(0);
}

} // Render
} // Framework
