#include <DumbFramework/mesh.hpp>
#include <cstring>

namespace Framework {
namespace Render    {

/* Vertex attributes. */
static const Geometry::Attribute vertexAttributes[Mesh::AttributeCount] = 
{
    Geometry::Attribute(Mesh::Position, Geometry::ComponentType::FLOAT, 3, false, 0, 0, 0),
    Geometry::Attribute(Mesh::Normal,   Geometry::ComponentType::FLOAT, 3, false, 0, 0, 0),
    Geometry::Attribute(Mesh::TexCoord, Geometry::ComponentType::FLOAT, 2, false, 0, 0, 0)
};

/**
 * Default constructor.
 */
Mesh::Mesh()
    : _vertexBuffer()
    , _indexBuffer()
    , _vertexCount(0)
    , _triangleCount(0)
    , _primitiveType(Geometry::Primitive::TRIANGLES)
    , _attributesMask(Mesh::None)
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
 * @param [in] vertexData    Pointer to vertex data (nullptr by default).
 * @param [in] indexData     Pointer to triangle data (nullptr by default).
 * @return false if an error occured.
 */
bool Mesh::create(size_t vertexCount, size_t triangleCount, uint32_t mask, void* vertexData, void* indexData)
{
    bool ret;
    
    _triangleCount  = triangleCount;
    _vertexCount    = vertexCount;
    _primitiveType  = Geometry::Primitive::TRIANGLES;
    _attributesMask = mask;
    
    // stride will be fixed later.
    size_t offset = 0;
    for(size_t i=0; i<AttributeCount; i++)
    {
        if(mask && (1 << i))
        {
            memcpy(&_attributes[i], &vertexAttributes[i], sizeof(Geometry::Attribute));
            _attributes[i].offset = offset;
            offset += _attributes[i].bytes();
        }
    }
    // fix stride.
    size_t stride = offset;
    for(size_t i=0; i<AttributeCount; i++)
    {
        _attributes[i].stride = stride;
    }
    
    ret = _indexBuffer.create(3 * triangleCount * sizeof(GLuint), indexData, BufferObject::Access::Frequency::STATIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create index buffer!");
        return false;
    }
    
    ret = _vertexBuffer.create(vertexCount * stride, vertexData, BufferObject::Access::Frequency::DYNAMIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create vertex buffer!");
        return false;
    }
    
    if(nullptr != vertexData)
    {
        update();
    }
    return true;
}
/**
 * Set vertex attribute data.
 * @param [in] id  Attribute id.
 * @param [in] ptr Pointer to vertex attribute data.
 * @return false if the attribute is not supported by the mesh or if the 
 *         pointer is null.
 */
bool Mesh::setAttribute(AttributeId id, uint8_t* ptr)
{
    if(nullptr == ptr)
    {
        Log_Error(Module::Render, "Invalid data pointer.");
        return false;
    }
    if(0 == (_attributesMask & (1 << id)))
    {
        Log_Error(Module::Render, "Unsupported vertex attribute.");
        return false;
    }
    
    uint8_t* data = (uint8_t*)_vertexBuffer.map(BufferObject::Access::Policy::WRITE_ONLY);
    data += _attributes[id].offset;
    size_t size   = _attributes[id].bytes();
    size_t stride = _attributes[id].stride;
    for(size_t i=0; i<_vertexCount; i++)
    {
        memcpy(data, ptr, size);
        ptr  += size;
        data += stride;
    }
    return _vertexBuffer.unmap();
}
/**
 * Update mesh.
 * It recomputes bounding box and bounding sphere, as long as
 * vertex normals, tangents and bitangents.
 */
void Mesh::update()
{
    computeBoundingObjects();
    // [todo] compute normals, tangents and bitangents.
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


