#include "boundingobjects.hpp"

namespace Dumb      {
namespace Framework {

#if 0
/// Construct from camera.
/// \param [in] camera Source camera.
void Frustum::Create(const Camera& camera)
{
	float t;
	float clip[16];
	glm::vec4 plane;

	const glm::mat4& modelview  = camera.GetModelview();
	const glm::mat4& projection = camera.GetProjection();

	clip[ 0] =  modelview[0][0] * projection[0][0];
	clip[ 1] =  modelview[0][1] * projection[1][1];
	clip[ 2] =  modelview[0][2] * projection[2][2];
	clip[ 3] = -modelview[0][2];

	clip[ 4] =  modelview[1][0] * projection[0][0];
	clip[ 5] =  modelview[1][1] * projection[1][1];
	clip[ 6] =  modelview[1][2] * projection[2][2];
	clip[ 7] = -modelview[1][2];

	clip[ 8] =  modelview[2][0] * projection[0][0];
	clip[ 9] =  modelview[2][1] * projection[1][1];
	clip[10] =  modelview[2][2] * projection[2][2];
	clip[11] = -modelview[2][2];

	clip[12] =  modelview[3][0] * projection[0][0];
	clip[13] =  modelview[3][1] * projection[1][1];
	clip[14] =  modelview[3][2] * projection[2][2] + projection[3][2];
	clip[15] = -modelview[3][2];

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

/// Test if the given point is inside the frustum.
/// \param [in] point Point to be tested.
/// \return true if the point is inside the frustum.
bool Frustum::Contains(const glm::vec3& point) const
{
	glm::vec4 p(point, 1.0);

	for(int i=0; i<FRUSTUM_PLANE_COUNT; i++)
	{
		if(glm::dot(m_planes[i], p) < 0)
		{
			return false;
		}
	}

	return true;
}

/// Test if the given axis aligned bouding box is inside the frustum.
/// \param [in] box Axis aligned bounding box.
/// \return true if the axis aligned bounding box is inside the frustum.
bool Frustum::Contains(const AABB& box) const
{
	glm::vec4 neg(1.0);

	for(int i=0; i<FRUSTUM_PLANE_COUNT; i++)
	{
		const glm::vec4& plane = m_planes[i];
		neg.x = (plane.x > 0) ? box.min.x : box.max.x;
		neg.y = (plane.y > 0) ? box.min.y : box.max.y;
		neg.z = (plane.z > 0) ? box.min.z : box.max.z;
		if(glm::dot(m_planes[i], neg) < 0)
		{
			return false;
		}
	}

	return true;
}

/// Test if an axis aligned bouding box intersects the frustum.
/// \param [in] box Axis aligned bounding box to be tested.
/// \return
///		-1 if the axis aligned bounding box is outside the frustum.
///      0 if the axis aligned bounding box is inside the frustum.
///      1 if the axis aligned bounding box intersects the frustum.
int Frustum::Intersects(const AABB& box) const
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
			return -1;
		}
		if(glm::dot(m_planes[i], pos) < 0)
		{
			return 1;
		}
	}

	return 0;
}
#endif

}}
