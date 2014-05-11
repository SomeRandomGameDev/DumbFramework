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
	if((box.max.x < min.x) ||
	   (box.min.x > max.x) ||
	   (box.max.y < min.y) ||
	   (box.min.y > max.y) ||
	   (box.max.z < min.z) ||
	   (box.min.z > max.z))
	{ return ContainmentType::Disjoints; }	
	if((box.min.x >= min.x) &&
	   (box.max.x <= max.x) &&
	   (box.min.y >= min.y) &&
	   (box.max.y <= max.y) &&
	   (box.min.z >= min.z) &&
	   (box.max.z <= max.z))
	{ return ContainmentType::Contains; }	
	return ContainmentType::Intersects;
}
/** Check if the current bounding box contains the specified bounding sphere. */
ContainmentType::Value BoundingBox::contains(const BoundingSphere& sphere)
{
	glm::vec3 diffMin = sphere.center - min;
	glm::vec3 diffMax = max - sphere.center;
	
	if((diffMin.x >= sphere.radius) &&
	   (diffMin.y >= sphere.radius) &&
	   (diffMin.z >= sphere.radius) &&
	   (diffMax.x >= sphere.radius) &&
	   (diffMax.y >= sphere.radius) &&
	   (diffMax.z >= sphere.radius))
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
	float r2 = sphere.radius * sphere.radius;
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
	if((point.x < min.x) ||
	   (point.y < min.y) ||
	   (point.z < min.z) ||
	   (point.x > max.x) ||
	   (point.y > max.y) ||
	   (point.z > max.z))
	{ return ContainmentType::Disjoints; }
	if((point.x == min.x) ||
	   (point.y == min.y) ||
	   (point.z == min.z) ||
	   (point.x == max.x) ||
	   (point.y == max.y) ||
	   (point.z == max.z))
	{ return ContainmentType::Intersects; }
	return ContainmentType::Contains;
}
/** Check if the current bounding box intersects the specified ray.
*  @param [in] ray Ray to be tested.
*/
bool BoundingBox::intersects(const Ray& ray)
{
	/// @todo
/*
    ref : http://www.cg.cs.tu-bs.de/media/publications/fast-rayaxis-aligned-bounding-box-overlap-tests-using-ray-slopes.pdf
    r->ii = 1.0/i; // inverse ray direction
    r->ij = 1.0/j;
    r->ik = 1.0/k

    r->s_yx = r->i * r->ij; // ray slopes
    r->s_xy = r->j * r->ii;
    r->s_zy = r->j * r->ik;
    r->s_yz = r->k * r->ij;
    r->s_xz = r->i * r->ik;
    r->s_zx = r->k * r->ii;

    r->c_xy = r->y - r->s_xy * r->x; // precomputation
    r->c_yx = r->x - r->s_yx * r->y;
    r->c_zy = r->y - r->s_zy * r->z;
    r->c_yz = r->z - r->s_yz * r->y;
    r->c_xz = r->z - r->s_xz * r->x;
    r->c_zx = r->x - r->s_zx * r->z 
 
	if ((r->ox > b->x1) || (r->oy > b->y1) || (r->oz > b->z1)
	 || (r->s_xy * b->x1 - b->y0 + r->c_xy < 0)
	 || (r->s_yx * b->y1 - b->x0 + r->c_yx < 0)
	 || (r->s_zy * b->z1 - b->y0 + r->c_zy < 0)
	 || (r->s_yz * b->y1 - b->z0 + r->c_yz < 0)
	 || (r->s_xz * b->x1 - b->z0 + r->c_xz < 0)
	 || (r->s_zx * b->z1 - b->x0 + r->c_zx < 0))
		return false;
	return true
*/
	return false;
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
