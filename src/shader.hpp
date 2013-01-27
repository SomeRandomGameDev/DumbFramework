#ifndef _DUMB_FW_SHADER_
#define _DUMB_FW_SHADER_

#include <GL/glew.h>
#include <GL/glfw.h>
/** @todo configuration header. */
/** @todo some kind of logging. */

/**
 * GLSL Shader wrapper.
 */
class Shader
{
    public:
        /** Shader types. */
        enum Type
        {
            VERTEX_SHADER = 0,
            GEOMETRY_SHADER,
            FRAGMENT_SHADER,
            TESSELATION_CONTROL_SHADER,
            TESSELATION_EVALUATION_SHADER,
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

        /** Get shader id. */
        GLuint getId() const;

        /** Get shader type. */
        Shader::Type getType() const;

        /** Retrieve shader source length. */
        GLint getSourceLength() const;

        /** Retrieve shader source.
         *  @param [out] out Character buffer where the source code will be stored.
         *  @param [in]  outSize Character buffer size in bytes.
         *  @param [out] length Source code length.
         *  @return true if the shader source code was successfully retrieved.
         */
        bool getSource(char* out, size_t outSize, size_t& length) const;

        /** Retrieve internal shader info logs. 
         *  @todo add return value. 
         */
        void infoLog() const;

    private:
        /** Id  */
        GLuint _id;
        /** Type */
        Shader::Type _type;
};

#endif /* _DUMB_FW_SHADER_ */
