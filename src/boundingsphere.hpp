#ifndef _DUMB_FW_BOUNDING_SHPERE_
#define _DUMB_FW_BOUNDING_SHPERE_

#include <glm/glm.hpp>

namespace Dumb      {
namespace Framework {

namespace ContainmentType
{
	enum Value
	{
		Contains = 0,
		Intersects,
		Disjoints
	};
}

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
	BoundingSphere(const glm::vec3 c, float r);
	/** Constructor
	 *  @param [in] buffer Pointer to the point array.
	 *  @param [in] count  Number of points 
	 *  @param [in] stride Offset between two consecutive points. (default=0)
	 */
	BoundingSphere(const float* buffer, size_t count, size_t stride=0);
	/** Copy constructor.
	 *  @param [in] sphere Source bounding sphere.
	 */
    BoundingSphere(const BoundingSphere& sphere);

	/** Copy operator.
     *  @param [in] sphere Source bounding sphere.
	 */
	BoundingSphere& operator= (const BoundingSphere& sphere);
	
	/** Check if the current bounding sphere contains the specified bounding sphere. */
	ContainmentType::Value Contains(const BoundingSphere& sphere);
	/** Check if the current bounding sphere contains the specified list of points.
	  * @param [in] buffer Pointer to the point array.
	  * @param [in] count  Number of points 
	  * @param [in] stride Offset between two consecutive points. (default=0)
	  */
	ContainmentType::Value Contains(const float* buffer, size_t count, size_t stride=0);
	/** Check if the current bounding sphere contains the specified point.
	  * @param [in] point Point to be tested.
	  */
	ContainmentType::Value Contains(const glm::vec3& point);

	// [todo] MORE!

    /** Center  */
    glm::vec3 center;
    /** Radius */
	float radius;
};

}}

#endif /* _DUMB_FW_BOUNDING_SHPERE_ */
