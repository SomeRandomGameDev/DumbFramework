#include <DumbFramework/log.hpp>
#include <DumbFramework/vertexstream.hpp>

namespace Framework {

/**
 * Constructor.
 */
VertexStream::VertexStream()
    : _vao()
    , _vertexBuffer(NULL)
    , _attributes()
{}
/**
 * Destructor.
 */
VertexStream::~VertexStream()
{}
/**
 * Create vertex stream.
 * @param [in] vertexBuffer Vertex buffer that will be associated
 *                          with the current vertex stream.
 * @return true if the vertex stream was succesfully created.
 */
bool VertexStream::create(VertexBuffer* vertexBuffer)
{
    if(_vao)
    {
        destroy();
    }
    
    if(NULL == vertexBuffer)
    {
        Log_Error(Framework::Module::Render, "Invalid vertex buffer.");
        return false;
    }
    
    _vertexBuffer = vertexBuffer;

    glGenVertexArrays(1, &_vao);
    GLenum err = glGetError();
    if(GL_NONE != err)
    {
        Log_Error(Framework::Module::Render, "An error occured while creating vertex array object: %s", gluErrorString(err));
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

    _vertexBuffer = NULL;
    _attributes.clear();
}
/**
 * Add attribute to vertex stream.
 * @param [in] index   Attribute index (starts at 0).
 * @param [in] type    Type of the attribute components.
 * @param [in] size    Number of components.
 * @param [in] stride  Number of bytes between 2 consecutives attributes.
 * @param [in] offset  Offset of the first attribute of the first vertex.
 * @param [in] divisor (default=0).
 * @return true if the attribute was successfully set.
 */
bool VertexStream::set(unsigned int index, Geometry::ComponentType type, size_t size, size_t stride, size_t offset, unsigned int divisor)
{
    return set(Geometry::Attribute(index, type, size, stride, offset, divisor));
}
/**
 * Add attribute to vertex stream.
 * @param [in] attr   Attribute.
 * @return true if the attribute was successfully set.
 */
bool VertexStream::set(Geometry::Attribute const& attr)
{
    if(attr.index >= _attributes.size())
    {
        _attributes.resize(attr.index+1);
    }
    _attributes[attr.index] = attr;
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
        Log_Error(Framework::Module::Render, "Invalid vertex array object id: %x\n", _vao);
        return false;
    }
    if(NULL == _vertexBuffer)
    {
        Log_Error(Framework::Module::Render, "Invalid vertex buffer.");
        return false;
    }
    if(0 == _attributes.size())
    {
        Log_Error(Framework::Module::Render, "Empty attributes.");
        return false;
    }

    // Validate attributes.
    for(size_t i=0; i<_attributes.size(); i++)
    {
        if(0 == _attributes[i].size)
        {
            Log_Error(Framework::Module::Render, "Attributes %d is not set.", i);
            return false;
        }
    }

    bind();
    for(size_t i=0; i<_attributes.size(); i++)
    {
        glEnableVertexAttribArray(i);
    }
    _vertexBuffer->bind();
    for(size_t i=0; i<_attributes.size(); i++)
    {
        glVertexAttribPointer(i, _attributes[i].size, _attributes[i].type, GL_FALSE, _attributes[i].stride, (GLvoid*)_attributes[i].offset);
        glVertexAttribDivisor(i, _attributes[i].divisor);
    }
    _vertexBuffer->unbind();
    unbind();
    
    GLenum err = glGetError();
    if(GL_NO_ERROR != err)
    {
        Log_Error(Framework::Module::Render, "An error occured %d: %s.", err, gluErrorString(err));
        return false;
    }
    return true;
}
/**
 * Build vertex stream.
 * This method is a combination of VertexStream::create,
 * VertexStream::set and VertexStream::compile.
 * @param [in] vertexBuffer Vertex buffer that will be associated
 *                          with the current vertex stream.
 * @param [in] attr         A list of vertex attributes.
 * @return true if the vertex stream was successfully built.
 */
bool VertexStream::build(VertexBuffer* vertexBuffer, std::initializer_list<Geometry::Attribute> const& attr)
{
    bool ret;
    
    if(false == _attributes.empty())
    {
        Log_Warning(Framework::Module::Render, "Flusing attributes.");
        _attributes.clear();
    }
    
    ret = create(vertexBuffer);
    if(false == ret)
    {
        Log_Error(Framework::Module::Render, "Vertex stream creation failed.");
        return false;
    }
    
    _attributes.insert(_attributes.end(), attr);
    
    return compile();
}
/**
 * Bind vertex stream.
 */
void VertexStream::bind()
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    if(!_vao)
    {
        Log_Warning(Module::Render, "You are trying to bind an invalid vertex stream!");
    }
    else
    {
        GLuint current;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&current);
        if(current && (current != _vao))
        {
            Log_Warning(Module::Render, "Vertex stream %d is currently bound.", current);
        }
    }
#endif // SANITY_CHECK
   glBindVertexArray(_vao);
}
/**
 * Unbind vertex stream.
 */
void VertexStream::unbind()
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLuint current;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&current);
    if(current != _vao)
    {
        if(_vao)
        {
            Log_Warning(Module::Render, "You are trying to unbind vertex stream %d whereas the current bound vertex stream is %d", _vao, current);
            Log_Warning(Module::Render, "If you really want to unbind the currently bound stream use the unbindAll() (static) method instead.");
        }
        else
        {
            Log_Warning(Module::Render, "There is no vertex stream bound.");
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
            Log_Warning(Module::Render, "You are trying to issue a draw call on an unbound vertex stream.");
        }
        else
        {
            Log_Warning(Module::Render, "There is no vertex stream bound.");
        }
    }
#endif // SANITY_CHECK
    glDrawArrays(primitive, offset, count);

#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLenum err = glGetError();
    if(GL_NO_ERROR != err)
    {
        Log_Error(Framework::Module::Render, "An error occured %d: %s", _vao, gluErrorString(err));
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
    if(0 == _attributes[index].size)
    {
        return false;
    }
    return true;
}
/**
 * Get the vertex buffer.
 * @return Pointer to the vertex buffer.
 */
VertexBuffer* VertexStream::getVertexBuffer()
{
    return _vertexBuffer;
}
/**
 * Get the vertex buffer.
 * @return Constant pointer to the vertex buffer.
 */
VertexBuffer const* VertexStream::getVertexBuffer() const
{
    return _vertexBuffer;
}

} // Framework
