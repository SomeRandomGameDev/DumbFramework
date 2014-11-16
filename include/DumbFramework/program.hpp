#ifndef _DUMB_FW_PROGRAM_
#define _DUMB_FW_PROGRAM_

#include <initializer_list>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <DumbFramework/log.hpp>
#include <DumbFramework/shader.hpp>

namespace Framework {

/**
 * GLSL program wrapper.
 * @ingroup DUMB_FW_RENDERING
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
         * All-in-one program creation.
         * Creates, loads and compiles shaders with the parameters provided.
         * Those shaders are then attached to the program.
         * Note that the program is not linked. You will have to call it
         * beforehand.
         * @param [in] attr A list of shader type and source code string tuples.
         * @return true if the shaders where succesfully compiled and attached to the program.
         */
        bool create(std::initializer_list<std::pair<Shader::Type, char const *>> const& attr);
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
        /**
         * Set uniform variable.
         * @param [in] id  Uniform identifier.
         * @param [in] t   Data. Supported types are int, unsigned int
         *                 float, double, glm::vec2, glm::vec3, 
         *                 glm::vec4, glm::dvec2, glm::dvec3, 
         *                 glm::dvec4, glm::ivec2, glm::ivec3, 
         *                 glm::ivec4, glm::uvec2, glm::uvec3 and
         *                 glm::uvec4.
         */
        template <typename T>
        void uniform(int id, T const& t) const;
        /**
         * Set uniform matrix.
         * @param [in] id        Uniform identifier.
         * @param [in] transpose Does the matrix need to be transposed?
         * @param [in] t         Matrix. Supported types are glm::mat2,
         *             glm::mat2x3, glm::mat2x4, glm::mat3, glm::mat3x2,
         *             glm::mat3x4, glm::mat4, glm::mat4x2, glm::mat4x3,
         *             glm::dmat2, glm::dmat2x3, glm::dmat2x4,
         *             glm::dmat3, glm::dmat3x2, glm::dmat3x4,
         *             glm::dmat4, glm::dmat4x2 and glm::dmat4x3.
         */
        template <typename T>
        void uniform(int id, bool transpose, T const& mat) const;

    private:
        GLuint _id; /**< Program id */
};

} // Framework

#endif /* _DUMB_FW_PROGRAM_ */
