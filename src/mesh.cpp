#include <DumbFramework/mesh.hpp>

namespace Framework {

/**
 * Default constructor.
 */
Mesh::Mesh()
    : _vertexBuffer()
    , _indexBuffer()
    , _vertexCount(0)
    , _sphere()
    , _aabb()
{}
/**
 * Destructor.
 */
Mesh::~Mesh()
{}
/**
 * Release any resource allocated by this mesh.
 */
void Mesh::destroy()
{
    _vertexBuffer.destroy();
    _indexBuffer.destroy();
}
/** Compute axis aligned bounding box and bounding sphere. **/
void Mesh::computeBoundingObjects()
{
    const uint8_t *ptr = (uint8_t*)_vertexBuffer.map(BufferObject::Access::Policy::READ_ONLY);
    if(nullptr == ptr)
    {
        return;
    }
    _aabb = BoundingBox(ptr+_attributes[Position].offset, _vertexCount, _attributes[Position].stride);
    _vertexBuffer.unmap();
    
    _sphere = BoundingSphere(_aabb.getCenter(), glm::distance(_aabb.getMin(), _aabb.getMax()) / 2.0f);
}

} // Framework


