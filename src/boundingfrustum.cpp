#include "boundingobjects.hpp"

namespace Dumb      {
namespace Framework {
/** Constructor. */
BoundingFrustum::BoundingFrustum()
	: m_camera()
	, m_projection()
{
    for(size_t i=0; i<FRUSTUM_PLANE_COUNT; i++)
    {
    	m_planes[i] = glm::vec4(0.0f)
    }
}
/** Build frustum from camera and projection matrices.
 * @param [in] camera Camera matrix.
 * @param [in] projection Projection matrix.
 */
BoundingFrustum::BoundingFrustum(const glm::mat4& camera, const glm::mat4& projection)
	: m_camera(camera)
	, m_projection(projection)
{
	float t;
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

	t = sqrt( plane.x*plane.x + plane.y*plane.y + plane.z*plane.z );
	m_planes[FRUSTUM_PLANE_RIGHT] = plane / t;

	// left
	plane.x = clip[ 3] + clip[ 0];
	plane.y = clip[ 7] + clip[ 4];
	plane.z = clip[11] + clip[ 8];
	plane.w = clip[15] + clip[12];

	t = sqrt( plane.x*plane.x + plane.y*plane.y + plane.z*plane.z );
	m_planes[FRUSTUM_PLANE_LEFT] = plane / t;
	

	// bottom
	plane.x = clip[ 3] + clip[ 1];
	plane.y = clip[ 7] + clip[ 5];
	plane.z = clip[11] + clip[ 9];
	plane.w = clip[15] + clip[13];

	t = sqrt( plane.x*plane.x + plane.y*plane.y + plane.z*plane.z );
	m_planes[FRUSTUM_PLANE_BOTTOM] = plane / t;
	
	// top
	plane.x = clip[ 3] - clip[ 1];
	plane.y = clip[ 7] - clip[ 5];
	plane.z = clip[11] - clip[ 9];
	plane.w = clip[15] - clip[13];

	t = sqrt( plane.x*plane.x + plane.y*plane.y + plane.z*plane.z );
	m_planes[FRUSTUM_PLANE_TOP] = plane / t;

	// far
	plane.x = clip[ 3] - clip[ 2];
	plane.y = clip[ 7] - clip[ 6];
	plane.z = clip[11] - clip[10];
	plane.w = clip[15] - clip[14];

	t = sqrt( plane.x*plane.x + plane.y*plane.y + plane.z*plane.z );
	m_planes[FRUSTUM_PLANE_FAR] = plane / t;

	// near
	plane.x = clip[ 3] + clip[ 2];
	plane.y = clip[ 7] + clip[ 6];
	plane.z = clip[11] + clip[10];
	plane.w = clip[15] + clip[14];

	t = sqrt( plane.x*plane.x + plane.y*plane.y + plane.z*plane.z );
	m_planes[FRUSTUM_PLANE_NEAR] = plane / t;

}
/** Copy constructor.
 * @param [in] frustum Source bounding frustum.
 */
BoundingFrustum::BoundingFrustum(const BoundingFrustum& frustum)
	: m_camera(frustum.m_camera)
	, m_projection(frustum.m_projection)
{
	for(size_t i=0; i<FRUSTUM_PLANE_COUNT; i++)
	{
		m_planes[i] = frustum.m_planes[i];
	}
}
/** Copy operator.
 * @param [in] frustum Source bounding frustum.
 */
BoundingFrustum& BoundingFrustum::operator= (const BoundingFrustum& frustum)
{
	m_camera = frustum.m_camera;
	m_projection = frustum.m_projection;
	for(size_t i=0; i<FRUSTUM_PLANE_COUNT; i++)
	{
		m_planes[i] = frustum.m_planes[i];
	}
	return *this;
}
/** Check if the current bounding frustum contains the specified bounding box. */
ContainmentType::Value BoundingFrustum::contains(const BoundingBox& box)
{
	glm::vec4 neg(1.0);
	glm::vec4 pos(1.0);

	for(int i=0; i<FRUSTUM_PLANE_COUNT; i++)
	{
		const glm::vec4& plane = m_planes[i];
		neg.x = (plane.x > 0) ? box.min.x : box.max.x;
		pos.x = (plane.x > 0) ? box.max.x : box.min.x;
		neg.y = (plane.y > 0) ? box.min.y : box.max.y;
		pos.y = (plane.y > 0) ? box.max.y : box.min.y;
		neg.z = (plane.z > 0) ? box.min.z : box.max.z;
		pos.z = (plane.z > 0) ? box.max.z : box.min.z;
		if(glm::dot(m_planes[i], neg) < 0)
		{
			return ContainmentType::Disjoints;
		}
		if(glm::dot(m_planes[i], pos) < 0)
		{
			return ContainmentType::Intersects;
		}
	}

	return ContainmentType::Contains;
}
/** Check if the current bounding frustum contains the specified bounding sphere. */
ContainmentType::Value BoundingFrustum::contains(const BoundingSphere& sphere)
{
	/// @todo
	return ContainmentType::Disjoints;
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
const glm::mat4& BoundingFrustum::getCameraMatrix() const { return m_camera; }
/** Get projection matrix used to build frustum planes. **/
const glm::mat4& BoundingFrustum::getProjectionMatrix() const { return m_projection; }
/** Get near plane. **/
const glm::vec4& BoundingFrustum::getNear() const { return m_planes[FRUSTUM_PLANE_NEAR]; }
/** Get far plane. **/
glm::vec4 BoundingFrustum::getFar() const { return m_planes[FRUSTUM_PLANE_NEAR]; }
/** Get top plane. **/
glm::vec4 BoundingFrustum::getTop() const { return m_planes[FRUSTUM_PLANE_TOP]; }
/** Get bottom plane. **/
glm::vec4 BoundingFrustum::getBottom() const { return m_planes[FRUSTUM_PLANE_BOTTOM]; }
/** Get left plane. **/
glm::vec4 BoundingFrustum::getLeft() const { return m_planes[FRUSTUM_PLANE_LEFT]; }
/** Get right plane. **/
glm::vec4 BoundingFrustum::getRight() const { return m_planes[FRUSTUM_PLANE_RIGHT]; }

}}
