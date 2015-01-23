#ifndef _DUMB_FW_BUFFER_OBJECT_
#define _DUMB_FW_BUFFER_OBJECT_

#include <DumbFramework/config.hpp>
#include <DumbFramework/log.hpp>

namespace Framework    {
namespace Render       {
/**
 * @defgroup DUMB_FW_BUFFER_OBJECT Buffer object management.
 * @ingroup DUMB_FW_RENDERING
 * GPU buffer (vertex, index, pixel, ...) management.
 */
namespace BufferObject {
/**
 * @defgroup DUMB_FW_BUFFER_OBJECT_ACCESS Buffer object access usage.
 * @ingroup DUMB_FW_BUFFER_OBJECT
 * 
 */
namespace Access {
/**
 * @brief Buffer object access type.
 * This structure specifies the type of access to buffer object storage.
 *      - Type::DRAW : The data store contents are modified by the
 *                     application, and used as the source for graphic
 *                     library drawing and image specification commands. 
 *      - Type::READ : The data store contents are modified by reading
 *                     data from the graphic library, and used to return
 *                     that data when queried by the application. 
 *      - Type::COPY : The data store contents are modified by reading
 *                     data from the graphic library, and used as the
 *                     source for graphic library drawing and image
 *                     specification commands.
 * @ingroup DUMB_FW_BUFFER_OBJECT_ACCESS
 */
struct Type
{
    /** Buffer object access type. **/
    enum Value
    {
        /** The data store contents are modified by the application, and
         *  used as the source for graphic library drawing and image
         * specification commands. **/
        DRAW,
        /** The data store contents are modified by reading data from
         *  the graphic library, and used to return that data when 
         *  queried by the application. **/
        READ,
        /** The data store contents are modified by reading data from
         *  the graphic library, and used as the source for graphic
         *  library drawing and image specification commands. **/
        COPY
    };
    Value value; /**< Buffer object access type. **/
    /** 
     * @brief Default constructor.
     * By default the buffer object access type is @c Type::DRAW.
     */
    inline Type() : value(Type::DRAW) {}
    /**
     * @brief Constructor.
     * @param [in] v  Buffer object access frequency.
     */
    inline Type(Value v) : value(v) {}
    inline operator Value() { return value; }
};
/**
 * @brief Buffer object access frequency.
 * This structure specifies the frequency of access to buffer object
 * storage.
 *      - Frequency::STREAM  : the data store contents will be modified
 *                             once and used at most a few times. 
 *      - Frequency::STATIC  : the data store contents will be modified
 *                             once and used many times. 
 *      - Frequency::DYNAMIC : the data store contents will be modified
 *                             repeatedly and used many times.
 * @ingroup DUMB_FW_BUFFER_OBJECT_ACCESS
 */
struct Frequency
{
    /** Buffer object access frequency. **/
    enum Value
    {
        /** The data store contents will be modified once and used at
         *  most a few times. **/
        STREAM,
        /** The data store contents will be modified once and used many
         *  times. **/
        STATIC,
        /** The data store contents will be modified repeatedly and used
         *  many times. **/
        DYNAMIC
    };
    Value value; /**< Buffer object access frequency. **/
    /** 
     * @brief Default constructor.
     * By default the buffer object access frequency is @c Frequency::STATIC.
     */
    inline Frequency() : value(Frequency::STATIC) {}
    /**
     * @brief Constructor.
     * @param [in] v  Buffer object access frequency.
     */
    inline Frequency(Value v) : value(v) {}
    inline operator Value() { return value; }
};
/**
 * @brief Buffer object access policy.
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
struct Policy
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
     * By default the buffer object access is @c Policy::READ_ONLY.
     */
    inline Policy() : value(Policy::READ_ONLY) {}
    /**
     * @brief Constructor.
     * @param [in] v  Buffer object access value.
     */
    inline Policy(Value v) : value(v) {}
    inline operator Value() { return value; }
    inline operator GLenum();
};

/**
 * Convert access frequency and type to buffer data usage.
 * @param [in] freq    Access frequency.
 * @param [in] type    Access type.
 * @return OpenGL buffer usage.
 */
GLenum usage(Frequency freq, Type type);

} // Access

/**
 * @brief Buffer object type.
 */
enum Type
{
    /** Vertex buffer. **/
    VERTEX_BUFFER,
    /** Index buffer. **/
    INDEX_BUFFER,
    /** Uniform buffer. **/
    UNIFORM_BUFFER
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
         * @param [in] freq  Access frequency (default = Access::Frequency::STATIC).
         * @param [in] type  Access type (default = Access::Type::DRAW).
         * @return true if the buffer was successfully created.
         */
        bool create(size_t size, void* data = NULL, Access::Frequency freq = Access::Frequency::STATIC, Access::Type type = Access::Type::DRAW);
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
         * Resize buffer.
         * @param [in] size    Buffer size in bytes.
         */
        void resize(size_t size);
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
         * @param [in] policy  Data storage access policy.
         * @return Pointer to the buffer data storage or NULL if an
         *         error occured.
         */ 
        void* map(BufferObject::Access::Policy policy) const;
        /**
         * Map only a given area of the buffer data storage.
         * @param [in] access  Data storage access policy.
         * @param [in] offset  Starting offset in the buffer data
         *                     storage.
         * @param [in] length  Number of bytes to be mapped.
         * @return Pointer to the buffer data storage or NULL if an
         *         error occured.
         */
        void* map(BufferObject::Access::Policy access, off_t offset, size_t length) const;
        /**
         * Unmap buffer.
         * The pointer previously returned by Detail::map will become 
         * invalid.
         * @return true if the buffer was successfully unmapped.
         */
        bool unmap() const;
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
        /**
         * Get access frequency.
         */
        Access::Frequency accessFrequency() const;
        /**
         * Get access type.
         */
        Access::Type accessType() const;
        
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
        Access::Frequency _freq; /**< Access frequency. **/
        Access::Type      _type; /**< Access type. **/
        GLenum _usage; /**< Buffer usage. **/
};

} // BufferObject
} // Render
} // Framework

#include "bufferobject.inl"

#endif /* _DUMB_FW_BUFFER_OBJECT_ */
