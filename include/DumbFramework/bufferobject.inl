namespace Framework    {
namespace BufferObject {

/** Convert to OpenGL compliant value. **/
Access::Policy::operator GLenum()
{
    switch(value)
    {
        case READ_ONLY:
            return GL_READ_ONLY;
        case WRITE_ONLY:
            return GL_WRITE_ONLY;
        case READ_WRITE:
            return GL_READ_WRITE;
        default:
            return GL_READ_ONLY;
    }
}

/** Constructor. **/
template <Type t>
Detail<t>::Detail()
    : _id(0)
    , _size(0)
{}
/**Destructor. **/
template <Type t>
Detail<t>::~Detail()
{}
/**
 * Create a buffer.
 * @param [in] size  Size in bytes of the buffer data storage.
 * @param [in] data  Pointer to the data that will be copied to
 *                   buffer data storage (default = NULL).
 * @param [in] freq  Access frequency (default = Access::Frequency::STATIC).
 * @param [in] type  Access type (default = Access::Type::DRAW).
 * @return true if the buffer was successfully created.
 */
template <Type t>
bool Detail<t>::create(size_t size, void* data, Access::Frequency freq, Access::Type type)
{
    if(_id)
    {
        destroy();
    }
    
    GLenum access;
    static const GLenum conversionMatrix[3][3] =
    {
        // Access::Frequency::STREAM
        {GL_STREAM_DRAW,  GL_STREAM_READ,  GL_STREAM_COPY},
        // Access::Frequency::STATIC
        {GL_STATIC_DRAW,  GL_STATIC_READ,  GL_STATIC_COPY},
        // Access::Frequency::DYNAMIC
        {GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY}
    };
    access = conversionMatrix[freq.value][type.value];
    
    _size = size;
    
    glGenBuffers(1, &_id);
    glBindBuffer(_infos.target, _id);
    glBufferData(_infos.target, _size, data, access);

    GLenum err = glGetError();

    glBindBuffer(_infos.target, 0);

    if(GL_NO_ERROR != err)
    {
        Log_Error(Framework::Module::Render, "An error occured while creating buffer object %d: %s", _id, gluErrorString(err));
        return false;
    }
    return true;
}
/**
 * Destroy buffer.
 */
template <Type t>
void Detail<t>::destroy()
{
    if(_id)
    {
        glDeleteBuffers(1, &_id);
        _id = 0;
    }
    _size = 0;
}
/**
 * Copy data to buffer.
 * @param [in] offset Offset in the buffer data storage where
 *                    the data will be copied.
 * @param [in] size   Number of bytes to copy.
 * @param [in] data   Pointer to the data to be copied.
 * @return true if the data was successfully copied. It may fail
 *         if the buffer is in an invalid state or if the
 *         offset and size are out of bound.
 */
template <Type t>
bool Detail<t>::set(off_t offset, size_t size, void* data)
{
    if((size_t)offset >= _size)
    {
        Log_Error(Framework::Module::Render, "Offset (%d) out of bound (buffer size: %d)", offset, _size);
        return false;
    }

    if((offset+size) > _size)
    {
        Log_Error(Framework::Module::Render, "Buffer overflow (offset: %d, size: %d, buffer size: %d)", offset, size, _size);
        return false;
    }
    glBufferSubData(_infos.target, offset, size, data);
    return true;
}
/**
 * Bind buffer.
 */
template <Type t>
void Detail<t>::bind() const
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    if(!isValid())
    {
        Log_Warning(Module::Render, "You are trying to bind an invalid %s buffer!", _infos.name);
    }
    else
    {
        GLuint bufferId;
        glGetIntegerv(_infos.query, (GLint*)&bufferId);
        if(bufferId && (bufferId != _id))
        {
            Log_Warning(Module::Render, "%s buffer %d is currently bound.", _infos.name);
        }
    }
#endif // SANITY_CHECK
   glBindBuffer(_infos.target, _id);
}
/**
 * Unbind buffer.
 */
template <Type t>
void Detail<t>::unbind() const
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    GLuint bufferId;
    glGetIntegerv(_infos.query, (GLint*)&bufferId);
    if(bufferId != _id)
    {
        if(_id)
        {
            Log_Warning(Module::Render, "You are trying to unbind buffer %d whereas the current bound %s buffer is %d", _id, _infos.name, bufferId);
            Log_Warning(Module::Render, "If you really want to unbind the currently bound buffer use the unbindAll() (static) method instead.");
        }
        else
        {
            Log_Warning(Module::Render, "No %s buffer was bound.", _infos.name);
        }
    }
#endif // SANITY_CHECK
   glBindBuffer(_infos.target, 0);
}
/**
 * Check if the buffer is bound.
 * @return true if the buffer is bound.
 */
