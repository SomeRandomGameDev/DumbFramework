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
    
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    for(size_t i=1; i<_outputCount; ++i)
    {
// [todo]        _outputs[i].attach();
    }
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(GL_FRAMEBUFFER_COMPLETE != status)
    {
        GLenum err = glGetError();
        Log_Error(Module::Render, (const char*)gluErrorString(err));
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
    for(size_t i=1; i<_outputCount; ++i)
    {
// [todo]        _outputs[i].attach();
    }
    glDrawBuffers(_colorAttachmentCount, _colorAttachments);
    return true;
}

void Context::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
}

#if 0
bool attach(AttachmentPoint point, size_t index, Texture2D *target, int level=0, int layer=0);
Texture2D* output(AttachmentPoint point, size_t index);

glm::ivec2 const& size() const;
#endif // 0

} // Render
} // Framework
