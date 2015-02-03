#ifndef _DUMB_FW_TEXTURE_BUFFER_
#define _DUMB_FW_TEXTURE_BUFFER_

#include <DumbFramework/config.hpp>
#include <DumbFramework/render/bufferobject.hpp>

namespace Framework {
namespace Render    {

/**
 * @brief Texture buffer storage.
 * @ingroup DUMB_FW_BUFFER_OBJECT
 * @see Framework::BufferObject::Detail
 */
typedef BufferObject::Detail<BufferObject::TEXTURE_BUFFER> Storage;

} // Render
} // Framework

#endif /* _DUMB_FW_TEXTURE_BUFFER_ */