template <Type t>
bool Detail<t>::isBound() const
{
    GLuint bufferId;
    glGetIntegerv(_infos.query, (GLint*)&bufferId);
    return (bufferId == _id);
}
/**
 * Unbind currently bound buffers.
 */
template <Type t>
void Detail<t>::unbindAll()
{
   glBindBuffer(_infos.target, 0);
}
/**
 * Map buffer data storage.
 * @param [in] policy  Data storage access policy.
 * @return Pointer to the buffer data storage or NULL if an
 *         error occured.
 */
template <Type t>
void* Detail<t>::map(BufferObject::Access::Policy policy)
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    if(isMapped())
    {
        Log_Warning(Framework::Module::Render, "The buffer is alread mapped. Current mapping operation will fail!");
    }
#endif // SANITY_CHECK

    GLvoid* ptr;
    bind();
    ptr = glMapBuffer(_infos.target, GL_WRITE_ONLY);
    if(NULL == ptr)
    {
        GLenum err = glGetError();
        Log_Error(Framework::Module::Render, "An error occured while mapping buffer %d: %s", _id, gluErrorString(err));
    }
    return ptr;
}
/**
 * Map only a given area of the buffer data storage.
 * @param [in] policy  Data storage access policy.
 * @param [in] offset  Starting offset in the buffer data
 *                     storage.
 * @param [in] length  Number of bytes to be mapped.
 * @return Pointer to the buffer data storage or NULL if an
 *         error occured.
 */
template <Type t>
void* Detail<t>::map(BufferObject::Access::Policy policy, off_t offset, size_t length)
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    if(isMapped())
    {
        Log_Warning(Framework::Module::Render, "The buffer is alread mapped. Current mapping operation will fail!");
    }
#endif // SANITY_CHECK
    bind();
    
    GLbitfield access;
    switch(policy.value)
    {
        case BufferObject::Access::Policy::READ_ONLY:
            access = GL_MAP_READ_BIT;
            break;
        case BufferObject::Access::Policy::WRITE_ONLY:
            access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
            break;
        case BufferObject::Access::Policy::READ_WRITE:
            access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
            break;
    }
    
    GLvoid* ptr;
    ptr = glMapBufferRange(_infos.target, offset, length, access);
    if(NULL == ptr)
    {
        GLenum err = glGetError();
        Log_Error(Framework::Module::Render, "An error occured while mapping buffer %d: %s", _id, gluErrorString(err));
    }
    return static_cast<void*>(ptr);
}
/**
 * Unmap buffer.
 * The pointer previously returned by Detail::map will become 
 * invalid.
 * @return true if the buffer was successfully unmapped.
 */
template <Type t>
bool Detail<t>::unmap()
{
#if defined(SANITY_CHECK)
    // Warning! This may spam your logs!
    if(!isMapped())
    {
        Log_Warning(Framework::Module::Render, "The buffer was not mapped. Unmapping will fail!");
    }
    bind();
#endif // SANITY_CHECK

    GLboolean ret;
    ret = glUnmapBuffer(_infos.target);
    if(GL_FALSE == ret)
    {
        GLenum err = glGetError();
        Log_Error(Framework::Module::Render, "An error occured while unmapping buffer %d: %s", _id, gluErrorString(err));
        return false;
    }
    unbind();
    return true;
}
/**
 * Tells if the buffer is currently mapped.
 * @return true if the buffer is mapped.
 */
template <Type t>
bool Detail<t>::isMapped() const
{
    GLint ret;
    bind();
        glGetBufferParameteriv(_infos.target, GL_BUFFER_MAPPED, &ret);
    unbind();
    return (GL_TRUE == ret);
}
/**
 * Tell if the buffer is valid.
 * @return true if the buffer is valid.
 */
template <Type t>
bool Detail<t>::isValid() const
{
    return (0 != _id);
}
/**
 * Get buffer size.
 * @return size in bytes.
 */
template <Type t>
size_t Detail<t>::size() const
{
    return _size;
}

} // BufferObject
} // Framework

