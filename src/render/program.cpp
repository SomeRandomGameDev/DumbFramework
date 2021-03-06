/*
 * Copyright 2015 MooZ
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <DumbFramework/render/program.hpp>

namespace Dumb   {
namespace Render {

/**
 * Constructor.
 */
Program::Program()
    : _id ()
{}
/**
 * Copy constructor.
 */
Program::Program(Program const& prog)
    : _id(prog._id)
{}
/**
 * Destructor.
 */
Program::~Program()
{
    destroy();
}
/**
 * Copy operator.
 */
Program& Program::operator= (Program const& prog)
{
    _id = prog._id;
    return *this;
}
/**
 * Create program.
 */
bool Program::create()
{
    if(_id)
    {
        destroy();
    }
    
    GLuint id = glCreateProgram();
    if(0 == id)
    {
        return false;
    }
    _id = std::make_shared<GLuint>(id);
    return true;
}
/**
 * All-in-one program creation.
 * Creates, loads and compiles shaders with the parameters provided.
 * Those shaders are then attached to the program.
 * Note that the program is not linked. You will have to call it
 * beforehand.
 * @param [in] attr A list of shader type and source code string tuples.
 * @return true if the shaders where succesfully compiled and attached to the program.
 */
bool Program::create(std::initializer_list<std::pair<Shader::Type, char const *>> const& attr)
{
    bool ret;
    
    // Create program
    ret = create();
    if(false == ret)
    {
        return ret;
    }
    
    std::pair<Shader::Type, char const*> const* it;
    int i;
    for(it=attr.begin(), i=0; ret && (it!=attr.end()); ++it, ++i)
    {
        Shader shader;
        ret = shader.create(it->first, it->second);
        if(false == ret)
        {
            Log_Error(Dumb::Module::Render, "Failed to create shader #%d", i);
        }
        else
        {
            ret = attach(shader);
        }
    }
    return ret;
}
/**
 * Attach a shader to current program.
 * @param [in] shader Shader to attach.
 */
bool Program::attach(Shader const& shader)
{ 
    if(false == _id)
    {
        Log_Error(Dumb::Module::Render, "Can't attach shader to an uninitialized program!");
        return false;
    }
    glAttachShader(*_id, shader._id);
    GLenum err = glGetError();
    if(GL_NO_ERROR != err)
    {
        Log_Error(Dumb::Module::Render, "Can't attach shader (%x) to program (%x) : %s", shader._id, *_id, gluErrorString(err));
        return false;
    }
    
    return true;
}
/**
 * Link program.
 */
bool Program::link()
{
    if(false == _id)
    {
        return false;
    }
    
    glLinkProgram(*_id);

    GLint result;
    glGetProgramiv(*_id, GL_LINK_STATUS, &result);
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
    if(_id)
    {
        GLuint current;
        glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&current);
        return (current == *_id);
    }
    return false;
}
/**
 * Use program.
 */
bool Program::begin() const
{
    if(false == _id)
    {
        return false;
    }
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLuint current;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&current);
    if(current && (current != *_id))
    {
        Log_Warning(Dumb::Module::Render, "Program %d is currently in use.", current);
    }
#endif // SANITY_CHECK
    glUseProgram(*_id);
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
    if((true == _id) && (current != *_id))
    {
        Log_Warning(Dumb::Module::Render, "You are trying to end using program %d whereas the current active program is %d", _id, current);
        Log_Warning(Dumb::Module::Render, "If you really want to end any program currently in use, call Program::endAny() (static) instead.");
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
        if(1 == _id.use_count())
        {
            // Delete program.
            glDeleteProgram (*_id);
        }
        _id.reset();
    }
}
/**
 * Output link status. 
 */
void Program::infoLog(Dumb::Severity severity) const
{
    if(!_id)
    {
        return;
    }
    
    GLsizei maxLogLength, logLength;
    GLchar *log;

    glGetProgramiv(*_id, GL_INFO_LOG_LENGTH, (GLint*)&maxLogLength);
    if(!maxLogLength)
        return;
    
    log = new GLchar[maxLogLength];
    if(log == NULL)
    {
        Log_Error(Dumb::Module::Render, "Not enough memory");
        return;
    }

    glGetProgramInfoLog(*_id, maxLogLength, &logLength, log);
    Log_Ex(Dumb::Module::Render, severity, log);

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
    glBindAttribLocation(*_id, index, name);
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
        glBindAttribLocation(*_id, it->first, it->second);
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
    glBindFragDataLocation(*_id, index, name);
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
        glBindFragDataLocation(*_id, it->first, it->second);
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
    glTransformFeedbackVaryings(*_id, names.size(), ptr, (interleaved ? GL_INTERLEAVED_ATTRIBS : GL_SEPARATE_ATTRIBS));
}
/**
 * Delete attached shaders.
 */
