#ifndef _DUMB_FW_RENDER_LIGHT_PASS_
#define _DUMB_FW_RENDER_LIGHT_PASS_

#include <memory>

#include <DumbFramework/render/vertexbuffer.hpp>
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

    protected:
        Texture2D* _gbuffer;
        Renderbuffer* _depthbuffer;
        
        GLuint        _framebuffer;
        Texture2D     _output;

        // [todo] per light type
        size_t _count;
        Render::VertexBuffer  _vertexBuffer;
        Render::VertexStream  _vertexStream;
        Render::Program       _program;
};

// Generate shadow maps for shadow casting lights
// Light => screen space quad covering light bounding object
//          vertex buffer => light position + extra infos depending of lights
//          vertex/geometry/pixel shader per type of light
//          geometry shader build screen quad depending of light type
//          pixel shader => compute brdf

} // Render
} // Framework

#endif /* _DUMB_FW_RENDER_LIGHT_PASS_ */
