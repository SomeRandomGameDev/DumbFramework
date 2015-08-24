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
#include <DumbFramework/log.hpp>
#include <DumbFramework/render/vertexstream.hpp>

namespace Dumb   {
namespace Render {

/**
 * Constructor.
 */
VertexStream::VertexStream()
    : _vao()
    , _attributes()
    , _elements(nullptr)
{}
/**
 * Destructor.
 */
VertexStream::~VertexStream()
{}
/**
 * Create vertex stream.
 * @return true if the vertex stream was succesfully created.
 */
bool VertexStream::create()
{
    if(_vao)
    {
        destroy();
    }
    
    glGenVertexArrays(1, &_vao);
    GLenum err = glGetError();
    if(GL_NONE != err)
    {
        Log_Error(Dumb::Module::Render, "An error occured while creating vertex array object: %s", gluErrorString(err));
        return false;
    }
    
    return true;
}
/**
 * Destroy vertex stream.
 */
void VertexStream::destroy()
{
    if(_vao)
    {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
    _attributes.clear();
    _elements = nullptr;
}
/**
 * Add attribute to vertex stream.
 * @param [in] vertexBuffer Vertex buffer that will be associated
 *                          with the current attribute.
 * @param [in] index      Attribute index (starts at 0).
 * @param [in] type       Type of the attribute components.
 * @param [in] size       Number of components.
 * @param [in] normalize  Specify if the data should be normalized or not.
 * @param [in] stride     Number of bytes between 2 consecutives attributes.
 * @param [in] offset     Offset of the first attribute of the first vertex.
 * @param [in] divisor    Attribute divisor (default=0). 
 * @return true if the attribute was successfully set.
 */
bool VertexStream::add(VertexBuffer const* vertexBuffer, unsigned int index, Geometry::ComponentType type, size_t size, bool normalize, size_t stride, size_t offset, unsigned int divisor)
{
    return add(vertexBuffer, index, Geometry::Attribute(type, size, normalize, stride, offset, divisor));
}
/**
 * Add attribute to vertex stream.
 * @param [in] vertexBuffer Vertex buffer that will be associated
 *                          with the current attribute.
 * @param [in] index  Attribute index.
 * @param [in] attr   Attribute.
 * @return true if the attribute was successfully set.
 */
bool VertexStream::add(VertexBuffer const* vertexBuffer, unsigned int index, Geometry::Attribute const& attr)
{
    if(nullptr == vertexBuffer)
    {
        Log_Error(Dumb::Module::Render, "Invalid vertex buffer.");
        return false;
    }
    
    if(index >= _attributes.size())
    {
        _attributes.resize(index+1);
    }
    _attributes[index].first  = vertexBuffer;
    _attributes[index].second = attr;
    return true;
}
/**
 * Add a set of attributes to vertex stream.
 * @param [in] vertexBuffer Vertex buffer that will be associated
 *                          with the current attributes.
 * @param [in] attr         A list of vertex attributes.
 * @return true if the attributes were successfully added.
 */
bool VertexStream::add(VertexBuffer const* vertexBuffer, std::initializer_list<std::pair<unsigned int, Geometry::Attribute>> const& attr)
{
    if(nullptr == vertexBuffer)
    {
        Log_Error(Dumb::Module::Render, "Invalid vertex buffer.");
        return false;
    }
    unsigned int biggestIndex = 0;

    const std::pair<unsigned int, Geometry::Attribute>* ptr;
    for(ptr=attr.begin(); ptr!=attr.end(); ptr++)
    {
        if(ptr->first > biggestIndex)
        {
            biggestIndex = ptr->first;
        }
    }

    if(biggestIndex >= _attributes.size())
    {
        _attributes.resize(biggestIndex+1);
    }

    for(ptr=attr.begin(); ptr!=attr.end(); ptr++)
    {
        _attributes[ptr->first].first  = vertexBuffer;
        _attributes[ptr->first].second = ptr->second;
    }

    return true;
}
/**
 * Add index buffer to vertex stream.
 */
bool VertexStream::add(IndexBuffer* indexBuffer)
{
    _elements = indexBuffer;
    return true;
}
/**
 * 
 */
bool VertexStream::compile()
{
    // Sanity check
    if(!_vao)
    {
        Log_Error(Dumb::Module::Render, "Invalid vertex array object id: %x\n", _vao);
        return false;
    }
    if(0 == _attributes.size())
    {
        Log_Error(Dumb::Module::Render, "Empty attributes.");
        return false;
    }

    // Validate attributes.
    for(size_t i=0; i<_attributes.size(); i++)
    {
        if(0 == _attributes[i].second.size)
        {
            Log_Error(Dumb::Module::Render, "Attributes %d is not set.", i);
            return false;
        }
    }

    bind();
    for(size_t i=0; i<_attributes.size(); i++)
    {
        glEnableVertexAttribArray(i);
        _attributes[i].first->bind();
        _attributes[i].second.attach(i);
    }
    if(nullptr != _elements)
    {
        _elements->bind();
    }
    unbind();
    VertexBuffer::unbindAll();
    IndexBuffer::unbindAll();
    for(size_t i=0; i<_attributes.size(); i++)
    {
        glDisableVertexAttribArray(i);
    }
    GLenum err = glGetError();
    if(GL_NO_ERROR != err)
    {
        Log_Error(Dumb::Module::Render, "An error occured %d: %s.", err, gluErrorString(err));
        return false;
    }
    return true;
}

/**
 * Bind vertex stream.
 */
void VertexStream::bind() const
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    if(!_vao)
    {
        Log_Warning(Dumb::Module::Render, "You are trying to bind an invalid vertex stream!");
    }
    else
    {
        GLuint current;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&current);
        if(current && (current != _vao))
        {
            Log_Warning(Dumb::Module::Render, "Vertex stream %d is currently bound.", current);
        }
    }
#endif // SANITY_CHECK
   glBindVertexArray(_vao);
}
/**
 * Unbind vertex stream.
 */
void VertexStream::unbind() const
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLuint current;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&current);
    if(current != _vao)
    {
        if(_vao)
        {
            Log_Warning(Dumb::Module::Render, "You are trying to unbind vertex stream %d whereas the current bound vertex stream is %d", _vao, current);
            Log_Warning(Dumb::Module::Render, "If you really want to unbind the currently bound stream use the unbindAll() (static) method instead.");
        }
        else
        {
            Log_Warning(Dumb::Module::Render, "There is no vertex stream bound.");
        }
    }
