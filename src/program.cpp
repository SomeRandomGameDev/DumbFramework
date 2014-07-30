#include <DumbFramework/program.hpp>

namespace Framework {

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
        Log_Error(Framework::Module::Render, "Can't attach shader to an uninitialized program!");
        return false;
    }

    glAttachShader(_id, shader.getId());
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        Log_Error(Framework::Module::Render, "Can't attach shader (%x) to program (%x) : %s", shader.getId(), _id, gluErrorString(err));
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
        infoLog();
        return false;
    }
    return true;
}
/**
 * Check if the program is in use.
 * @return true if the program is currently used.
 */
bool Program::isInUse() const
{
    return (Program::getCurrentProgramId() == _id);
}
/**
 * Use program.
 */
bool Program::begin() const
{
    if(!_id)
    {
        return false;
    }
#if defined(SANITY_CHECK)
    GLuint progID = Program::getCurrentProgramId();
    if(progID && (progID != _id))
    {
        Log_Warning(Module::Render, "Program %d is currently in use.");
    }
#endif // SANITY_CHECK

    glUseProgram(_id);
    
    return true;
}

/**
 * Stop using program.
 * When the SANITY_CHECK flag is enable, it checks if the program is currently bound.
 */
void Program::end() const
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLuint progID =  Program::getCurrentProgramId();
    if(progID != _id)
    {
        Log_Warning(Module::Render, "You are trying to end using program %d whereas the current active program is %d", _id, progID);
        Log_Warning(Module::Render, "If you really want to end any program currently in use, call Program::endAny() (static) instead.");
    }
#endif // SANITY_CHECK
    glUseProgram(0);
}
/**
 * Any active program will be stopped.
 */
void Program::endAny()
{
    glUseProgram(0);
}
/**
 * Retrieve the id of the program currently in use.
 * @return Program id.
 */
GLuint Program::getCurrentProgramId()
{
    GLuint progID;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&progID);
    return progID;
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

    if(isInUse())
    {
        end();
    }

    // Delete program
    glDeleteProgram (_id);
    _id = 0;
}

/**
 * Output link status. 
 */
void Program::infoLog(Framework::Severity severity) const
{
    GLsizei maxLogLength, logLength;
    GLchar *log;

    glGetProgramiv(_id, GL_INFO_LOG_LENGTH, (GLint*)&maxLogLength);
    if(!maxLogLength)
        return;
    
    log = new GLchar[maxLogLength];
    if(log == NULL)
    {
        Log_Error(Framework::Module::Render, "Not enough memory");
        return;
    }

    glGetProgramInfoLog(_id, maxLogLength, &logLength, log);
    Log_Ex(Framework::Module::Render, severity, log);

    delete [] log;
}

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
 * Bind program input (attribute) and output (fragment output) parameters.
 * This can be viewed as a batch version of bindAttribLocation and bindFragDataLocation.
 * @param [in] input Attribute location array.
 * @param [in] inputCount Number of elements in attribute location array.
 * @param [in] output Fragment output location array.
 * @param [in] outputCount Number of elements in fragment output location array.
 * @todo add transform feedback output?
 */
void Program::bindParameters(ParameterInfo const * input, size_t inputCount, ParameterInfo const * output, size_t outputCount)
{
    if(NULL != input)
    {
        for(size_t i=0; i<inputCount; i++)
        {
            glBindAttribLocation(_id, input[i].id, input[i].name);
        }
    }
    if(NULL != output)
    {
        for(size_t i=0; i<outputCount; i++)
        {
            glBindFragDataLocation(_id, output[i].id, output[i].name);
        }
    }
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

/**
 * Delete attached shaders.
 */
void Program::destroyShaders()
{
    if(_id)
    {
        GLsizei count;
        glGetProgramiv(_id, GL_ATTACHED_SHADERS, &count);
        if(count)
        {
            GLuint* shaders = new GLuint[count];
            glGetAttachedShaders(_id, count, NULL, &shaders[0]);
            for(GLsizei i=0; i<count; i++)
            {
                glDeleteShader(shaders[i]);
            }
            delete [] shaders;
        }
    }
}

} // Framework
