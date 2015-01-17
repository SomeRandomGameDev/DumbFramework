#ifndef _DUMB_FW_UNIFORM_BUFFER_
#define _DUMB_FW_UNIFORM_BUFFER_

#include <DumbFramework/config.hpp>
#include <DumbFramework/bufferobject.hpp>

namespace Framework {

/**
 * @brief Uniform buffer.
 * @ingroup DUMB_FW_BUFFER_OBJECT
 * @see Framework::BufferObject::Detail
 * [todo] description
 */
typedef BufferObject::Detail<BufferObject::UNIFORM_BUFFER> UniformBuffer;

} // Framework

#endif /* _DUMB_FW_UNIFORM_BUFFER_ */