#endif // SANITY_CHECK
   glBindVertexArray(0);
}
/**
 * Unbind any vertex stream.
 */
void VertexStream::unbindAll()
{
    glBindVertexArray(0);
}
/**
 * Check if the vertex stream is bound.
 * @return true if the vertex stream is bound.
 */
bool VertexStream::isBound() const
{
    GLuint current;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&current);
    return (current == _vao);
}
/** 
 * Draw primitives.
 * @note The vertex stream must have been bound.
 * @param [in] primitive Primitive type to be drawn.
 * @param [in] offset    Start offset in the vertex buffer.
 * @param [in] count     Number of primitives to draw.
 */
void VertexStream::draw(Geometry::Primitive primitive, size_t offset, size_t count) const
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLuint current;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&current);
    if(current != _vao)
    {
        if(_vao)
        {
            Log_Warning(Dumb::Module::Render, "You are trying to issue a draw call on an unbound vertex stream.");
        }
        else
        {
            Log_Warning(Dumb::Module::Render, "There is no vertex stream bound.");
        }
    }
#endif // SANITY_CHECK
    glDrawArrays(primitive.to(), offset, count);

#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLenum err = glGetError();
    if(GL_NO_ERROR != err)
    {
        Log_Error(Dumb::Module::Render, "An error occured %d: %s", _vao, gluErrorString(err));
    }
#endif // SANITY_CHECK
}
/**
 * Check if the attribute at the specified index was set.
 * @param [in] index Attribute index.
 * @return true if the attribute is set.
 */
bool VertexStream::isAttributeSet(unsigned int index) const
{
    if(index >= _attributes.size())
    {
        return false;
    }
    if(0 == _attributes[index].second.size)
    {
        return false;
    }
    return true;
}
/**
 * Get the vertex buffer for a given attribute.
 * @param [in] index Attribute index.
 * @return Constant pointer to the vertex buffer.
 */
VertexBuffer const* VertexStream::getVertexBuffer(unsigned int index) const
{
    if(index >= _attributes.size())
    {
        return nullptr;
    }
    return _attributes[index].first;
}
/**
 * Get index buffer.
 */
IndexBuffer const* VertexStream::getIndexBuffer() const
{
    return _elements;
}

} // Render
} // Dumb
