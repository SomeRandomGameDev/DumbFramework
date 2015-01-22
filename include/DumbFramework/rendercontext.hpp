#ifndef _DUMB_FW_RENDER_CONTEXT_
#define _DUMB_FW_RENDER_CONTEXT_

#include <glm/glm.hpp>
#include <DumbFramework/config.hpp>
#include <DumbFramework/texture2d.hpp>

namespace Framework {
namespace Render    {

class Context
{
    public:
        enum AttachmentPoint
        {
            DEPTH = 0,
            STENCIL,
            DEPTH_STENCIL,
            COLOR
        };
    
    public:
        Context();
        ~Context();
        
        bool create();
        void destroy();
        bool compile();
        
        bool bind();
        void unbind();
        
        bool attach(AttachmentPoint point, size_t index, Texture2D *target, int level=0, int layer=0);
        Texture2D* output(AttachmentPoint point, size_t index);
        
        glm::ivec2 const& size() const;
        
    protected:
        struct Output
        {
            GLenum     attachment;
            Texture2D *target;
            GLint      level;
            GLint      layer;
            
            inline bool complete() const;
        };
        
        enum InternalState
        {
            UNITIALIZED = 0,
            COMPILATION_NEEDED,
            READY
        };
        
    protected:
        InternalState _state;
        GLuint _framebuffer;
        
        size_t  _colorAttachmentCount;
        GLenum* _colorAttachments;
        
        size_t  _outputCapacity;
        size_t  _outputCount;
        Output* _outputs;
};

} // Render
} // Framework

#include "rendercontext.inl"

#endif // _DUMB_FW_RENDER_CONTEXT_
