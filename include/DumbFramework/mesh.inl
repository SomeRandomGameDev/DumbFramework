namespace Framework {
namespace Render    {

/**
 * Get geometry attribute.
 * @param [in] id Attribute identifier.
 * @return Geometry attribute. @warning It can be empty.
 */
Geometry::Attribute const& Mesh::attribute(AttributeId id) const
{
    return _attributes[id];
}
/**
 * Get mesh bounding sphere.
 * @return Bounding sphere.
 */
BoundingSphere const& Mesh::boundingSphere() const
{
    return _sphere;
}
/**
 * Get mesh axis aligned bounding box.
 * @return Axis aligned bounding box.
 */
BoundingBox const& Mesh::boundingBox() const
{
    return _aabb;
}
/**
 * Get vertex count.
 */
size_t Mesh::vertexCount() const
{
    return _vertexCount;
}
/**
 * Get attributes mask.
 */
uint32_t Mesh::attributesMask() const
{
    return _attributesMask;
}
/**
 * Get vertex buffer.
 */
VertexBuffer const& Mesh::vertexBuffer() const
{
    return _vertexBuffer;
}
/**
 * Get indeex buffer.
 */
IndexBuffer const& Mesh::indexBuffer() const
{
    return _indexBuffer;
}
/**
 * Get vertex size in bytes.
 */
size_t Mesh::vertexSize() const
{
    size_t size = 0;
    for(size_t i=0; i<AttributeCount; i++)
    {
        size += _attributes[i].bytes();
    }
    return size;
}
/**
 * Get index count.
 */
size_t Mesh::triangleCount() const
{
    return _triangleCount;
}

} // Render
} // Framework
