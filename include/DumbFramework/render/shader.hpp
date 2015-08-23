#ifndef _DUMB_FW_SHADER_
#define _DUMB_FW_SHADER_

#include <DumbFramework/config.hpp>
#include <DumbFramework/log.hpp>
#include <DumbFramework/render.hpp>

namespace Framework {
namespace Render    {

/**
 * GLSL Shader wrapper.
 * @ingroup DUMB_FW_RENDERING
 */
class Shader
{
    friend class Program;
    
    public:
        /** Shader types. */
        enum Type
        {
            VERTEX_SHADER = 0,  /**< Vertex shader. **/
            GEOMETRY_SHADER,    /**< Geometry shader. **/
            FRAGMENT_SHADER,    /**< Fragment shader. **/
            UNKNOWN_SHADER
        };

    public:
        /** Constructor. */
        Shader();
        /** Destructor. */
        ~Shader();

        /** Create a shader from ASCII string.
         *  @param [in] type Shader type.
         *  @param [in] in Shader source code.
         *  @return true the shader was successfully created and compiled.
         */
        bool create(Shader::Type type, char const * in);

        /** Delete shader.
         *  The shader will not be physically deleted until it is attached to a program. */
        void destroy();

        /** Get shader type. */
        Shader::Type getType() const;

        /** Retrieve shader source length. */
        size_t getSourceLength() const;

        /** Retrieve shader source.
         *  @param [out] out Character buffer where the source code will be stored.
         *  @param [in]  outSize Character buffer size in bytes.
         *  @param [out] length Source code length.
         *  @return true if the shader source code was successfully retrieved.
         */
        bool getSource(char* out, size_t outSize, size_t& length) const;

        /** Retrieve internal shader info logs. **/
        void infoLog(Dumb::Severity severity = Dumb::Severity::Error) const;

    private:
        /** Id  */
        GLuint _id;
        /** Type */
        Shader::Type _type;
};

} // Render
} // Framework

#endif /* _DUMB_FW_SHADER_ */
