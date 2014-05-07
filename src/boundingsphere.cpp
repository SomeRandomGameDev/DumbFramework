#include "boundingsphere.hpp"

namespace Dumb      {
namespace Framework {

/** Constructor. */
BoundingSphere::BoundingSphere()
	: center(0.0f)
	, radius(0.0f)
{}
/** Constructor. 
 *  @param [in] c  Bounding sphere center.
 *  @param [in] r  Bounding sphere radius.
 */
BoundingSphere::BoundingSphere(const glm::vec3 c, float r)
	: center(c)
	, radius(r)
{}
/** Constructor
 *  @param [in] buffer Pointer to the point array.
 *  @param [in] count  Number of points 
 *  @param [in] stride Offset between two consecutive points. (default=0)
 */
BoundingSphere::BoundingSphere(const float* buffer, size_t count, size_t stride)
{
	off_t offset, inc;
	glm::vec3 pmin, pmax;
	
	pmin = pmax = glm::vec3(buffer[0], buffer[1], buffer[2]);
	inc = stride + 3;
	offset = inc;
	for(size_t i=1; i<count; i++, offset+=inc)
	{
		glm::vec3 dummy(buffer[offset], buffer[offset+1], buffer[offset+2]);
		pmin = glm::min(pmin, dummy);
		pmax = glm::max(pmax, dummy);
	}
	center = (pmin+pmax) / 2.0f;
	radius = glm::distance(pmin, pmax) / 2.0f;
}
/** Constructor.
 *  Merge two bounding spheres.
 */
BoundingSphere::BoundingSphere(const BoundingSphere& s0, const BoundingSphere& s1)
{
	BoundingSphere merged;
	float distance = glm::distance(s0.center, s1.center);
	float dr = s1.radius - s0.radius;

	radius = (distance + s0.radius + s1.radius) / 2.0f;
	center = (((distance - dr) * s0.center) + ((distance + dr) * s1.center)) / (2.0f * distance);
}
/** Copy constructor.
 *  @param [in] sphere Source bounding sphere.
 */
BoundingSphere::BoundingSphere(const BoundingSphere& sphere)
	: center(sphere.center)
	, radius(sphere.radius)
{}
/** Copy operator.
 *  @param [in] sphere Source bounding sphere.
 */
BoundingSphere& BoundingSphere::operator= (const BoundingSphere& sphere)
{
	center = sphere.center;
	radius = sphere.radius;
	return *this;
}	
/** Check if the current bounding sphere contains the specified bounding sphere. */
ContainmentType::Value BoundingSphere::contains(const BoundingSphere& sphere)
{
	glm::vec3 delta = center - sphere.center;
	
	float squareDistance = glm::dot(delta, delta);
	float dr0 = radius + sphere.radius;
	float dr1 = radius - sphere.radius;
	float sqdr0 = dr0 * dr0;
	float sqdr1 = dr1 * dr1;

	if(squareDistance > sqdr0) { return ContainmentType::Disjoints;  }
	if(squareDistance < sqdr1) { return ContainmentType::Contains; }
	return ContainmentType::Intersects;
}
/** Check if the current bounding sphere contains the specified list of points.
 * @param [in] buffer Pointer to the point array.
 * @param [in] count  Number of points
 * @param [in] stride Offset between two consecutive points. (default=0) 
 */
ContainmentType::Value BoundingSphere::contains(const float* buffer, size_t count, size_t stride)
{
	off_t offset = 0, inc = stride + 3;
	size_t inside = 0;
	for(size_t i=0; i<count; i++, offset+=inc)
	{
		glm::vec3 point(buffer[offset], buffer[offset+1], buffer[offset+2]);
		float distance = glm::distance(center, point);
		inside += (radius < distance) ? 0 : 1;
	}

	if(inside == 0) { return ContainmentType::Disjoints;  }
	if(inside < count) { return ContainmentType::Intersects; }
	return ContainmentType::Contains;
}
/** Check if the current bounding sphere contains the specified point.
 * @param [in] point Point to be tested.
 */
ContainmentType::Value BoundingSphere::contains(const glm::vec3& point)
{
	float distance = glm::distance(center, point);
	if(distance < radius) { return ContainmentType::Contains;  }
	if(distance > radius) { return ContainmentType::Disjoints; }
	return ContainmentType::Intersects;
}

}}
