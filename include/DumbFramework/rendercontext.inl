namespace Framework {
namespace Render    {

/** Constructs attachment point from value. **/
void Attachment::from(GLenum v)
{
    switch(v)
    {
        case GL_DEPTH_ATTACHMENT:
            value = Attachment::DEPTH;
        break;
        
        case GL_STENCIL_ATTACHMENT:
            value = Attachment::STENCIL;
        break;
        
        case GL_DEPTH_STENCIL_ATTACHMENT:
            value = Attachment::DEPTH_STENCIL;
        break;
        
        default:
            value = Attachment::COLOR;
            if(v >= GL_COLOR_ATTACHMENT0)
            {
                value += (v - GL_COLOR_ATTACHMENT0);
            }
        break;
    }
}
/** Convert to OpenGL compliant value. **/
GLenum Attachment::to() const
{
    switch(value)
    {
        case Attachment::DEPTH:
            return GL_DEPTH_ATTACHMENT;
        case Attachment::STENCIL:
            return GL_STENCIL_ATTACHMENT;
        case Attachment::DEPTH_STENCIL:
            return GL_DEPTH_STENCIL_ATTACHMENT;
        default:
            return GL_COLOR_ATTACHMENT0 + (value - Attachment::COLOR);
    }
}
bool operator== (Attachment const& a0, Attachment const& a1)
{ return (a0.value == a1.value); }
bool operator== (Attachment::Value const& v, Attachment const& a0)
{ return (static_cast<unsigned int>(v) == a0.value); }
bool operator== (unsigned int v, Attachment const& a0)
{ return (v == a0.value); }
bool operator== (Attachment const& a0, Attachment::Value const& v)
{ return (a0.value == static_cast<unsigned int>(v)); }
bool operator== (Attachment const& a0, unsigned int v)
{ return (a0.value == v); }
bool operator!= (Attachment const& a0, Attachment const& a1)
{ return (a0.value != a1.value); }
bool operator!= (Attachment::Value const& v, Attachment const& a0)
{ return (static_cast<unsigned int>(v) != a0.value); }
bool operator!= (unsigned int v, Attachment const& a0)
{ return (v != a0.value); }
bool operator!= (Attachment const& a0, Attachment::Value const& v)
{ return (a0.value != static_cast<unsigned int>(v)); }
bool operator!= (Attachment const& a0, unsigned int v)
{ return (a0.value != v); }


bool Context::Output::complete() const
{
    return (nullptr != target) && (GL_NONE != attachment);
}

void Context::Output::attach() const
{
    if(nullptr == target)
    {
        return;
    }
    if(target->layerCount() > 1)
    {
        glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, target->id(), level, layer);
    }
    else
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, target->id(), level);
    }
}

} // Render
} // Framework
