#include <DumbFramework/bufferobject.hpp>

namespace Framework    {
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
} // Framework
