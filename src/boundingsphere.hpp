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

	// [todo] build from point set
	// [todo] point set and single point containment
	// [todo] MORE!

    /** Center  */
    glm::vec3 center;
    /** Radius */
	float radius;
};

}}

#endif /* _DUMB_FW_BOUNDING_SHPERE_ */
