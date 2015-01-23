#ifndef _DUMB_FW_INDEX_BUFFER_
#define _DUMB_FW_INDEX_BUFFER_

#include <DumbFramework/config.hpp>
#include <DumbFramework/render/bufferobject.hpp>

namespace Framework {
namespace Render    {
/**
 * @brief Index buffer.
 * @ingroup DUMB_FW_BUFFER_OBJECT
 * @see Framework::BufferObject::Detail
 * An index buffer - as its name implies - is a buffer containing
 * indices. This indices may be used to fetch data from vertex buffers
 * in order to construct geometric primitives.
 */
typedef BufferObject::Detail<BufferObject::INDEX_BUFFER> IndexBuffer;

} // Render
} // Framework

#endif /* _DUMB_FW_INDEX_BUFFER_ */
