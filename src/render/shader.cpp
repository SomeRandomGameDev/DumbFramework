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
#include <DumbFramework/render/shader.hpp>

namespace Dumb   {
namespace Render {

/** Constructor. */
Shader::Shader()
    : _id(0)
    , _type(Shader::UNKNOWN_SHADER)
{}

/** Destructor. */
Shader::~Shader()
{}

/** Translate shader type to OpenGL enum.
 *  @param [in] type Shader type.
 *  @return OpenGL shader type or GL_INVALID_ENUM if the shader type is invalid or not supported.
 */
static GLenum translateType(Shader::Type const & type)
{
    switch(type)
    {
        case Shader::VERTEX_SHADER:
            return GL_VERTEX_SHADER;
        case Shader::GEOMETRY_SHADER:
            return GL_GEOMETRY_SHADER;
        case Shader::FRAGMENT_SHADER:
            return GL_FRAGMENT_SHADER;
        default:
            return GL_INVALID_ENUM;
    };
}

/** Create a shader from ASCII string.
  *  @param [in] type Shader type.
  *  @param [in] in Shader source code.
  *  @return true the shader was successfully created and compiled.
  */
bool Shader::create(Shader::Type type, char const * in)
{
    GLenum internalType;
    /* Sanity check. */
    if(NULL == in)
    {
        return false;
    }
    internalType = translateType(type);
    if(GL_INVALID_ENUM == internalType)
    {
        return false;
    }
    _type = type;
    
    _id = glCreateShader(internalType);
    /* @todo check for errors */

    glShaderSource(_id, 1, const_cast<GLchar const **>(&in), NULL);
    /* @todo check for errors */

    glCompileShader(_id);
    /* @todo check for error */

    GLint result;
    glGetShaderiv(_id, GL_COMPILE_STATUS, &result);
    if(GL_FALSE == result)
    {
        infoLog();
        return false;
    }
    return true;
}

/** Delete shader.
  *  The shader will not be physically deleted until it is attached t    o a program. */
void Shader::destroy()
{
    glDeleteShader(_id);
}

/** Get shader type. */
Shader::Type Shader::getType() const
{
    return _type;
}

/** Retrieve shader source length. */
size_t Shader::getSourceLength() const
{
    GLint len;
    glGetShaderiv(_id, GL_SHADER_SOURCE_LENGTH, &len);
    return static_cast<size_t>(len);
}


/** Retrieve shader source.
 *  @param [out] out Character buffer where the source code will be stored.
 *  @param [in]  outSize Character buffer size in bytes.
 *  @param [out] length Source code length.
 *  @return true if the shader source code was successfully retrieved.
 */
bool Shader::getSource(char* out, size_t outSize, size_t& length) const
{
    if(0 == _id)
    {
        return false;
    }
    GLsizei sourceLength;
    glGetShaderSource(_id, outSize, &sourceLength, out);
    length = static_cast<size_t>(sourceLength);
    return true;
}

/** Retrieve internal shader info logs. */
void Shader::infoLog(Dumb::Severity severity) const
{
    GLsizei maxLogLength, loglength;
    GLchar* log;

    glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &maxLogLength);
    if(0 == maxLogLength)
    {
        return;
    }

    log = new GLchar[maxLogLength];
    if(NULL == log)
    {
        return;
    }

    glGetShaderInfoLog(_id, maxLogLength, &loglength, log);
    Log_Ex(Dumb::Module::Render, severity, log);
    delete [] log;
}

} // Render
} // Dumb
