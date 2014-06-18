#include "line2d.hpp"

namespace Framework {

/** Constructor. */
Line2d::Line2d()
{}
/** Build plane from 2 points.
 *  @param [in] p0 First point.
 *  @param [in] p1 Second point.
 */
Line2d::Line2d(const glm::vec2& p0, const glm::vec2& p1)
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
 *  @param [in] plane Source plane.
 */
Line2d::Line2d(const Line2d& line)
    : _normal(line._normal)
    , _distance(line._distance)
{}
/** Copy operator.
 *  @param [in] plane Source line.
 */
Line2d& Line2d::operator= (const Line2d& line)
{
    _normal   = line._normal;
    _distance = line._distance;
    return *this;
}
/** Compute distance of a point to the line.
 *  @param [in] p Point.
 */
float Line2d::distance(const glm::vec2& p) const
{
    return glm::dot(_normal, p) + _distance;
}
/** Tell on which side the specified point is.
 *  @param [in] p Point.
 */
Side Line2d::classify(const glm::vec2& p) const
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
bool Line2d::intersects(const Ray2d& ray, float& distance)
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
glm::vec2 Line2d::closestPoint(const glm::vec2& p) const
{
    return p - (distance(p) * _normal);
}
/** Get normal. **/
const glm::vec2& Line2d::getNormal() const { return _normal; }
/** Get distance. **/
float Line2d::getDistance() const { return _distance; }

}

