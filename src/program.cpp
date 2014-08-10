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
    if(_id)
    {
        destroy();
    }
    
    _id = glCreateProgram();
    if(0 == _id)
    {
        return false;
    }

    return true;
}
/**
 * Attach a shader to current program.
 * @param [in] shader Shader to attach.
 */
bool Program::attach(Shader const& shader)
{ 
    if(0 == _id)
    {
        Log_Error(Framework::Module::Render, "Can't attach shader to an uninitialized program!");
        return false;
    }
    glAttachShader(_id, shader._id);
    GLenum err = glGetError();
    if(GL_NO_ERROR != err)
    {
        Log_Error(Framework::Module::Render, "Can't attach shader (%x) to program (%x) : %s", shader._id, _id, gluErrorString(err));
        return false;
    }
    
    return true;
}
/**
 * Link program.
 */
bool Program::link()
{
    if(0 == _id)
    {
        return false;
    }
    
    glLinkProgram(_id);

    GLint result;
    glGetProgramiv(_id, GL_LINK_STATUS, &result);
    if(GL_FALSE == result)
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
    GLuint current;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&current);
    return (current == _id);
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
    // Warning! This may spam your logs!
    GLuint current;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&current);
    if(current && (current != _id))
    {
        Log_Warning(Module::Render, "Program %d is currently in use.", current);
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
    GLuint current;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&current);
    if(current != _id)
    {
        Log_Warning(Module::Render, "You are trying to end using program %d whereas the current active program is %d", _id, current);
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
 * Delete program and detach shaders.
 */
void Program::destroy()
{
    if(_id)
    {
        // Stop program.
        if(isInUse())
        {
            end();
        }
        // Delete program.
        glDeleteProgram (_id);
        _id = 0;
    }
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
 * Bind multiple attribute locations.
 * @note This must be done before link.
 * @param [in] attr  A list of index, name tuples.
 */
void Program::bindAttribLocation(std::initializer_list<std::pair<unsigned int, char const*>> const& attr)
{
    std::pair<unsigned int, char const*> const* it;
    for(it=attr.begin(); it!=attr.end(); ++it)
    {
        glBindAttribLocation(_id, it->first, it->second);
    }
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
 * Bind multiple fragment output locations.
 * @note This must be done before link.
 * @param [in] attr  A list of index, name tuples.
 */
void Program::bindFragDataLocation(std::initializer_list<std::pair<unsigned int, char const*>> const& fragData)
{
    std::pair<unsigned int, char const*> const* it;
    for(it=fragData.begin(); it!=fragData.end(); ++it)
    {
        glBindFragDataLocation(_id, it->first, it->second);
    }
}
/**
 * Specify varying outputs for transform feedback
 * @param [in] interleaved Indicates if the varyings are interleaved.
 * @param [in] names       List of varying outputs.
 */
void Program::transformFeedbackVaryings(bool interleaved, std::initializer_list<char const*> names)
{
    const char** ptr = const_cast<const char**>(names.begin());
    glTransformFeedbackVaryings(_id, names.size(), ptr, (interleaved ? GL_INTERLEAVED_ATTRIBS : GL_SEPARATE_ATTRIBS));
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
/**
 * Get uniform variable location.
 * @param [in] name Uniform variable name.
 * @return Uniform variable index.
 */
int Program::getUniformLocation(char const* name)
{
    GLint uid = glGetUniformLocation(_id, name);
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        Log_Error(Framework::Module::Render, "Unable to retrieve uniform %s id: %s", name, gluErrorString(err));
    }
    return static_cast<int>(uid);
}
/**
 * Get attribute location.
 * @param [in] name Attribute name.
 * @return Attribute index.
 */
int Program::getAttribLocation(char const* name)
{
    GLint uid = glGetAttribLocation(_id, name);
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        Log_Error(Framework::Module::Render, "Unable to retrieve attribute %s id: %s", name, gluErrorString(err));
    }
    return static_cast<int>(uid);
}

} // Framework
