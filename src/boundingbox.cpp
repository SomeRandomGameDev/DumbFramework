#include "boundingobjects.hpp"

namespace Dumb      {
namespace Framework {

/** Constructor. */
BoundingBox::BoundingBox()
	: min(0.0f)
	, max(0.0f)
{}
/** Constructor. 
*  @param [in] bmin Bounding box minimum point.
*  @param [in] bmax Bounding box maximum point.
*/
BoundingBox::BoundingBox(const glm::vec3& bmin, const glm::vec3& bmax)
	: min(bmin)
	, max(bmax)
{}
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
	
	min = max = glm::vec3(buffer[0], buffer[1], buffer[2]);
	for(size_t i=1; i<count; i++, offset+=inc)
	{
		glm::vec3 dummy(buffer[offset], buffer[offset+1], buffer[offset+2]);
		min = glm::min(min, dummy);
		max = glm::max(max, dummy);
	}
}
/** Constructor.
*  @param [in] sphere Bounding sphere.
*/
BoundingBox::BoundingBox(const BoundingSphere& sphere)
{
	glm::vec3 direction = glm::vec3(sphere.radius);
	min = sphere.center - direction;
	max = sphere.center + direction;
}
/** Constructor.
*  Merge two bounding boxes.
*/
BoundingBox::BoundingBox(const BoundingBox& b0, const BoundingBox& b1)
	: min(glm::min(b0.min, b1.min))
	, max(glm::max(b0.max, b1.max))
{}
/** Copy constructor.
*  @param [in] box Source bounding box.
*/
BoundingBox::BoundingBox(const BoundingBox& box)
	: min(box.min)
	, max(box.max)
{}
/** Copy operator.
*  @param [in] box Source bounding box.
*/
BoundingBox& BoundingBox::operator= (const BoundingBox& box)
{
	min = box.min;
	max = box.max;
	return *this;
}
/** Check if the current bounding box contains the specified bounding box. */
ContainmentType::Value BoundingBox::contains(const BoundingBox& box)
{
	/// @todo
	return ContainmentType::Disjoints;
}
/** Check if the current bounding box contains the specified bounding sphere. */
ContainmentType::Value BoundingBox::contains(const BoundingSphere& sphere)
{
	/// @todo
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
		inside += ((point.x >= min.x) && (point.x <= max.x) &&
		           (point.y >= min.y) && (point.y <= max.y) &&
				   (point.z >= min.z) && (point.z <= max.z));
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
	glm::bvec3 isBetween((point.x > min.x) && (point.x < max.x),
						 (point.y > min.y) && (point.y < max.y),
						 (point.z > min.z) && (point.z < max.z));
	if(isBetween.x && isBetween.y && isBetween.z)
	{
		return ContainmentType::Contains;
	}
	if((point.x == min.x) || (point.x == max.x))
	{
		if(isBetween.y && isBetween.z)
		{ return ContainmentType::Intersects; }
	}
	if((point.y == min.y) || (point.y == max.y))
	{
		if(isBetween.z && isBetween.x)
		{ return ContainmentType::Intersects; }
	}
	if((point.z == min.z) || (point.z == max.z))
	{
		if(isBetween.x && isBetween.y)
		{ return ContainmentType::Intersects; }
	}
	return ContainmentType::Disjoints;
}
/** Check if the current bounding box contains or intersects the specified ray.
*  @param [in] ray Ray to be tested.
*/
ContainmentType::Value BoundingBox::contains(const Ray& ray)
{
	/// @todo
	return ContainmentType::Disjoints;
}
/** Apply transformation.
*  @param [in] m 4*4 transformation matrix.
*/
void BoundingBox::transform(const glm::mat4& m)
{
	glm::vec3 dummy[2] =
	{ 
		glm::vec3(m * glm::vec4(min,1.0f)),
		glm::vec3(m * glm::vec4(max,1.0f))
	};
	min = glm::min(dummy[0], dummy[1]);
	max = glm::max(dummy[0], dummy[1]);
}

}}
