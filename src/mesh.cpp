#include <DumbFramework/mesh.hpp>

namespace Framework {
namespace Render    {

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
 * Create mesh.
 * @param [in] vertexCount   Vertex count.
 * @param [in] triangleCount Triangle count.
 * @param [in] mask          Attribute masks.
 * @return false if an error occured.
 */
bool Mesh::create(size_t vertexCount, size_t triangleCount, AttributeMask mask)
{
    bool ret;
    
    _triangleCount = triangleCount;
    _vertexCount   = vertexCount;
    _primitiveType = Geometry::Primitive::TRIANGLES;
    
    // stride will be fixed later.
    // [todo] make an array out of this
    size_t offset = 0;
    if(mask & HasPosition)
    {
        _attributes[Position] = Geometry::Attribute(Position, Geometry::ComponentType::FLOAT, 3, false, 0, offset, 0);
        offset += _attributes[Position].bytes();
    }
    if(mask & HasNormal)
    {
        _attributes[Normal] = Geometry::Attribute(Normal, Geometry::ComponentType::FLOAT, 3, false, 0, offset, 0);
        offset += _attributes[Position].bytes();
    }
    if(mask & HasTexCoord)
    {
        _attributes[TexCoord] = Geometry::Attribute(TexCoord, Geometry::ComponentType::FLOAT, 2, false, 0, offset, 0);
        offset += _attributes[Position].bytes();
    }
    // fix stride.
    size_t stride = offset;
    for(size_t i=0; i<AttributeCount; i++)
    {
        _attributes[i].stride = stride;
    }
    
    ret = _indexBuffer.create(3 * triangleCount * sizeof(GLuint), nullptr, BufferObject::Access::Frequency::STATIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        // [todo] Log_Error
        return false;
    }
    
    ret = _vertexBuffer.create(vertexCount * stride, nullptr, BufferObject::Access::Frequency::DYNAMIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        // [todo] Log_Error
        return false;
    }
    
    return true;
}
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

} // Render
} // Framework


