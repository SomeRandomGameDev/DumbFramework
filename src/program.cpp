#include <program.hpp>

#include <stdarg.h>

/**
 * Constructor.
 */
Program::Program() :
	_id (0)
{}

/**
 * Destructor.
 */
Program::~Program()
{}

/**
 * Create program.
 */
bool Program::create()
{
	_id = glCreateProgram();

	if(_id == 0)
	{
		// todo InfoLog();
		return false;
	}

	return true;
}

/**
 * Attach a shader to current program.
 * @param [in] shader Shader to attach.
 */
bool Program::attach(const Shader& shader)
{
	if(_id == 0)
	{
		/// \todo : error messages
		// fprintf(stderr, "Can't attach shader to an uninitialized program!\n");
		return false;
	}

	glAttachShader(_id, shader.getId());
	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		/// \todo : error messages
		// fprintf(stderr, "Can't attach shader (%x) to program (%x) : %s", shader.Id(), _id, gluErrorString(err));
		return false;
	}
	
	return true;
}

/**
 * Link program.
 */
bool Program::link()
{
	if(_id == 0)
	{
		return false;
	}
	
	glLinkProgram(_id);

	GLint result;
	glGetProgramiv(_id, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		// todo InfoLog();
		return false;
	}
	
	return true;
}

/**
 * Use program.
 */
bool Program::begin()
{
	if(!_id)
	{
		return false;
	}
	
	glUseProgram(_id);
	
	return true;
}

/**
 * Stop using program.
 */
void Program::end()
{
	GLuint progID;
	glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&progID);
	
	if(progID == _id)
	{
		glUseProgram(0);
	}
}

/**
 * Delete program and detach shaders.
 */
void Program::destroy()
{
	if(_id <= 0)
	{
		return;
	}

	end();

	// Get number of shaders attached to program                  
	GLint shaderCount;

	glGetProgramiv(_id, GL_ATTACHED_SHADERS, &shaderCount );
	if(shaderCount > 0)
	{     
		GLuint *shaders = new GLuint[shaderCount];
		glGetAttachedShaders(_id, (GLsizei)shaderCount, NULL, shaders);
		// Detach shaders
		for(int i=0; i<shaderCount; ++i)
		{
			glDetachShader(_id, shaders[i]);
		}

		delete [] shaders;
	}

	// Delete program
	glDeleteProgram (_id);
	_id = 0;
}

/* todo
void Program::InfoLog() const
{
	GLsizei maxLogLength, logLength;
	GLchar *log;

	glGetProgramiv(_id, GL_INFO_LOG_LENGTH, (GLint*)&maxLogLength);
	if(!maxLogLength)
		return;
	
	log = new GLchar[maxLogLength];
	if(log == NULL)
	{
		Log_Error("Not enough memory");
		return;
	}

	glGetProgramInfoLog(_id, maxLogLength, &logLength, log);
	Log_Error("%s", log);

	delete [] log;
}
*/

/**
 * Bind attribute location.
 * @note This must be done before link.
 * @param [in] index Attribute index.
 * @param [in] name Attribute name.
 */
void Program::bindAttribLocation(GLuint index, const GLchar* name)
{
	glBindAttribLocation(_id, index, name);
}

/**
 * Bind fragment output location.
 * \note This must be done before link.
 * \param [in] index Fragment output index.
 * \param [in] name Fragment output name.
 */
void Program::bindFragDataLocation(GLuint index, const GLchar* name)
{
	glBindFragDataLocation(_id, index, name);
}

/**
 * Specify varying outputs for transform feedback
 * @param [in] varyingNames Varying name array.
 * @param [in] varyingNameCount Varying name count.
 * @param [in] interleaved Indicates if the varyings are interleaved.
 */
void Program::transformFeedbackVaryings(const char** varyingNames, int varyingNameCount, bool interleaved)
{
	glTransformFeedbackVaryings(_id, varyingNameCount, varyingNames, (interleaved ? GL_INTERLEAVED_ATTRIBS : GL_SEPARATE_ATTRIBS));
}
