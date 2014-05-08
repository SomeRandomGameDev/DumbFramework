#include "log.hpp"
#include "bufferobject.hpp"

namespace Render {

    /**
     * Constructor.
     */
    BufferObject::BufferObject()
        : _id(0)
          , _target(GL_NONE)
          , _size(0)
          , _usage(0)
    {}
    /**
     * Destructor.
     */
    BufferObject::~BufferObject()
    {
        destroy();
    }
    /**
     * Create buffer object.
     * @param [in] target Target buffer type.
     * @param [in] size   Size in bytes of the buffer object.
     * @param [in] usage  Expected usage of the buffer object data
     * @param [in] data   Pointer to the data that will be copied. (Optional. Default value is NULL). 
     * @return true if the buffer object was succesfully created.
     */
    bool BufferObject::create( GLint target, GLsizei size, GLenum usage, const GLvoid *data )
    {
        GLenum err;
        if(_id)
        {
            Log_Warning(ModuleID::RENDER, "Destroying buffer object %d!", _id);
            destroy();
        }

        _target = target;
        _size   = size;
        _usage  = usage;

        glGenBuffers(1, &_id);
        glBindBuffer(_target, _id);
        glBufferData(_target, _size, data, GL_STATIC_DRAW);

        err = glGetError();

        glBindBuffer(_target, 0);

        if(err != GL_NO_ERROR)
        {
            Log_Warning(ModuleID::RENDER, "An error occured while creating buffer object %d: %s", _id, gluErrorString(err));
            return false;
        }

        return true;
    }

    /**
     * Destroy buffer object.
     */
    void BufferObject::destroy()
    {
        if(_id)
        {
            unbind();
            glDeleteBuffers(1, &_id);
            _id = 0;
        }
    }

    /**
     * Bind buffer.
     */
    void BufferObject::bind() const
    {
        glBindBuffer(_target, _id);
    }

    /**
     * Bind buffer to an indexed buffer object.
     * This can be used to bind a buffer to a uniform block or for transform feedback.
     * @param [in] target Target buffer type (GL_TRANSFORM_FEEDBACK_BUFFER or GL_UNIFORM_BUFFER).
     * @param [in] index  Binding point index (uniform id or transform feedback).
     */
    void BufferObject::bindBase( GLenum bindTarget, GLuint index ) const
    {
        glBindBufferBase(bindTarget, index, _id);
    }

    /**
     * Bind buffer to an indexed buffer object.
     * This can be used to bind a buffer to a uniform block or for transform feedback.
     * @param [in] target Target buffer type (GL_TRANSFORM_FEEDBACK_BUFFER or GL_UNIFORM_BUFFER).
     * @param [in] index  Binding point index (uniform id or transform feedback).
     * @param [in] offset Offset in the buffer object.
     * @param [in] size   Size of the buffer area to be read.
     */
    void BufferObject::bindRange( GLenum bindTarget, GLuint index, GLintptr offset, GLsizeiptr size ) const
    {
        glBindBufferRange( bindTarget, index, _id, offset, size );
    }

    /**
     * Unbind bufer object.
     */
    void BufferObject::unbind() const
    {
        GLuint boundId;
        GLenum binding;

        switch(_target)
        {
            case GL_ARRAY_BUFFER:
                binding = GL_ARRAY_BUFFER_BINDING;
                break;
            case GL_ELEMENT_ARRAY_BUFFER:
                binding = GL_ELEMENT_ARRAY_BUFFER_BINDING;
                break;
            case GL_TEXTURE_BUFFER:
                binding = GL_TEXTURE_BINDING_BUFFER;
                break;
            case GL_PIXEL_PACK_BUFFER:
                binding = GL_PIXEL_PACK_BUFFER_BINDING;
                break;
            case GL_PIXEL_UNPACK_BUFFER:
                binding = GL_PIXEL_UNPACK_BUFFER_BINDING;
                break;
            default:
                glBindBuffer(_target, 0);
                return;
        };
        glGetIntegerv(binding, (GLint*)&boundId);

        if(boundId != _id) return;
        glBindBuffer(_target, 0);
    }

    /**
     * Set buffer data.
     * @param [in]  offset Offset where the data will be copied in the buffser object.
     * @param [in]  size   Size of the data to be copied.
     * @param [in]  data   Pointer to the data to be copied.
     * @param [out] true if the data was succesfully copied.
     */
    bool BufferObject::data( GLintptr offset, GLsizeiptr size, const GLvoid* data )
    {
        if(offset >= _size)
        {
            Log_Error(ModuleID::RENDER, "Offset (%d) out of bound (vbo size: %d)", offset, _size);
            return false;
        }

        if((offset+size) > _size)
        {
            Log_Error(ModuleID::RENDER, "Buffer overflow (offset: %d, size: %d, vbo size: %d)", offset, size, _size);
            return false;
        }

        glBindBuffer(_target, _id);
        glBufferSubData(_target, offset, size, data);
        glBindBuffer(_target, 0);

        return true;
    }

    /**
     * Map buffer area.
     * @param [in] mode   Mapping mode (read or write).
     */
    GLvoid* BufferObject::map(MappingMode mode)
    {
        GLvoid* ptr;

        glBindBuffer(_target, _id);
        ptr = glMapBuffer(_target, ((mode == BUFFER_READ) ? GL_READ_ONLY : GL_WRITE_ONLY) | GL_MAP_UNSYNCHRONIZED_BIT);

        if(ptr == NULL)
        {
            GLenum err = glGetError();
            Log_Error(ModuleID::RENDER, "An error occured while mapping buffer object %d: %s", _id, gluErrorString(err));
            return 0;
        }

        return ptr;
    }

    /**
     * Map buffer area.
     * @param [in] mode   Mapping mode (read or write).
     * @param [in] offset Offset of the area to be mapped.
     * @param [in] size   Size of the area to be mapped.
     */
    GLvoid* BufferObject::mapRange(MappingMode mode, GLintptr offset, GLsizei size)
    {
        GLvoid* ptr;

        glBindBuffer(_target, _id);
        ptr = glMapBufferRange(_target, offset, size, ((mode == BUFFER_READ) ? GL_MAP_READ_BIT : (GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT)) | GL_MAP_UNSYNCHRONIZED_BIT);

        if(ptr == NULL)
        {
            GLenum err = glGetError();
            Log_Error(ModuleID::RENDER, "An error occured while mapping buffer object %d: %s", _id, gluErrorString(err));
            return 0;
        }

        return ptr;
    }

    /**
     * Unmap buffer.
     */
    void BufferObject::unmap()
    {
        glUnmapBuffer(_target);
    }

}
