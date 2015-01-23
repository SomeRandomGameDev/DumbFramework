#ifndef _DUMB_FW_RENDER_LIGHT_RENDERER_
#define _DUMB_FW_RENDER_LIGHT_RENDERER_

#include <DumbFramework/render/vertexbuffer.hpp>
#include <DumbFramework/render/vertexstream.hpp>
#include <DumbFramework/render/light/light.hpp>

namespace Framework {
namespace Render    {

class LightPass
{
    public:
    // Generate shadow maps for shadow casting lights
    // Light => screen space quad covering light bounding object
    //          vertex buffer => light position + extra infos depending of lights
    //          vertex/geometry/pixel shader per type of light
    //          geometry shader build screen quad depending of light type
    //          pixel shader => compute brdf
        void draw(/* [todo] GBuffer */);
        void attachOutput(Texture2D *out);
    protected:
        Render::RenderContext _renderContext;
        Render::VertexBuffer  _vertexBuffer;
        Render::VertexStream  _vertexStream;
        Render::Program       _program;
};

} // Render
} // Framework

#endif /* _DUMB_FW_RENDER_LIGHT_RENDERER_ */
