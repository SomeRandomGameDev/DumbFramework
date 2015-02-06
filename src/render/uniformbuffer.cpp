#include <DumbFramework/render/uniformbuffer.hpp>

namespace Framework {
namespace Render    {

/** Constructor. **/
UniformBuffer::UniformBuffer()
    : BufferObject::Detail<BufferObject::UNIFORM_BUFFER>()
{}
/** Destructor. **/
UniformBuffer::~UniformBuffer()
{}

} // Render
} // Framework
