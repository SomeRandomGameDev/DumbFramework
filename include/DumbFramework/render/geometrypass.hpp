#ifndef _DUMB_FW_RENDER_GEOMETRY_PASS_
#define _DUMB_FW_RENDER_GEOMETRY_PASS_

#include <DumbFramework/geometry/camera.hpp>
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
            NORMAL,
            WORLD_POS,
            OUTPUT_LAYER_COUNT
        };
        
    public:
        GeometryPass();
        ~GeometryPass();
        
        bool create(glm::ivec2 const& viewportSize);
        void destroy();
        
        void begin(Dumb::Core::Geometry::Camera const& camera);
        void end();
        void render(Material& material, glm::mat4 const& modelMatrix, glm::mat3 const& normalMatrix, Mesh const& mesh);
        
        Texture2D* output();
        Renderbuffer* depthbuffer();
        
        void debug(OutputLayer layer, glm::ivec2 const& pos, glm::ivec2 const& size);
        
    protected:
        GLuint        _framebuffer;
        Program       _program;
        Texture2D     _output;
        Renderbuffer  _depthbuffer;
        
        glm::ivec2 _viewportSize;
        int _modelMatrixId;
        int _normalMatrixId;
        UniformBuffer _view;
};

} // Render
} // Framework

#endif /* _DUMB_FW_RENDER_GEOMETRY_PASS_ */
