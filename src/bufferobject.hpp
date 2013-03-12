#ifndef _DUMB_FW_BUFFER_OBJECT_
#define _DUMB_FW_BUFFER_OBJECT_

#include <GL/glew.h>
#include <GL/glfw.h>

#include <config.hpp>
#include <log.hpp>

/**
 * Buffer object.
 */
class BufferObject
{
	public:
		/**
		 * Constructor.
		 */
		BufferObject();
		/**
		 * Destructor.
		 */
		~BufferObject();

		/**
		 * Create buffer object.
		 * @param [in] target Target buffer type.
		 * @param [in] size   Size in bytes of the buffer object.
		 * @param [in] usage  Expected usage of the buffer object data
		 * @param [in] data   Pointer to the data that will be copied. (Optional. Default value is NULL). 
		 * @return true if the buffer object was succesfully created.
		 */
		bool create( GLint target, GLsizei size, GLenum usage, const GLvoid *data = NULL );
		/**
		 * Destroy buffer object.
		 */
		void destroy();
		
		/**
		 * Bind buffer.
		 */
		void bind() const;
		/**
		 * Bind buffer to an indexed buffer object.
		 * This can be used to bind a buffer to a uniform block or for transform feedback.
		 * @param [in] target Target buffer type (GL_TRANSFORM_FEEDBACK_BUFFER or GL_UNIFORM_BUFFER).
		 * @param [in] index  Binding point index (uniform id or transform feedback).
		 */
		void bindBase( GLenum bindTarget, GLuint index ) const;
		/**
		 * Bind buffer to an indexed buffer object.
		 * This can be used to bind a buffer to a uniform block or for transform feedback.
		 * @param [in] target Target buffer type (GL_TRANSFORM_FEEDBACK_BUFFER or GL_UNIFORM_BUFFER).
		 * @param [in] index  Binding point index (uniform id or transform feedback).
		 * @param [in] offset Offset in the buffer object.
		 * @param [in] size   Size of the buffer area to be read.
		 */
		void bindRange( GLenum bindTarget, GLuint index, GLintptr offset, GLsizeiptr size ) const;
		/**
		 * Unbind bufer object.
		 */
		void unbind() const;
		/**
		 * Utility function to transate offset into pointer.
		 */
		static inline const GLvoid* offset(GLint off);
		/**
		 * Set buffer data.
		 * @param [in]  offset Offset where the data will be copied in the buffser object.
		 * @param [in]  size   Size of the data to be copied.
		 * @param [in]  data   Pointer to the data to be copied.
		 * @param [out] true if the data was succesfully copied.
		 */
		bool data( GLintptr offset, GLsizeiptr size, const GLvoid* data );

		enum MappingMode
		{
			BUFFER_READ,
			BUFFER_WRITE
		};
		/**
		 * Map buffer area.
		 * @param [in] mode   Mapping mode (read or write).
		 */
		GLvoid* map(MappingMode mode);
		/**
		 * Map buffer area.
		 * @param [in] mode   Mapping mode (read or write).
		 * @param [in] offset Offset of the area to be mapped.
		 * @param [in] size   Size of the area to be mapped.
		 */
		GLvoid* mapRange(MappingMode mode, GLintptr offset, GLsizei size);
		/**
		 * Unmap buffer.
		 */
		static void unmap();

		GLuint id    () const { return _id;     }
		GLint  target() const { return _target; }
		GLint  size  () const { return _size;   }
	protected:
		/**
		 * Buffer id.
		 */
		GLuint  _id;
		/**
		 * Buffer target. Supported values are :
		 *		GL_ARRAY_BUFFER
		 *		GL_COPY_READ_BUFFER
		 *		GL_COPY_WRITE_BUFFER
		 *		GL_ELEMENT_ARRAY_BUFFER
		 *		GL_PIXEL_PACK_BUFFER
		 *		GL_PIXEL_UNPACK_BUFFER
		 *		GL_TEXTURE_BUFFER
		 *		GL_TRANSFORM_FEEDBACK_BUFFER
		 *		GL_UNIFORM_BUFFER
		 */
		GLint   _target;
		/**
		 * Size in bytes of the buffer object.
		 */
		GLint   _size;
		/**
		 * Expected usage of the buffer object data.
		 * Supported values are:
		 *		GL_STREAM_DRAW,  GL_STREAM_READ,  GL_STREAM_COPY 
		 *		GL_STATIC_DRAW,  GL_STATIC_READ,  GL_STATIC_COPY 
		 *		GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COP
		 */
		GLenum  _usage;
};

/**
	* Utility function to transate offset into pointer.
	*/
inline const GLvoid* BufferObject::offset(GLint off)
{
	return (char*)NULL + off;
}

#endif /* _DUMB_FW_BUFFER_OBJECT_ */