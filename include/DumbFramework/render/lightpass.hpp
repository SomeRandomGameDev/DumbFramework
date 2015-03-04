#ifndef _DUMB_FW_RENDER_LIGHT_PASS_
#define _DUMB_FW_RENDER_LIGHT_PASS_

#include <memory>

#include <DumbFramework/render/vertexbuffer.hpp>
#include <DumbFramework/render/uniformbuffer.hpp>
#include <DumbFramework/render/indexbuffer.hpp>
#include <DumbFramework/render/vertexstream.hpp>
#include <DumbFramework/render/geometrypass.hpp>
#include <DumbFramework/render/light.hpp>

namespace Framework {
namespace Render    {

/**
 * Compute per pixel lighting.
 */
class LightPass
{
    public:
        /**
         * Constructor.
         */
        LightPass();
        /**
         * Destructor.
         */
        ~LightPass();
        /**
         * Create light pass.
         * @param [in] gbuffer     Geometry pass output.
         * @param [in] depthbuffer Depth buffer build by geometry pass.
         * @return false if an error occured.
         */
        bool create(Texture2D* gbuffer, Renderbuffer* depthbuffer);
        /**
         * Destroy light pass.
         */
        void destroy();
        /**
         * Clear light count.
         */
        void clear();
        
        // [todo] add multiple lights at once
        bool add(PointLight       const& light);
        // [todo] bool add(SpotLight        const& light);
        // [todo] bool add(DirectionalLight const& light);
        
        void draw(Camera const& camera);
        Texture2D* output();

        void debug(glm::ivec2 const& pos, glm::ivec2 const& size);

    private:
        bool createOccluders();

    protected:
        Texture2D* _gbuffer;
        Renderbuffer* _depthbuffer;
        Program _program;
        
        GLuint        _framebuffer;
        Texture2D     _output;

        UniformBuffer _view;

        VertexBuffer _occludersVertexBuffer;
        IndexBuffer  _occludersIndexBuffer;

        VertexStream _occluders[LightType::COUNT];
        VertexBuffer _buffer[LightType::COUNT];
        unsigned int _count[LightType::COUNT];
};

} // Render
} // Framework

#endif /* _DUMB_FW_RENDER_LIGHT_PASS_ */
