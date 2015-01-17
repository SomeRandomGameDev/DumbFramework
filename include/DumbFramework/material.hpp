#ifndef _DUMB_FW_MATERIAL_
#define _DUMB_FW_MATERIAL_

#include <DumbFramework/program.hpp>
#include <DumbFramework/uniformbuffer.hpp>

namespace Framework {
namespace Render    {

class Material
{
    public:
        Material();
        ~Material();

        void bind();
        void unbind();

        void texture2D(std::string const& name, Texture2D const& texture);
        Texture2D const& texture2D(std::string const& name);

        void program(Program const& prog);
        Program const& program() const;

        std::string const& name() const;

        int input(std::string const& name) const;

        template<typename T>
        void set(std::string const& name, T const& value);

        template<typename T>
        void set(std::string const& name, bool transpose, T const& value);

        template<typename T>
        T const& get(std::string const& name) const;

    public:
        bool visible;

        bool      blend;
        BlendFunc blendFunc;
                
        bool      depthWrite;
        DepthFunc depthFunc;
        
        bool     culling;
        CullFace cullingmode;
        
    protected:
        std::string _name;
        std::vector<std::pair<std::string, Texture2D>> _textures2D;
        Program _program;
        UniformBuffer _uniformBuffer;
};

} // Render
} // Framework

#endif // _DUMB_FW_MATERIAL_
