#ifndef _DUMB_FW_MATERIAL_
#define _DUMB_FW_MATERIAL_

#include <DumbFramework/program.hpp>
#include <DumbFramework/renderer.hpp>
#include <DumbFramework/texture2d.hpp>
#include <DumbFramework/uniformbuffer.hpp>

namespace Framework {
namespace Render    {

/**
 * Material.
 * [todo]
 */
class Material
{
    public:
        Material();
        ~Material();

        bool create(std::string const& name);
        void destroy();

        void bind();
        void unbind();

        std::string const& name() const;

        void attach(Program const& prog);

    public:
        bool visible;

        bool      blend;
        BlendFunc srcBLend, dstBlend;

        bool      depthWrite;
        bool      depthTest;
        DepthFunc depthFunc;
        
        bool     culling;
        CullFace cullingMode;

        Texture2D   diffuseMap;
        Texture2D   specularMap;
        float       shininess;

        // [todo] normal map, etc...
        
    private:
        std::string _name;
        Program     _program;
        int         _shininessId; // [todo] change to UBO if there're more uniforms.
};

} // Render
} // Framework

#endif // _DUMB_FW_MATERIAL_
