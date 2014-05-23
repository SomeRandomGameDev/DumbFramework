#ifndef _DUMB_FW_RAY_
#define _DUMB_FW_RAY_

#include <glm/glm.hpp>

namespace Framework {

/**
 * Ray.
 */
struct Ray
{
    /** Constructor. */
    Ray();
    /** Constructor. 
	 *  @param [in] o  Origin.
	 *  @param [in] d  Direction.
	 */
	Ray(const glm::vec3& o, const glm::vec3& d);
	/** Copy constructor.
	 *  @param [in] r Source ray.
	 */
    Ray(const Ray& r);

	/** Copy operator.
     *  @param [in] r Source ray.
	 */
	Ray& operator= (const Ray& r);
	
    /** Origin */
    glm::vec3 origin;
    /** Direction */
	glm::vec3 direction;
};

}

#endif /* _DUMB_FW_RAY_ */
