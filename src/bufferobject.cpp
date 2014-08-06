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

} // BufferObject
} // Framework