void Program::destroyShaders()
{
    if(_id)
    {
        GLsizei count;
        glGetProgramiv(*_id, GL_ATTACHED_SHADERS, &count);
        if(count)
        {
            GLuint* shaders = new GLuint[count];
            glGetAttachedShaders(*_id, count, NULL, &shaders[0]);
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
    GLint uid = glGetUniformLocation(*_id, name);
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        Log_Error(Dumb::Module::Render, "Unable to retrieve uniform %s id: %s", name, gluErrorString(err));
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
    GLint uid = glGetAttribLocation(*_id, name);
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        Log_Error(Dumb::Module::Render, "Unable to retrieve attribute %s id: %s", name, gluErrorString(err));
    }
    return static_cast<int>(uid);
}

#if defined(SANITY_CHECK)
#define CHECK_GL_ERRORS do { GLenum err = glGetError(); if(GL_NO_ERROR != err) {Log_Error(Dumb::Module::Render, (const char*)gluErrorString(err)); } } while(0); \

#else
#define CHECK_GL_ERRORS
#endif // SANITY_CHECK

template<>
void Program::uniform<int>(int id, int const& i) const
{ glUniform1i(id, i); CHECK_GL_ERRORS }

template<>
void Program::uniform<unsigned int>(int id, unsigned int const& u) const
{ glUniform1ui(id, u); CHECK_GL_ERRORS }

template<>
void Program::uniform<float>(int id, float const& f) const
{ glUniform1f(id, f); CHECK_GL_ERRORS }

template<>
void Program::uniform<double>(int id, double const& d) const
{ glUniform1d(id, d); CHECK_GL_ERRORS }

template<>
void Program::uniform<glm::vec2>(int id, glm::vec2 const& v) const
{ glUniform2fv(id, 1, glm::value_ptr(v)); CHECK_GL_ERRORS }

template<>
void Program::uniform<glm::vec3>(int id, glm::vec3 const& v) const
{ glUniform3fv(id, 1, glm::value_ptr(v)); CHECK_GL_ERRORS }

template<>
void Program::uniform<glm::vec4>(int id, glm::vec4 const& v) const
{ glUniform4fv(id, 1, glm::value_ptr(v)); CHECK_GL_ERRORS }

template<>
void Program::uniform<glm::dvec2>(int id, glm::dvec2 const& v) const
{ glUniform2dv(id, 1, glm::value_ptr(v)); CHECK_GL_ERRORS }

template<>
void Program::uniform<glm::dvec3>(int id, glm::dvec3 const& v) const
{ glUniform3dv(id, 1, glm::value_ptr(v)); CHECK_GL_ERRORS }

template<>
void Program::uniform<glm::dvec4>(int id, glm::dvec4 const& v) const
{ glUniform4dv(id, 1, glm::value_ptr(v)); CHECK_GL_ERRORS }

template<>
void Program::uniform<glm::ivec2>(int id, glm::ivec2 const& v) const
{ glUniform2iv(id, 1, glm::value_ptr(v)); CHECK_GL_ERRORS }

template<>
void Program::uniform<glm::ivec3>(int id, glm::ivec3 const& v) const
{ glUniform3iv(id, 1, glm::value_ptr(v)); CHECK_GL_ERRORS }

template<>
void Program::uniform<glm::ivec4>(int id, glm::ivec4 const& v) const
{ glUniform4iv(id, 1, glm::value_ptr(v)); CHECK_GL_ERRORS }

template<>
void Program::uniform<glm::uvec2>(int id, glm::uvec2 const& v) const
{ glUniform2uiv(id, 1, glm::value_ptr(v)); CHECK_GL_ERRORS }

template<>
void Program::uniform<glm::uvec3>(int id, glm::uvec3 const& v) const
{ glUniform3uiv(id, 1, glm::value_ptr(v)); CHECK_GL_ERRORS }

template<>
void Program::uniform<glm::uvec4>(int id, glm::uvec4 const& v) const
{ glUniform4uiv(id, 1, glm::value_ptr(v)); CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::mat4>(int id, bool transpose, glm::mat4 const& mat) const
{ glUniformMatrix4fv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::mat4x2>(int id, bool transpose, glm::mat4x2 const& mat) const
{ glUniformMatrix4x2fv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::mat4x3>(int id, bool transpose, glm::mat4x3 const& mat) const
{ glUniformMatrix4x3fv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::mat3>(int id, bool transpose, glm::mat3 const& mat) const
{ glUniformMatrix3fv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }
        
template <>
void Program::uniform<glm::mat3x2>(int id, bool transpose, glm::mat3x2 const& mat) const
{ glUniformMatrix3x2fv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::mat3x4>(int id, bool transpose, glm::mat3x4 const& mat) const
{ glUniformMatrix3x4fv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::mat2>(int id, bool transpose, glm::mat2 const& mat) const
{ glUniformMatrix2fv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }
        
template <>
void Program::uniform<glm::mat2x3>(int id, bool transpose, glm::mat2x3 const& mat) const
{ glUniformMatrix2x3fv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::mat2x4>(int id, bool transpose, glm::mat2x4 const& mat) const
{ glUniformMatrix2x4fv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::dmat4>(int id, bool transpose, glm::dmat4 const& mat) const
{ glUniformMatrix4dv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::dmat4x2>(int id, bool transpose, glm::dmat4x2 const& mat) const
{ glUniformMatrix4x2dv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::dmat4x3>(int id, bool transpose, glm::dmat4x3 const& mat) const
{ glUniformMatrix4x3dv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::dmat3>(int id, bool transpose, glm::dmat3 const& mat) const
{ glUniformMatrix3dv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }
        
template <>
void Program::uniform<glm::dmat3x2>(int id, bool transpose, glm::dmat3x2 const& mat) const
{ glUniformMatrix3x2dv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::dmat3x4>(int id, bool transpose, glm::dmat3x4 const& mat) const
{ glUniformMatrix3x4dv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::dmat2>(int id, bool transpose, glm::dmat2 const& mat) const
{ glUniformMatrix2dv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }
        
template <>
void Program::uniform<glm::dmat2x3>(int id, bool transpose, glm::dmat2x3 const& mat) const
{ glUniformMatrix2x3dv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

template <>
void Program::uniform<glm::dmat2x4>(int id, bool transpose, glm::dmat2x4 const& mat) const
{ glUniformMatrix2x4dv(id, 1, transpose, glm::value_ptr(mat)) ; CHECK_GL_ERRORS }

} // Render
} // Dumb
