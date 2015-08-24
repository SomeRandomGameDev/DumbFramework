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
#include <DumbFramework/render/bufferobject.hpp>

namespace Dumb         {
namespace Render       {
namespace BufferObject {

template<>
const BufferObject::Detail<BufferObject::VERTEX_BUFFER>::Infos
BufferObject::Detail<BufferObject::VERTEX_BUFFER>::_infos = 
{
    "vertex",
    GL_ARRAY_BUFFER,
    GL_ARRAY_BUFFER_BINDING
};

template<>
const BufferObject::Detail<BufferObject::INDEX_BUFFER>::Infos
BufferObject::Detail<BufferObject::INDEX_BUFFER>::_infos = 
{
    "index",
    GL_ELEMENT_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER_BINDING
};

template<>
const BufferObject::Detail<BufferObject::UNIFORM_BUFFER>::Infos
BufferObject::Detail<BufferObject::UNIFORM_BUFFER>::_infos = 
{
    "uniform",
    GL_UNIFORM_BUFFER,
    GL_UNIFORM_BUFFER_BINDING
};

template<>
const BufferObject::Detail<BufferObject::TEXTURE_BUFFER>::Infos
BufferObject::Detail<BufferObject::TEXTURE_BUFFER>::_infos = 
{
    "texture",
    GL_TEXTURE_BUFFER,
    GL_TEXTURE_BUFFER_DATA_STORE_BINDING
};

namespace Access {
/**
 * Convert access frequency and type to buffer data usage.
 * @param [in] freq    Access frequency.
 * @param [in] type    Access type.
 * @return OpenGL buffer usage.
 */
GLenum usage(Frequency freq, Type type)
{
    static const GLenum conversionMatrix[3][3] =
    {
        // Access::Frequency::STREAM
        {GL_STREAM_DRAW,  GL_STREAM_READ,  GL_STREAM_COPY},
        // Access::Frequency::STATIC
        {GL_STATIC_DRAW,  GL_STATIC_READ,  GL_STATIC_COPY},
        // Access::Frequency::DYNAMIC
        {GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY}
    };
    return conversionMatrix[freq.value][type.value];
}
} // Access

} // BufferObject
} // Render
} // Dumb
