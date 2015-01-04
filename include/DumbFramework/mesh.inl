namespace Framework {

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

} // Framework
