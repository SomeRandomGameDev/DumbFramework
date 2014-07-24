#ifndef _DUMB_FW_PROGRAM_
#define _DUMB_FW_PROGRAM_

#include <shader.hpp>
#include <log.hpp>

namespace Framework {

/**
 * GLSL program wrapper.
 */
class Program
{
	public:
		/**
		 * Program parameter info.
		 * This simple structure is used to specify attribute or fragement output location.
		 */
		struct ParameterInfo
		{
			GLint id;           /**< Parameter id. */
			GLchar const* name;	/**< Parameter name. */
		};

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
		bool attach(const Shader& shader);
			
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
		void bindAttribLocation(GLuint index, const GLchar* name);
		
		/**
		 * Bind fragment output location.
		 * \note This must be done before link.
		 * \param [in] index Fragment output index.
		 * \param [in] name Fragment output name.
		 */
		void bindFragDataLocation(GLuint index, const GLchar* name);

		/**
		 * Bind program input (attribute) and output (fragment output) parameters.
		 * This can be viewed as a batch version of bindAttribLocation and bindFragDataLocation.
		 * @param [in] input Attribute location array.
		 * @param [in] inputCount Number of elements in attribute location array.
		 * @param [in] output Fragment output location array.
		 * @param [in] outputCount Number of elements in fragment output location array.
		 */
		void bindParameters(ParameterInfo const * input, size_t inputCount, ParameterInfo const * output, size_t outputCount);
		
		/**
		 * Specify varying outputs for transform feedback
		 * @param [in] varyingNames Varying name array.
		 * @param [in] varyingNameCount Varying name count.
		 * @param [in] interleaved Indicates if the varyings are interleaved.
		 */
		void transformFeedbackVaryings(const char** varyingNames, int varyingNameCount, bool interleaved);

		/**
		 * Use program.
		 */
		bool begin();
		/**
		 * Stop using program.
		 */
		void end();

		/**
		 * Delete program and detach shaders.
		 */
		void destroy();
			
		/**
		 * Accessor.
		 */
		GLuint getId () const { return _id; }


		/**
		 * Use program (simple alias).
		 */
		inline void use() { glUseProgram(_id); }

		/** 
		 * Get uniform variable location.
		 * @param [in] name Uniform variable name.
		 * @return Uniform variable index.
		 */
		inline GLint getUniformLocation(const GLchar* name);
		/**
		 * Get attribute location.
		 * @param [in] name Attribute name.
		 * @return Attribute index.
		 */
		inline GLint getAttribLocation(const GLchar* name);
		
		/**
		 * Output link status. 
		 */
		void infoLog(Framework::Severity severity = Framework::Severity::Error) const;

        /**
         * Delete attached shaders.
         */
        void destroyShaders();
        
	private:
		GLuint _id; /**< Program id */
};

/**
 * Get uniform variable location.
 * @param [in] name Uniform variable name.
 * @return Uniform variable index.
 */
GLint Program::getUniformLocation(const GLchar* name)
{
	GLint uid = glGetUniformLocation(_id, name);
		
	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		Log_Error(Framework::Module::Render, "Can't get id for uniform var %s : %s", name, gluErrorString(err));
	}
	return uid;
}

/**
 * Get attribute location.
 * @param [in] name Attribute name.
 * @return Attribute index.
 */
GLint Program::getAttribLocation(const GLchar* name)
{
	return glGetAttribLocation(_id, name);
}

} // Framework

#endif /* _DUMB_FW_PROGRAM_ */
