#include "boundingobjects.hpp"

namespace Dumb {

/** Constructor. */
BoundingFrustum::BoundingFrustum()
	: _camera()
	, _projection()
{
    for(size_t i=0; i<FRUSTUM_PLANE_COUNT; i++)
    {
    	_planes[i] = glm::vec4(0.0f);
    }
}
/** Build frustum from camera and projection matrices.
 * @param [in] camera Camera matrix.
 * @param [in] projection Projection matrix.
 */
BoundingFrustum::BoundingFrustum(const glm::mat4& camera, const glm::mat4& projection)
	: _camera(camera)
	, _projection(projection)
{
	float clip[16];
	glm::vec4 plane;

	clip[ 0] =  camera[0][0] * projection[0][0];
	clip[ 1] =  camera[0][1] * projection[1][1];
	clip[ 2] =  camera[0][2] * projection[2][2];
	clip[ 3] = -camera[0][2];

	clip[ 4] =  camera[1][0] * projection[0][0];
	clip[ 5] =  camera[1][1] * projection[1][1];
	clip[ 6] =  camera[1][2] * projection[2][2];
	clip[ 7] = -camera[1][2];

	clip[ 8] =  camera[2][0] * projection[0][0];
	clip[ 9] =  camera[2][1] * projection[1][1];
	clip[10] =  camera[2][2] * projection[2][2];
	clip[11] = -camera[2][2];

	clip[12] =  camera[3][0] * projection[0][0];
	clip[13] =  camera[3][1] * projection[1][1];
	clip[14] =  camera[3][2] * projection[2][2] + projection[3][2];
	clip[15] = -camera[3][2];

	// right
	plane.x = clip[ 3] - clip[ 0];
	plane.y = clip[ 7] - clip[ 4];
	plane.z = clip[11] - clip[ 8];
	plane.w = clip[15] - clip[12];
	_planes[FRUSTUM_PLANE_RIGHT] = plane;

	// left
	plane.x = clip[ 3] + clip[ 0];
	plane.y = clip[ 7] + clip[ 4];
	plane.z = clip[11] + clip[ 8];
	plane.w = clip[15] + clip[12];
	_planes[FRUSTUM_PLANE_LEFT] = plane;

	// bottom
	plane.x = clip[ 3] + clip[ 1];
	plane.y = clip[ 7] + clip[ 5];
	plane.z = clip[11] + clip[ 9];
	plane.w = clip[15] + clip[13];
	_planes[FRUSTUM_PLANE_BOTTOM] = plane;

	// top
	plane.x = clip[ 3] - clip[ 1];
	plane.y = clip[ 7] - clip[ 5];
	plane.z = clip[11] - clip[ 9];
	plane.w = clip[15] - clip[13];
    _planes[FRUSTUM_PLANE_TOP] = plane;
	
	// far
	plane.x = clip[ 3] - clip[ 2];
	plane.y = clip[ 7] - clip[ 6];
	plane.z = clip[11] - clip[10];
	plane.w = clip[15] - clip[14];
    _planes[FRUSTUM_PLANE_FAR] = plane;

	// near
	plane.x = clip[ 3] + clip[ 2];
	plane.y = clip[ 7] + clip[ 6];
	plane.z = clip[11] + clip[10];
	plane.w = clip[15] + clip[14];
    _planes[FRUSTUM_PLANE_NEAR] = plane;
}
/** Copy constructor.
 * @param [in] frustum Source bounding frustum.
 */
BoundingFrustum::BoundingFrustum(const BoundingFrustum& frustum)
	: _camera(frustum._camera)
	, _projection(frustum._projection)
{
	for(size_t i=0; i<FRUSTUM_PLANE_COUNT; i++)
	{
		_planes[i] = frustum._planes[i];
	}
}
/** Copy operator.
 * @param [in] frustum Source bounding frustum.
 */
BoundingFrustum& BoundingFrustum::operator= (const BoundingFrustum& frustum)
{
	_camera = frustum._camera;
	_projection = frustum._projection;
	for(size_t i=0; i<FRUSTUM_PLANE_COUNT; i++)
	{
		_planes[i] = frustum._planes[i];
	}
	return *this;
}
/** Check if the current bounding frustum contains the specified bounding box. */
ContainmentType::Value BoundingFrustum::contains(const BoundingBox& box)
{
    const glm::vec3& bmin = box.getMin();
    const glm::vec3& bmax = box.getMax();
    
	glm::vec3 neg;
	glm::vec3 pos;

	for(int i=0; i<FRUSTUM_PLANE_COUNT; i++)
	{
		const glm::vec3& pnormal = _planes[i].getNormal();
		neg.x = (pnormal.x > 0) ? bmin.x : bmax.x;
		pos.x = (pnormal.x > 0) ? bmax.x : bmin.x;
		neg.y = (pnormal.y > 0) ? bmin.y : bmax.y;
		pos.y = (pnormal.y > 0) ? bmax.y : bmin.y;
		neg.z = (pnormal.z > 0) ? bmin.z : bmax.z;
		pos.z = (pnormal.z > 0) ? bmax.z : bmin.z;
		if(_planes[i].distance(neg) < 0)
		{
			return ContainmentType::Disjoints;
		}
		if(_planes[i].distance(pos) < 0)
		{
			return ContainmentType::Intersects;
		}
	}

	return ContainmentType::Contains;
}
/** Check if the current bounding frustum contains the specified bounding sphere. */
ContainmentType::Value BoundingFrustum::contains(const BoundingSphere& sphere)
{
    Plane::Side side;
    bool intersects = false;
    
    for(int i=0; i<FRUSTUM_PLANE_COUNT; i++)
    {
        side = sphere.classify(_planes[i]);
        if(Plane::Front == side)
        { return ContainmentType::Disjoints; }
        intersects = intersects || (Plane::On == side);
    }
    
	return intersects ? ContainmentType::Intersects : ContainmentType::Contains;
}
/** Check if the current bounding frustum contains the specified bounding frustum. */
ContainmentType::Value BoundingFrustum::contains(const BoundingFrustum& frustum)
{
	/// @todo
	return ContainmentType::Disjoints;
}
/** Check if the current bounding frustum contains the specified list of points.
 * @param [in] buffer Pointer to the point array.
 * @param [in] count Number of points
 * @param [in] stride Offset between two consecutive points. (default=0)
 */
ContainmentType::Value BoundingFrustum::contains(const float* buffer, size_t count, size_t stride)
{
	/// @todo
	return ContainmentType::Disjoints;
}
/** Check if the current bounding box contains the specified point.
 * @param [in] point Point to be tested.
 */
ContainmentType::Value BoundingFrustum::contains(const glm::vec3& point)
{
	/// @todo
	return ContainmentType::Disjoints;
}
/** Check if the current bounding box intersects the specified ray.
 * @param [in] ray Ray to be tested.
 */
bool BoundingFrustum::intersects(const Ray& ray)
{
	/// @todo
	return false;
}
/** Get camera matrix used to build frustum planes. **/
const glm::mat4& BoundingFrustum::getCameraMatrix() const { return _camera; }
/** Get projection matrix used to build frustum planes. **/
const glm::mat4& BoundingFrustum::getProjectionMatrix() const { return _projection; }
/** Get near plane. **/
const Plane& BoundingFrustum::getNear() const { return _planes[FRUSTUM_PLANE_NEAR]; }
/** Get far plane. **/
const Plane& BoundingFrustum::getFar() const { return _planes[FRUSTUM_PLANE_NEAR]; }
/** Get top plane. **/
const Plane& BoundingFrustum::getTop() const { return _planes[FRUSTUM_PLANE_TOP]; }
/** Get bottom plane. **/
const Plane& BoundingFrustum::getBottom() const { return _planes[FRUSTUM_PLANE_BOTTOM]; }
/** Get left plane. **/
const Plane& BoundingFrustum::getLeft() const { return _planes[FRUSTUM_PLANE_LEFT]; }
/** Get right plane. **/
const Plane& BoundingFrustum::getRight() const { return _planes[FRUSTUM_PLANE_RIGHT]; }

}
