#ifndef _DUMB_FW_VERTEX_BUFFER_
#define _DUMB_FW_VERTEX_BUFFER_

#include <DumbFramework/config.hpp>
#include <DumbFramework/render/bufferobject.hpp>

namespace Framework {
namespace Render    {

/**
 * @brief Vertex buffer.
 * @ingroup DUMB_FW_BUFFER_OBJECT
 * @see Framework::BufferObject::Detail
 */
typedef BufferObject::Detail<BufferObject::VERTEX_BUFFER> VertexBuffer;

} // Render
} // Framework

#endif /* _DUMB_FW_VERTEX_BUFFER_ */

