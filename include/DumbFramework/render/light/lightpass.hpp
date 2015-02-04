#ifndef _DUMB_FW_RENDER_LIGHT_PASS_
#define _DUMB_FW_RENDER_LIGHT_PASS_

#include <memory>

#include <DumbFramework/render/vertexbuffer.hpp>
#include <DumbFramework/render/uniformbuffer.hpp>
#include <DumbFramework/render/vertexstream.hpp>
#include <DumbFramework/render/geometrypass.hpp>
#include <DumbFramework/render/light/pointlight.hpp>

namespace Framework {
namespace Render    {

class LightPass
{
    public:
        LightPass();
        ~LightPass();
        
        bool create(Texture2D* gbuffer, Renderbuffer* depthbuffer);
        void destroy();
        
        void clear();
        
        // [todo] one per light
        bool add(PointLight const& light);
        
        void draw(Camera const& camera);
        Texture2D* output();

        void debug(glm::ivec2 const& pos, glm::ivec2 const& size);

    protected:
        Texture2D* _gbuffer;
        Renderbuffer* _depthbuffer;
        Program _program;
        
        GLuint        _framebuffer;
        Texture2D     _output;

        VertexStream _fsQuad;
        VertexBuffer _fsQuadBuffer;
        
        // [todo] one per light type
        UniformBuffer _buffer;
        unsigned int _count;
        GLuint _countId;
};

} // Render
} // Framework

#endif /* _DUMB_FW_RENDER_LIGHT_PASS_ */
