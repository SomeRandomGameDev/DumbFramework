namespace Framework {
namespace Render    {

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
 * Get index count.
 */
size_t Mesh::triangleCount() const
{
    return _triangleCount;
}
/**
 * Retrieve vertex stream.
 */
VertexStream const& Mesh::vertexStream() const
{
    return _stream;
}

} // Render
} // Framework
