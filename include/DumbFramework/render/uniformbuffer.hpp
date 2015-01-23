#ifndef _DUMB_FW_UNIFORM_BUFFER_
#define _DUMB_FW_UNIFORM_BUFFER_

#include <DumbFramework/config.hpp>
#include <DumbFramework/render/bufferobject.hpp>

namespace Framework {
namespace Render    {

/**
 * @brief Uniform buffer.
 * @ingroup DUMB_FW_BUFFER_OBJECT
 * @see Framework::BufferObject::Detail
 * [todo] description
 */
typedef BufferObject::Detail<BufferObject::UNIFORM_BUFFER> UniformBuffer;

} // Render
} // Framework

#endif /* _DUMB_FW_UNIFORM_BUFFER_ */
