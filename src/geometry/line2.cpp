#include <DumbFramework/geometry/line2.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

/** Constructor. */
Line2::Line2()
{}
/** Build plane from 2 points.
 *  @param [in] p0 First point.
 *  @param [in] p1 Second point.
 */
Line2::Line2(glm::vec2 const& p0, glm::vec2 const& p1)
{
    glm::vec2 direction = p1 - p0;
    _normal.x = -direction.y;
    _normal.y =  direction.x;
    _distance = (p0.x * p1.y) - (p1.x * p0.y);
    
    float length = glm::length(direction);
    _normal   /= length;
    _distance /= length;
}
/** Copy constructor.
 *  @param [in] line Source line.
 */
Line2::Line2(Line2 const& line)
    : _normal(line._normal)
    , _distance(line._distance)
{}
/** Copy operator.
 *  @param [in] line Source line.
 */
Line2& Line2::operator= (Line2 const& line)
{
    _normal   = line._normal;
    _distance = line._distance;
    return *this;
}
/** Compute distance of a point to the line.
 *  @param [in] p Point.
 */
float Line2::distance(glm::vec2 const& p) const
{
    return glm::dot(_normal, p) + _distance;
}
/** Tell on which side the specified point is.
 *  @param [in] p Point.
 */
Side Line2::classify(glm::vec2 const& p) const
{
    float epsilon = std::numeric_limits<float>::epsilon();
    float signedDistance = distance(p);
    if(signedDistance < -epsilon)
    {
        return Side::Back;
    }
    if(signedDistance > epsilon)
    {
        return Side::Front;
    }
    return Side::On;
}
/** Compute intersection between the specified ray and the line.
 *  @param [in]  ray       Ray to be tested.
 *  @param [out] distance  Distance from origin to plane if the ray instersects. -1.0 otherwise.
 */
bool Line2::intersects(Ray2 const& ray, float& distance)
{
    float epsilon = std::numeric_limits<float>::epsilon();
    float nd = glm::dot(_normal, ray.direction);
    if(glm::abs(nd) < epsilon)
    {
        distance = -std::numeric_limits<float>::max();
        return false;
    }
    distance = -(glm::dot(_normal, ray.origin) + _distance) / nd;
    return (distance >= 0);
}
/** Compute closest point on the line from the specified point. **/
glm::vec2 Line2::closestPoint(glm::vec2 const& p) const
{
    return p - (distance(p) * _normal);
}
/** Get normal. **/
glm::vec2 const& Line2::getNormal() const { return _normal; }
/** Get distance. **/
float Line2::getDistance() const { return _distance; }

} // Geometry
} // Core
} // Dumb

