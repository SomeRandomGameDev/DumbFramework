#ifndef _DUMB_FW_RENDER_GEOMETRY_PASS_
#define _DUMB_FW_RENDER_GEOMETRY_PASS_

#include <DumbFramework/camera.hpp>
#include <DumbFramework/render/vertexbuffer.hpp>
#include <DumbFramework/render/vertexstream.hpp>
#include <DumbFramework/render/program.hpp>
#include <DumbFramework/render/mesh.hpp>
#include <DumbFramework/render/material.hpp>
#include <DumbFramework/render/renderbuffer.hpp>

namespace Framework {
namespace Render    {

/**
 * [todo] Fill GBuffer
 */
class GeometryPass
{
    public:
        enum OutputLayer
        {
            ALBEDO = 0,
            SPECULAR,
            NORMAL_DEPTH,
            OUTPUT_LAYER_COUNT
        };
        
    public:
        GeometryPass();
        ~GeometryPass();
        
        bool create(glm::ivec2 const& viewportSize);
        void destroy();
        
        void begin();
        void end();
        void render(Camera const& camera, Material& material, Mesh const& mesh);
        // [todo] render instancied
        
        Texture2D* output();
        Renderbuffer* depthbuffer();
        
        void debug(OutputLayer layer, glm::ivec2 const& pos, glm::ivec2 const& size);
        
    protected:
        GLuint        _framebuffer;
        Program       _program;
        Texture2D     _output;
        Renderbuffer  _depthbuffer;
        
        glm::ivec2 _viewportSize;
        int _viewProjId;
};

} // Render
} // Framework

#endif /* _DUMB_FW_RENDER_GEOMETRY_PASS_ */
