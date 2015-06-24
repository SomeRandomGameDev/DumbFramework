/*
 * Copyright 2015 MooZ
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <DumbFramework/geometry/plane.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

/** Constructor. */
Plane::Plane()
    : _normal(0.0f)
    , _distance(0.0f)
{}
/** Constructor.
 *  @param [in] n Normal.
 *  @param [in] d Distance.
 */
Plane::Plane(glm::vec3 const& n, float d)
    : _normal(n)
    , _distance(d)
{}
/** Build plane from 3 points.
 *  @param [in] p0 First point.
 *  @param [in] p1 Second point.
 *  @param [in] p2 Third point.
 */
Plane::Plane(glm::vec3 const& p0, glm::vec3 const& p1, glm::vec3 const& p2)
{
    _normal   =  glm::normalize(glm::cross(p1-p0, p2-p0));
    _distance = -glm::dot(_normal, p0);
}
/** Build plane from a vec4.
 *  @param [in] p Raw data.
 */
Plane::Plane(glm::vec4 const& p)
    : _normal(p.x, p.y, p.z)
    , _distance(p.w)
{
    normalize();
}
/** Copy constructor.
 *  @param [in] plane Source plane.
 */
Plane::Plane(Plane const& plane)
    : _normal(plane._normal)
    , _distance(plane._distance)
{}
/** Copy operator.
 *  @param [in] plane Source plane.
 */
Plane& Plane::operator= (Plane const& plane)
{
    _normal   = plane._normal;
    _distance = plane._distance;
    return *this;
}
/** Initialize from vec4.
 *  @param [in] p Raw data.
 */
Plane& Plane::operator= (glm::vec4 const& p)
{
    _normal   = glm::vec3(p.x, p.y, p.z);
    _distance = p.w;
    normalize();
    return *this;
}
/** Normalize plane. **/
Plane normalize(Plane const& plane)
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
float Plane::distance(glm::vec3 const& p) const
{
    return glm::dot(p, _normal) + _distance;
}
/** Tell on which side the specified point is.
 *  @param [in] p Point.
 */
Side Plane::classify(glm::vec3 const& p) const
{
    float epsilon = 1e-6f;
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
/** Compute intersection between the specified ray and the plane.
 *  @param [in]  ray       Ray to be tested.
 *  @param [out] distance  Distance from origin to plane if the ray instersects. -1.0 otherwise.
 */
bool Plane::intersects(Ray3 const& ray, float& distance)
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
/** Compute closest point on the plane from the specified point. **/
glm::vec3 Plane::closestPoint(glm::vec3 const& p) const
{
    return p - (distance(p) * _normal);
}
/** Get normal. **/
const glm::vec3& Plane::getNormal() const { return _normal; }
/** Get distance. **/
float Plane::getDistance() const { return _distance; }

} // Geometry
} // Core
} // Dumb
