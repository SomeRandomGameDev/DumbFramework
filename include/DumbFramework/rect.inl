
namespace Framework {

/**
 * Compute minimum point.
 */
glm::vec2 Rect::min() const
{
    return position;
}
/**
 * Compute maximum point.
 */
glm::vec2 Rect::max() const
{
    return position + size;
}
/**
 * Compute center.
 */
glm::vec2 Rect::center() const
{
    return position + (size / 2.0f);
}
/**
 * Compute extent.
 */
glm::vec2 Rect::extent() const
{
    return size / 2.0f;
}

} // Framework
