#ifndef _DUMB_FW_BUFFER_OBJECT_
#define _DUMB_FW_BUFFER_OBJECT_

#include <DumbFramework/config.hpp>
#include <DumbFramework/log.hpp>

namespace Framework    {
/**
 * @defgroup DUMB_FW_BUFFER_OBJECT Buffer object management.
 * @ingroup DUMB_FW_RENDERING
 * GPU buffer (vertex, index, pixel, ...) management.
 */
namespace BufferObject {
/**
 * @brief Buffer object access.
 * This structure specifies the access policy to mapped buffer 
 * object storage.
 *      - Access::READ_ONLY : it is only possible to @b read @b from
 *                            the mapped buffer object data.
 *      - Access::WRITE_ONLY : it is only possible to @b write @b to
 *                             the mapped buffer object data.
 *      - Access::READ_WRITE : it is possible to both @b read @b from 
 *                             and @b write @b to the mapped buffer
 *                             object data.
 * @ingroup DUMB_FW_BUFFER_OBJECT
 */
struct Access
{
    /** Buffer object access. **/
    enum Value
    {
        /** The buffer object data access is @b read-only. **/
        READ_ONLY,
        /** The buffer object data access is @b write-only. **/
        WRITE_ONLY,
        /** The buffer object data access is @b read/write. **/
        READ_WRITE
    };
    Value value; /**< Buffer object access value. **/
    /** 
     * @brief Default constructor.
     * By default the buffer object access is @c Access::READ_ONLY.
     */
    inline Access() : value(Access::READ_ONLY) {}
    /**
     * @brief Constructor.
     * @param [in] v  Buffer object access value.
     */
    inline Access(Value v) : value(v) {}
    inline operator Value() { return value; }
    /** Convert to OpenGL compliant value. **/
    inline operator GLenum();
};

/**
 * @brief Buffer object type.
 */
enum Type
{
    /** Vertex buffer. **/
    VERTEX_BUFFER,
    /** Index buffer. **/
    INDEX_BUFFER
};

/**
 * Template for buffer object.
 * @ingroup DUMB_FW_BUFFER_OBJECT 
 */
template<Type t>
class Detail
{
        public:
        /** Constructor. **/
        Detail();
        /**Destructor. **/
        ~Detail();
        /**
         * Create a buffer.
         * @param [in] size  Size in bytes of the buffer data storage.
         * @param [in] data  Pointer to the data that will be copied to
         *                   buffer data storage (default = NULL).
         * @return true if the buffer was successfully created.
         */
        bool create(size_t size, void* data = NULL);
        /**
         * Destroy buffer.
         */
        void destroy();
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
        bool set(off_t offset, size_t size, void* data);
        /**
         * Bind buffer.
         */
        void bind() const;
        /**
         * Unbind buffer.
         */
        void unbind() const;
        /**
         * Check if the buffer is bound.
         * @return true if the buffer is bound.
         */
        bool isBound() const;
        /**
         * Unbind currently bound buffers.
         */
        static void unbindAll();
        /**
         * Map buffer data storage.
         * @param [in] access  Data storage access policy.
         * @return Pointer to the buffer data storage or NULL if an
         *         error occured.
         */ 
        void* map(BufferObject::Access access);
        /**
         * Map only a given area of the buffer data storage.
         * @param [in] access  Data storage access policy.
         * @param [in] offset  Starting offset in the buffer data
         *                     storage.
         * @param [in] length  Number of bytes to be mapped.
         * @return Pointer to the buffer data storage or NULL if an
         *         error occured.
         */
        void* map(BufferObject::Access access, off_t offset, size_t length);
        /**
         * Unmap buffer.
         * The pointer previously returned by Detail::map will become 
         * invalid.
         * @return true if the buffer was successfully unmapped.
         */
        bool unmap();
        /**
         * Tells if the buffer is currently mapped.
         * @return true if the buffer is mapped.
         */
        bool isMapped() const;
        /**
         * Tell if the buffer is valid.
         * @return true if the buffer is valid.
         */
        bool isValid() const;
        /**
         * Get buffer size.
         * @return size in bytes.
         */
        size_t size() const;
    
    private:
        struct Infos
        {
            char const* name;   /**< Type name. **/
            GLenum      target; /**< Target. **/
            GLenum      query;  /**< Binding query. **/
        };
        static const Infos _infos; /** Buffer type info. **/
        
        GLuint _id;   /**< Buffer id. **/
        size_t _size; /**< Buffer size in bytes. **/
};

} // BufferObject
} // Framework

#include "bufferobject.inl"

#endif /* _DUMB_FW_BUFFER_OBJECT_ */
