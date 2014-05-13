#ifndef _DUMB_FW_BOUNDING_FRUSTUM_
#define _DUMB_FW_BOUNDING_FRUSTUM_

#include "boundingobjects.hpp"

namespace Dumb      {
namespace Framework {

/**
 * Frustum (truncated pyramid).
 */
class BoundingFrustum
{
public:
    /** Constructor. */
    BoundingFrustum();
    /** Build frustum from camera and projection matrices.
     *  @param [in] camera     Camera matrix.
     *  @param [in] projection Projection matrix.
     */
    BoundingFrustum(const glm::mat4& camera, const glm::mat4& projection);
   /** Copy constructor.
    *  @param [in] frustum Source bounding frustum.
    */
    BoundingFrustum(const BoundingFrustum& frustum);
	/** Copy operator.
	 *  @param [in] frustum Source bounding frustum.
	 */
	BoundingFrustum& operator= (const BoundingFrustum& frustum);
	/** Check if the current bounding frustum contains the specified bounding box. */
	ContainmentType::Value contains(const BoundingBox& box);
	/** Check if the current bounding frustum contains the specified bounding sphere. */
	ContainmentType::Value contains(const BoundingSphere& sphere);
	/** Check if the current bounding frustum contains the specified bounding frustum. */
	ContainmentType::Value contains(const BoundingFrustum& frustum);
	/** Check if the current bounding frustum contains the specified list of points.
	 *  @param [in] buffer Pointer to the point array.
	 *  @param [in] count  Number of points 
	 *  @param [in] stride Offset between two consecutive points. (default=0)
	 */
	ContainmentType::Value contains(const float* buffer, size_t count, size_t stride=0);
	/** Check if the current bounding box contains the specified point.
	 *  @param [in] point Point to be tested.
	 */
	ContainmentType::Value contains(const glm::vec3& point);
    /** Check if the current bounding box intersects the specified ray.
     *  @param [in] ray Ray to be tested.
     */
    bool intersects(const Ray& ray);
    
    /** Get camera matrix used to build frustum planes. **/
    const glm::mat4& getCameraMatrix() const;
    /** Get projection matrix used to build frustum planes. **/
    const glm::mat4& getProjectionMatrix() const;
    /** Get near plane. **/
    const glm::vec4& getNear() const;
    /** Get far plane. **/
    const glm::vec4& getFar() const;
    /** Get top plane. **/
    const glm::vec4& getTop() const;
    /** Get bottom plane. **/
    const glm::vec4& getBottom() const;
    /** Get left plane. **/
    const glm::vec4& getLeft() const;
    /** Get right plane. **/
    const glm::vec4& getRight() const;
    
private:
    /** Plane names **/
    enum
    {
        FRUSTUM_PLANE_NEAR = 0,
        FRUSTUM_PLANE_FAR,
        FRUSTUM_PLANE_LEFT,
        FRUSTUM_PLANE_RIGHT,
        FRUSTUM_PLANE_TOP,
        FRUSTUM_PLANE_BOTTOM,
        FRUSTUM_PLANE_COUNT
    };
    /** Camera matrix used to build planes. **/
    glm::mat4 m_camera;
    /** Projection matrix used to build planes. **/
    glm::mat4 m_projection;
    /** Planes. **/
    glm::vec4 m_planes[FRUSTUM_PLANE_COUNT];
};

}}

#endif /* _DUMB_FW_BOUNDING_FRUSTUM_ */
