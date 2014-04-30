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
ContainmentType::Value BoundingSphere::Contains(const BoundingSphere& sphere)
{
	float distance = glm::distance(center, sphere.center);
	if(distance < (radius-sphere.radius)) return ContainmentType::Contains;
	if(distance > (radius+sphere.radius)) return ContainmentType::Disjoints;
	return ContainmentType::Intersects;
}

}}
