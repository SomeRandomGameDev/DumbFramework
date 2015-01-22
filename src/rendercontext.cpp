#include <cstring>

#include <DumbFramework/rendercontext.hpp>
#include <DumbFramework/log.hpp>

namespace Framework {
namespace Render    {

Context::Context()
    : _state(Context::UNITIALIZED)
    , _framebuffer(0)
    , _colorAttachmentCount(0)
    , _colorAttachments(nullptr)
    , _outputCount(0)
    , _outputIndex(nullptr)
    , _outputs(nullptr)
{}

Context::~Context()
{
    destroy();
}

bool Context::create()
{
    if(Context::UNITIALIZED != _state)
    {
        Log_Error(Module::Render, "Invalid context state (expected: %d, current: %d).", Context::UNITIALIZED, _state);
        return false;
    }
    
    glGenFramebuffers(1, &_framebuffer);
    GLenum err = glGetError(); 
    if(GL_NO_ERROR != err)
    {
        Log_Error(Module::Render, (const char*)gluErrorString(err));
        return false;
    }
    
    GLint maxColorAttachments;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
    
    _outputCapacity = maxColorAttachments+3; // color + depth + stenctil + depth_stencil
    _outputCount = 0;
    _outputIndex = new int[_outputCapacity];
    memset(_outputIndex, -1, sizeof(int)*_outputCapacity);
    _outputs = new Context::Output[_outputCapacity];
    memset(_outputs, 0, sizeof(Context::Output)*_outputCapacity);
    
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
    _outputCount = 0;
    _outputCapacity = 0;
    if(nullptr != _outputIndex)
    {
        delete [] _outputIndex;
        _outputIndex = nullptr;
    }
    if(nullptr != _outputs)
    {
        delete [] _outputs,
        _outputs = nullptr;
    }
}

bool Context::compile()
{
    if(Context::COMPILATION_NEEDED != _state)
    {
        Log_Error(Module::Render, "Invalid context state (expected: %d, current: %d).", Context::UNITIALIZED, _state);
        return false;
    }
    
    for(size_t i=0; i<_outputCount; ++i)
    {
        if(false == _outputs[i].complete())
        {
            Log_Error(Module::Render, "There is no target for output %d.", i);
            return false;
        }
    }
    for(size_t i=1; i<_outputCount; ++i)
    {
        if(_outputs[i].target->size() != _outputs[0].target->size())
        {
            Log_Error(Module::Render, "Output %d dimension mismatch. All outputs must have the same size (%d,%d).", _outputs[0].target->size().x, _outputs[0].target->size().y);
            return false;
        }
    }
    
    _colorAttachmentCount = 0;
    for(size_t i=0; i<_outputCount; ++i)
    {
        if(_outputs[i].attachment >= GL_COLOR_ATTACHMENT0)
        {
            _colorAttachments[_colorAttachmentCount++] = _outputs[i].attachment;
        }
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    for(size_t i=0; i<_outputCount; ++i)
    {
        _outputs[i].attach();
#ifdef SANITY_CHECK
        GLenum err = glGetError();
        if(GL_NO_ERROR != err)
        {
            Log_Error(Module::Render, "Unable to attach output %d : %s", i, (const char*)gluErrorString(err));
        }
#endif // SANITY_CHECK
     }

    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(GL_FRAMEBUFFER_COMPLETE != status)
    {
        GLenum err = glGetError();
        Log_Error(Module::Render, "%s",(const char*)gluErrorString(err));
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

bool Context::attach(Attachment point, Texture2D *target, int level, int layer)
{
    if(point >= _outputCapacity)
    {
        Log_Error(Module::Render, "Invalid attachment point.");
        return false;
    }
    
    size_t index = point;
    size_t pos;
    if(_outputIndex[index] != -1)
    {
        Log_Warning(Module::Render, "Something is already attached to attachment %d! It will replaced.", point.to());
        pos = _outputIndex[index];
    }
    else
    {
        pos = _outputCount++;
    }
    
    _outputIndex[index] = pos;
    _outputs[pos].attachment = point.to();
    _outputs[pos].target     = target;
    _outputs[pos].level      = level;
    _outputs[pos].layer      = layer;

    return true;
}

Texture2D* Context::output(Attachment point)
{
    size_t index = point.to();
    if(-1 == _outputIndex[index])
    {
        return nullptr;
    }
    return _outputs[_outputIndex[index]].target;
}

glm::ivec2 Context::size() const
{
    if(-1 == _outputIndex[Attachment::COLOR])
    {
        return glm::ivec2(0);
    }
    return _outputs[_outputIndex[Attachment::COLOR]].target->size();
}

} // Render
} // Framework
