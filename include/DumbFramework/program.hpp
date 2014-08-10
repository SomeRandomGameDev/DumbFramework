#ifndef _DUMB_FW_PROGRAM_
#define _DUMB_FW_PROGRAM_

#include <initializer_list>

#include <DumbFramework/log.hpp>
#include <DumbFramework/shader.hpp>

namespace Framework {

/**
 * GLSL program wrapper.
 */
class Program
{
    public:
        /** 
         * Constructor. 
         */
        Program();
        /**
         * Destructor.
         */
        ~Program();
        /** 
         * Create program.
         */
        bool create();
        /**
         * Attach a shader to current program.
         * @param [in] shader Shader to attach.
         */
        bool attach(Shader const& shader);
        /**
         * Link program.
         */
        bool link();
        /**
         * Bind attribute location.
         * @note This must be done before link.
         * @param [in] index Attribute index.
         * @param [in] name Attribute name.
         */
        void bindAttribLocation(unsigned index, char const* name);
        /**
         * Bind multiple attribute locations.
         * @note This must be done before link.
         * @param [in] attr  A list of index, name tuples.
         */
        void bindAttribLocation(std::initializer_list<std::pair<unsigned int, char const*>> const& attr);
        /**
         * Bind fragment output location.
         * \note This must be done before link.
         * \param [in] index Fragment output index.
         * \param [in] name Fragment output name.
         */
        void bindFragDataLocation(unsigned int index, char const* name);        
        /**
         * Bind multiple fragment output locations.
         * @note This must be done before link.
         * @param [in] fragData  A list of index, name tuples.
         */
        void bindFragDataLocation(std::initializer_list<std::pair<unsigned int, char const*>> const& fragData);
        /**
         * Specify varying outputs for transform feedback
         * @param [in] interleaved Indicates if the varyings are interleaved.
         * @param [in] names       List of varying outputs.
         */
        void transformFeedbackVaryings(bool interleaved, std::initializer_list<char const*> names);
        /**
         * Check if the program is in use.
         * @return true if the program is currently used.
         */
        bool isInUse() const;
        /**
         * Use program.
         */
        bool begin() const;
        /**
         * Stop using program.
         * When the SANITY_CHECK flag is enable, it checks if the program is currently bound.
         */
        void end() const;
        /**
         * Any active program will be stopped.
         */
        static void endAny();
        /**
         * Delete program and detach shaders.
         */
        void destroy();
        /** 
         * Get uniform variable location.
         * @param [in] name Uniform variable name.
         * @return Uniform variable index.
         */
        int getUniformLocation(char const* name);
        /**
         * Get attribute location.
         * @param [in] name Attribute name.
         * @return Attribute index.
         */
        int getAttribLocation(char const* name);
        /**
         * Output link status. 
         */
        void infoLog(Framework::Severity severity = Framework::Severity::Error) const;
        /**
         * Delete attached shaders.
         */
        void destroyShaders();
/*
[todo]
glUniform1i
glUniform1iv
glUniform2i
glUniform2iv
glUniform3i
glUniform3iv
glUniform4i
glUniform4iv
glUniform1ui
glUniform1uiv
glUniform2ui
glUniform2uiv
glUniform3ui
glUniform3uiv
glUniform4ui
glUniform4uiv
glUniform1f
glUniform1fv
glUniform2f
glUniform2fv
glUniform3f
glUniform3fv
glUniform4f
glUniform4fv
glUniform1d
glUniform1dv
glUniform2d
glUniform2dv
glUniform3d
glUniform3dv
glUniform4d
glUniform4dv
glUniformMatrix2fv
glUniformMatrix2dv
glUniformMatrix2x3fv
glUniformMatrix2x3dv
glUniformMatrix2x4fv
glUniformMatrix2x4dv
glUniformMatrix3fv
glUniformMatrix3dv
glUniformMatrix3x2fv
glUniformMatrix3x2dv
glUniformMatrix3x4fv
glUniformMatrix3x4dv
glUniformMatrix4fv
glUniformMatrix4dv
glUniformMatrix4x2fv
glUniformMatrix4x2dv
glUniformMatrix4x3fv
glUniformMatrix4x3dv
*/
    private:
        GLuint _id; /**< Program id */
};

} // Framework

#endif /* _DUMB_FW_PROGRAM_ */
