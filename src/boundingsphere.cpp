#include "boundingobjects.hpp"

namespace Framework {

/** Constructor. */
BoundingSphere::BoundingSphere()
	: _center(0.0f)
	, _radius(0.0f)
	, _squareRadius(0.0f)
{}
/** Constructor. 
 *  @param [in] c  Bounding sphere center.
 *  @param [in] r  Bounding sphere radius.
 */
BoundingSphere::BoundingSphere(const glm::vec3& c, float r)
	: _center(c)
	, _radius(r)
	, _squareRadius(r*r)
{}
/** Constructor
 *  @param [in] buffer Pointer to the point array.
 *  @param [in] count  Number of points 
 *  @param [in] stride Offset between two consecutive points. (default=0)
 */
BoundingSphere::BoundingSphere(const float* buffer, size_t count, size_t stride)
{
	off_t offset, inc;
	inc = stride + 3;
	offset = inc;
	
	glm::vec3 pmin, pmax;	
	pmin = pmax = glm::vec3(buffer[0], buffer[1], buffer[2]);

	for(size_t i=1; i<count; i++, offset+=inc)
	{
		glm::vec3 dummy(buffer[offset], buffer[offset+1], buffer[offset+2]);
		pmin = glm::min(pmin, dummy);
		pmax = glm::max(pmax, dummy);
	}
	
	glm::vec3 delta = pmax - pmin;
	_center = (pmin+pmax) / 2.0f;
	_squareRadius = glm::dot(delta, delta) / 4.0f;
	_radius = sqrt(_squareRadius);
}
/** Constructor.
 *  Merge two bounding spheres.
 */
BoundingSphere::BoundingSphere(const BoundingSphere& s0, const BoundingSphere& s1)
{
	BoundingSphere merged;
	float distance = glm::distance(s0._center, s1._center);
	float dr = s1._radius - s0._radius;

	_radius = (distance + s0._radius + s1._radius) / 2.0f;
	_squareRadius = _radius * _radius;
	_center = (((distance - dr) * s0._center) + ((distance + dr) * s1._center)) / (2.0f * distance);
}
/** Copy constructor.
 *  @param [in] sphere Source bounding sphere.
 */
BoundingSphere::BoundingSphere(const BoundingSphere& sphere)
	: _center(sphere._center)
	, _radius(sphere._radius)
	, _squareRadius(sphere._squareRadius)
{}
/** Copy operator.
 *  @param [in] sphere Source bounding sphere.
 */
BoundingSphere& BoundingSphere::operator= (const BoundingSphere& sphere)
{
	_center = sphere._center;
	_radius = sphere._radius;
	_squareRadius = sphere._squareRadius;
	return *this;
}
/** Check if the current bounding sphere contains the specified bounding box. */
ContainmentType::Value BoundingSphere::contains(const BoundingBox& box)
{
	glm::vec3 diffMin = _center  - box.getMin();
	glm::vec3 diffMax = box.getMax() - _center;
	
	glm::vec3 eMin = diffMin * diffMin;
	glm::vec3 eMax = diffMax * diffMax;
	
	float dmin = 0.0f;
	for(int i=0; i<3; i++)
	{
		if     (diffMin[i] < 0.0f) { dmin += eMin[i]; }
		else if(diffMax[i] < 0.0f) { dmin += eMax[i]; }
	}

	if(dmin <= _squareRadius)
	{
		bool all=true, any=false;
		for(int i=0; i<3; i++)
		{
			bool tMin = (eMin[i] <= _squareRadius);
			bool tMax(eMax[i] <= _squareRadius);
			all = all && (tMin && tMax);
			any = any || (tMin || tMax);
		}
		if(all) { return ContainmentType::Contains;   }
		if(any) { return ContainmentType::Intersects; }
	}
	return ContainmentType::Disjoints;
}
/** Check if the current bounding sphere contains the specified bounding sphere. */
ContainmentType::Value BoundingSphere::contains(const BoundingSphere& sphere)
{
	glm::vec3 delta = _center - sphere._center;
	
	float squareDistance = glm::dot(delta, delta);
	float dr0 = _radius + sphere._radius;
	float dr1 = _radius - sphere._radius;
	float sqdr0 = dr0 * dr0;
	float sqdr1 = dr1 * dr1;

	if(squareDistance >  sqdr0) { return ContainmentType::Disjoints;  }
	if(squareDistance <= sqdr1) { return ContainmentType::Contains; }
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
		float distance = glm::distance(_center, point);
		inside += (_radius < distance) ? 0 : 1;
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
	float distance = glm::distance(_center, point);
	if(distance < _radius) { return ContainmentType::Contains;  }
	if(distance > _radius) { return ContainmentType::Disjoints; }
	return ContainmentType::Intersects;
}
/** Check if the current bounding sphere intersects the specified ray.
  * @param [in] ray Ray to be tested.
  */
bool BoundingSphere::intersects(const Ray& ray)
{
    float epsilon = std::numeric_limits<float>::epsilon();
    float distance = glm::distance(_center, ray.origin);
    glm::vec3 diff = _center - ray.origin;
    float t0 = glm::dot(diff, ray.direction);
    float d2 = glm::dot(diff, diff) - (t0 * t0);
    if(d2 > _squareRadius) { return false; }
    float t1 = sqrt(_squareRadius - d2);
    distance = (t0 > (t1 + epsilon)) ? (t0 - t1) : (t0 + t1);
    return (distance > epsilon);
}
/** Tell on which side of the specified plane the current bounding sphere is.
 *  @param [in] plane Plane.
 */
Plane::Side BoundingSphere::classify(const Plane& plane) const
{
	float d = plane.distance(_center);
	if(d <= -_radius)
	{ return Plane::Back; }
	if(d >= _radius)
	{ return Plane::Front; }
	return Plane::On;
}
/** Apply transformation.
 *  @param [in] m 4*4 transformation matrix.
 */
void BoundingSphere::transform(const glm::mat4& m)
{
	// Transform center.
	glm::vec4 pt = m * glm::vec4(_center, 1.0f);
	_center = glm::vec3(pt.x, pt.y, pt.z);
	
	// Find the biggest scale.
	glm::vec3 sx(m[0].x, m[1].x, m[2].x);
	glm::vec3 sy(m[0].y, m[1].y, m[2].y);
	glm::vec3 sz(m[0].z, m[1].z, m[2].z);
	float scale = glm::max(glm::max(glm::dot(sx,sx), glm::dot(sy,sy)), glm::dot(sz,sz));
	_squareRadius = _squareRadius * scale;
	_radius = _radius * sqrt(scale);
}

/** Get sphere center. **/
const glm::vec3& BoundingSphere::getCenter() const
{ return _center; }
/** Get sphere radius. **/
float BoundingSphere::getRadius() const
{ return _radius; }
/** Get sphere squared radius. **/
float BoundingSphere::getSquareRadius() const
{ return _squareRadius; }

}
