#ifndef _DUMB_FW_MATERIAL_
#define _DUMB_FW_MATERIAL_

#include <DumbFramework/render/program.hpp>
#include <DumbFramework/render/renderer.hpp>
#include <DumbFramework/render/texture2d.hpp>
#include <DumbFramework/render/uniformbuffer.hpp>

namespace Framework {
namespace Render    {

/**
 * Material.
 * [todo]
 */
class Material
{
    public:
        enum TextureUnit
        {
            DIFFUSE  = 0,
            SPECULAR = 1
            // [todo] normal etc...
        };
    public:
        Material();
        ~Material();

        bool create(std::string const& name);
        void destroy();

        void bind();
        void unbind();

        std::string const& name() const;

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

        // [todo] normal map, etc...
        
    private:
        std::string _name;
};

} // Render
} // Framework

#endif // _DUMB_FW_MATERIAL_
