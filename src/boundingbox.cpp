#include "boundingobjects.hpp"

namespace Framework {

/** Constructor. */
BoundingBox::BoundingBox()
	: _min(0.0f)
	, _max(0.0f)
	, _center(0.0f)
	, _extent(0.0f)
{}
/** Constructor. 
*  @param [in] bmin Bounding box minimum point.
*  @param [in] bmax Bounding box maximum point.
*/
BoundingBox::BoundingBox(const glm::vec3& bmin, const glm::vec3& bmax)
	: _min(bmin)
	, _max(bmax)
{
	_update();
}
/** Constructor.
*  @param [in] buffer Pointer to the point array.
*  @param [in] count  Number of points 
*  @param [in] stride Offset between two consecutive points. (default=0)
*/
BoundingBox::BoundingBox(const float* buffer, size_t count, size_t stride)
{
	off_t offset, inc;
	inc = stride + 3;
	offset = inc;
	
	_min = _max = glm::vec3(buffer[0], buffer[1], buffer[2]);
	for(size_t i=1; i<count; i++, offset+=inc)
	{
		glm::vec3 dummy(buffer[offset], buffer[offset+1], buffer[offset+2]);
		_min = glm::min(_min, dummy);
		_max = glm::max(_max, dummy);
	}
	
	_update();
}
/** Constructor.
*  @param [in] sphere Bounding sphere.
*/
BoundingBox::BoundingBox(const BoundingSphere& sphere)
{
	glm::vec3 direction = glm::vec3(sphere.getRadius());
	_min = sphere.getCenter() - direction;
	_max = sphere.getCenter() + direction;
	_update();
}
/** Constructor.
*  Merge two bounding boxes.
*/
BoundingBox::BoundingBox(const BoundingBox& b0, const BoundingBox& b1)
	: _min(glm::min(b0._min, b1._min))
	, _max(glm::max(b0._max, b1._max))
{
	_update();
}
/** Copy constructor.
*  @param [in] box Source bounding box.
*/
BoundingBox::BoundingBox(const BoundingBox& box)
	: _min(box._min)
	, _max(box._max)
	, _center(box._center)
	, _extent(box._extent)
{}
/** Copy operator.
*  @param [in] box Source bounding box.
*/
BoundingBox& BoundingBox::operator= (const BoundingBox& box)
{
	_min    = box._min;
	_max    = box._max;
	_center = box._center;
	_extent = box._extent;
	return *this;
}
/** Check if the current bounding box contains the specified bounding box. */
ContainmentType::Value BoundingBox::contains(const BoundingBox& box)
{
	if((box._max.x < _min.x) ||
	   (box._min.x > _max.x) ||
	   (box._max.y < _min.y) ||
	   (box._min.y > _max.y) ||
	   (box._max.z < _min.z) ||
	   (box._min.z > _max.z))
	{ return ContainmentType::Disjoints; }	
	if((box._min.x >= _min.x) &&
	   (box._max.x <= _max.x) &&
	   (box._min.y >= _min.y) &&
	   (box._max.y <= _max.y) &&
	   (box._min.z >= _min.z) &&
	   (box._max.z <= _max.z))
	{ return ContainmentType::Contains; }	
	return ContainmentType::Intersects;
}
/** Check if the current bounding box contains the specified bounding sphere. */
ContainmentType::Value BoundingBox::contains(const BoundingSphere& sphere)
{
	glm::vec3 diffMin = sphere.getCenter() - _min;
	glm::vec3 diffMax = _max - sphere.getCenter();
	
	if((diffMin.x >= sphere.getRadius()) &&
	   (diffMin.y >= sphere.getRadius()) &&
	   (diffMin.z >= sphere.getRadius()) &&
	   (diffMax.x >= sphere.getRadius()) &&
	   (diffMax.y >= sphere.getRadius()) &&
	   (diffMax.z >= sphere.getRadius()))
    { return ContainmentType::Contains; }

	float dmin = 0.0f;
	float dmax = 0.0f;
	glm::vec3 sqrMin = diffMin * diffMin;
	glm::vec3 sqrMax = diffMax * diffMax;
	for(int i=0; i<3; i++)
	{
		dmax += glm::max(sqrMin[i], sqrMax[i]);
		if(diffMin[i] < 0.0f)
		{
			dmin += sqrMin[i];
		}
		else if(diffMax[i] < 0.0f)
		{
			dmin += sqrMax[i];
		}
	}
	float r2 = sphere.getSquareRadius();
	if((dmin <= r2) && (r2 <= dmax))
	{ return ContainmentType::Intersects; }
	return ContainmentType::Disjoints;
}
/** Check if the current bounding box contains the specified list of points.
*  @param [in] buffer Pointer to the point array.
*  @param [in] count  Number of points 
*  @param [in] stride Offset between two consecutive points. (default=0)
*/
ContainmentType::Value BoundingBox::contains(const float* buffer, size_t count, size_t stride)
{
	off_t offset = 0, inc = stride + 3;
	size_t inside = 0;
	for(size_t i=0; i<count; i++, offset+=inc)
	{
		glm::vec3 point(buffer[offset], buffer[offset+1], buffer[offset+2]);
		inside += ((point.x >= _min.x) && (point.x <= _max.x) &&
		           (point.y >= _min.y) && (point.y <= _max.y) &&
				   (point.z >= _min.z) && (point.z <= _max.z));
	}
	if(inside == 0) { return ContainmentType::Disjoints;  }
	if(inside < count) { return ContainmentType::Intersects; }
	return ContainmentType::Contains;
}
/** Check if the current bounding box contains the specified point.
*  @param [in] point Point to be tested.
*/
ContainmentType::Value BoundingBox::contains(const glm::vec3& point)
{
	glm::vec3 dummy = glm::abs(_center - point);
	if((dummy.x < _extent.x) &&
	   (dummy.y < _extent.y) &&
	   (dummy.z < _extent.z))
	{ return ContainmentType::Contains; }
	if((dummy.x > _extent.x) &&
	   (dummy.y > _extent.y) &&
	   (dummy.z > _extent.z))
	{ return ContainmentType::Disjoints; }
	return ContainmentType::Intersects;
}
/** Check if the current bounding box intersects the specified ray.
*  @param [in] ray Ray to be tested.
*/
bool BoundingBox::intersects(const Ray& ray)
{
	glm::vec3 t0 = (_min - ray.origin) / ray.direction;
	glm::vec3 t1 = (_max - ray.origin) / ray.direction;

	glm::vec3 t2 = glm::min(t0, t1);
	glm::vec3 t3 = glm::max(t0, t1);
	
	float tmin = glm::max(glm::max(t2.x, t2.y), t2.z);
	float tmax = glm::min(glm::min(t3.x, t3.y), t3.z);

	if((tmax < 0) || (tmin > tmax))
	{ return false; }
	return true;
}
/** Tell on which side of the specified plane the current bounding box is.
 *  @param [in] plane Plane.
 */
Side BoundingBox::classify(const Plane& plane) const
{
	float radius   = glm::dot(glm::abs(plane.getNormal()), _extent);
	float distance = plane.distance(_center);
    
	if(distance < -radius) { return Side::Back;  }
	if(distance >  radius) { return Side::Front; }
	return Side::On;
}
/** Apply transformation.
*  @param [in] m 4*4 transformation matrix.
*/
void BoundingBox::transform(const glm::mat4& m)
{
	glm::vec3 dummy[2] =
	{ 
		glm::vec3(m * glm::vec4(_min,1.0f)),
		glm::vec3(m * glm::vec4(_max,1.0f))
	};
	_min = glm::min(dummy[0], dummy[1]);
	_max = glm::max(dummy[0], dummy[1]);
	_update();
}
/** Get lowest box corner. **/
const glm::vec3& BoundingBox::getMin() const { return _min; }
/** Get highest box corner. **/
const glm::vec3& BoundingBox::getMax() const { return _max; }
/** Get box center. **/
const glm::vec3& BoundingBox::getCenter() const { return _center; }
/** Get box extent. **/
const glm::vec3& BoundingBox::getExtent() const { return _extent; }
/** Update center and extent. **/
void BoundingBox::_update()
{
	_center = (_min + _max) / 2.0f;
	_extent = glm::abs(_max - _center);	
}

}
