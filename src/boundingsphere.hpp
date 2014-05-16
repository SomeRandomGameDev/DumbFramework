#ifndef _DUMB_FW_BOUNDING_SPHERE_
#define _DUMB_FW_BOUNDING_SPHERE_

#include "boundingobjects.hpp"

namespace Dumb      {
namespace Framework {

/**
 * Bounding sphere.
 */
struct BoundingSphere
{
    /** Constructor. */
    BoundingSphere();
    /** Constructor. 
	 *  @param [in] c  Bounding sphere center.
	 *  @param [in] r  Bounding sphere radius.
	 */
	BoundingSphere(const glm::vec3& c, float r);
	/** Constructor.
	 *  @param [in] buffer Pointer to the point array.
	 *  @param [in] count  Number of points 
	 *  @param [in] stride Offset between two consecutive points. (default=0)
	 */
	BoundingSphere(const float* buffer, size_t count, size_t stride=0);
	/** Constructor.
	 *  Merge two bounding spheres.
	 */
	BoundingSphere(const BoundingSphere& s0, const BoundingSphere& s1);
	/** Copy constructor.
	 *  @param [in] sphere Source bounding sphere.
	 */
    BoundingSphere(const BoundingSphere& sphere);

	/** Copy operator.
     *  @param [in] sphere Source bounding sphere.
	 */
	BoundingSphere& operator= (const BoundingSphere& sphere);
	/** Check if the current bounding sphere contains the specified bounding box. */
	ContainmentType::Value contains(const BoundingBox& box);
	/** Check if the current bounding sphere contains the specified bounding sphere. */
	ContainmentType::Value contains(const BoundingSphere& sphere);
	/** Check if the current bounding sphere contains the specified list of points.
	 *  @param [in] buffer Pointer to the point array.
	 *  @param [in] count  Number of points 
	 *  @param [in] stride Offset between two consecutive points. (default=0)
	 */
	ContainmentType::Value contains(const float* buffer, size_t count, size_t stride=0);
	/** Check if the current bounding sphere contains the specified point.
	 *  @param [in] point Point to be tested.
	 */
	ContainmentType::Value contains(const glm::vec3& point);
    /** Check if the current bounding sphere intersects the specified ray.
     *  @param [in] ray Ray to be tested.
     */
    bool intersects(const Ray& ray);
	/** Tell on which side of the specified plane the current bounding sphere is.
     *  @param [in] plane Plane.
     */
    Plane::Side classify(const Plane& plane) const;
	/** Apply transformation.
	 *  @param [in] m 4*4 transformation matrix.
	 */
	void transform(const glm::mat4& m);
	
    /** Center  */
    glm::vec3 center;
    /** Radius */
	float radius;
};

}}

#endif /* _DUMB_FW_BOUNDING_SPHERE_ */
