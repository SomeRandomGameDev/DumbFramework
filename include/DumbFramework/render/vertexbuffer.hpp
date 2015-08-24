/*
 * Copyright 2015 MooZ
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _DUMB_FW_VERTEX_BUFFER_
#define _DUMB_FW_VERTEX_BUFFER_

#include <DumbFramework/config.hpp>
#include <DumbFramework/render/bufferobject.hpp>

namespace Dumb   {
namespace Render {

/**
 * @brief Vertex buffer.
 * @ingroup DUMB_FW_BUFFER_OBJECT
 * @see Dumb::BufferObject::Detail
 */
typedef BufferObject::Detail<BufferObject::VERTEX_BUFFER> VertexBuffer;

} // Render
} // Dumb

#endif /* _DUMB_FW_VERTEX_BUFFER_ */

