#ifndef _DUMB_FW_VERTEX_BUFFER_
#define _DUMB_FW_VERTEX_BUFFER_

#include <DumbFramework/config.hpp>
#include <DumbFramework/bufferobject.hpp>

namespace Framework {
/**
 * @brief Vertex buffer.
 * @ingroup DUMB_FW_BUFFER_OBJECT
 * @see Framework::BufferObject::Detail
 */
typedef BufferObject::Detail<BufferObject::VERTEX_BUFFER> VertexBuffer;

} // Framework

#endif /* _DUMB_FW_VERTEX_BUFFER_ */

