#include "plane.hpp"

namespace Dumb      {
namespace Framework {

/** Constructor. */
Plane::Plane()
    : _normal()
    , _distance(0.0f)
{}
/** Constructor.
 *  @param [in] n Normal.
 *  @param [in] d Distance.
 */
Plane::Plane(const glm::vec3& n, float d)
    : _normal(n)
    , _distance(d)
{}
/** Build plane from 3 points.
 *  @param [in] p0 First point.
 *  @param [in] p1 Second point.
 *  @param [in] p2 Third point.
 */
Plane::Plane(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
{
    _normal   =  glm::normalize(glm::cross(p1-p0, p2-p0));
    _distance = -glm::dot(_normal, p0);
}
/** Build plane from a vec4.
 *  @param [in] p Raw data.
 */
Plane::Plane(const glm::vec4& p)
    : _normal(p.x, p.y, p.z)
    , _distance(p.w)
{
    normalize();
}
/** Copy constructor.
 *  @param [in] plane Source plane.
 */
Plane::Plane(const Plane& plane)
    : _normal(plane._normal)
    , _distance(plane._distance)
{}
/** Copy operator.
 *  @param [in] plane Source plane.
 */
Plane& Plane::operator= (const Plane& plane)
{
    _normal   = plane._normal;
    _distance = plane._distance;
    return *this;
}
/** Initialize from vec4.
 *  @param [in] p Raw data.
 */
Plane& Plane::operator= (const glm::vec4& p)
{
    _normal   = glm::vec3(p.x, p.y, p.z);
    _distance = p.w;
    normalize();
    return *this;
}
/** Normalize plane. **/
Plane normalize(const Plane& plane)
{
    float length = glm::length(plane._normal);
    return Plane(plane._normal/length, plane._distance/length);
}
/** Normalize plane. **/
void Plane::normalize()
{
    float length = glm::length(_normal);
    _normal   /= length;
    _distance /= length;
}
/** Compute pseudo distance of a point to the plane.
 *  @param [in] p Point.
 */
float Plane::distance(const glm::vec3& p) const
{
    return glm::dot(p, _normal) + _distance;
}
/** Tell on which side the specified point is.
 *  @param [in] p Point.
 */
Plane::Side Plane::classifiy(const glm::vec3& p) const
{
    float epsilon = std::numeric_limits<float>::epsilon();
    float signedDistance = distance(p);
    if(signedDistance < -epsilon)
    {
        return Plane::Back;
    }
    if(signedDistance > epsilon)
    {
        return Plane::Front;
    }
    return Plane::On;
}
/** Compute intersection between the specified ray and the plane.
 *  @param [in]  ray       Ray to be tested.
 *  @param [out] distance  Distance from origin to plane if the ray instersects. -1.0 otherwise.
 */
bool Plane::intersects(const Ray& ray, float& distance)
{
    float epsilon = std::numeric_limits<float>::epsilon();
    float nd = glm::dot(_normal, ray.direction);
    if(glm::abs(nd) < epsilon)
    {
        distance = -1.0f;
        return false;
    }
    distance = -(glm::dot(_normal, ray.origin) + _distance) / nd;
	return true;
}
/** Compute closest point on the plane from the specified point. **/
glm::vec3 Plane::closestPoint(const glm::vec3& p) const
{
    return p - (glm::abs(distance(p)) * _normal);
}
/** Get normal. **/
const glm::vec3& Plane::getNormal() const { return _normal; }
/** Get distance. **/
float Plane::getDistance() const { return _distance; }

}}
