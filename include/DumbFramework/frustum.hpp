#ifndef _DUMB_FW_FRUSTUM_
#define _DUMB_FW_FRUSTUM_

#include <DumbFramework/boundingobjects.hpp>

namespace Framework {

/**
 * Frustum (truncated pyramid).
 */
class Frustum
{
public:
    /** Constructor. */
    Frustum();
    /** Build frustum from camera and projection matrices.
     *  @param [in] camera     Camera matrix.
     *  @param [in] projection Projection matrix.
     */
    Frustum(glm::mat4 const& camera, glm::mat4 const& projection);
    /** Copy constructor.
     *  @param [in] frustum Source bounding frustum.
     */
    Frustum(Frustum const& frustum);
    /** Copy operator.
     *  @param [in] frustum Source bounding frustum.
     */
    Frustum& operator= (Frustum const& frustum);
    /** Check if the current bounding frustum contains the specified bounding box. */
    ContainmentType::Value contains(BoundingBox const& box);
    /** Check if the current bounding frustum contains the specified bounding sphere. */
    ContainmentType::Value contains(BoundingSphere const& sphere);
    /** Check if the current bounding frustum contains the specified bounding frustum. */
    ContainmentType::Value contains(Frustum const& frustum);
    /** Check if the current bounding frustum contains the specified list of points.
     *  @param [in] buffer Pointer to the point array.
     *  @param [in] count  Number of points 
     *  @param [in] stride Offset between two consecutive points. (default=0)
     */
    ContainmentType::Value contains(const float* buffer, size_t count, size_t stride=0);
    /** Check if the current bounding box contains the specified point.
     *  @param [in] point Point to be tested.
     */
    ContainmentType::Value contains(glm::vec3 const& point);
    /** Check if the current bounding box intersects the specified ray.
     *  @param [in] ray Ray to be tested.
     */
    bool intersects(Ray const& ray);
    
    /** Get camera matrix used to build frustum planes. **/
    const glm::mat4& getCameraMatrix() const;
    /** Get projection matrix used to build frustum planes. **/
    const glm::mat4& getProjectionMatrix() const;
    /** Get near plane. **/
    const Plane& getNear() const;
    /** Get far plane. **/
    const Plane& getFar() const;
    /** Get top plane. **/
    const Plane& getTop() const;
    /** Get bottom plane. **/
    const Plane& getBottom() const;
    /** Get left plane. **/
    const Plane& getLeft() const;
    /** Get right plane. **/
    const Plane& getRight() const;
    
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
    glm::mat4 _camera;
    /** Projection matrix used to build planes. **/
    glm::mat4 _projection;
    /** Planes. **/
    Plane _planes[FRUSTUM_PLANE_COUNT];
};

}

#endif /* _DUMB_FW_FRUSTUM_ */
