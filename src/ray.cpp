#include "ray.hpp"

namespace Dumb      {
namespace Framework {

    /** Constructor. */
    Ray::Ray() 
        : origin(0.0f)
        , direction(0.0f)
    {}
    /** Constructor. 
	 *  @param [in] o  Origin.
	 *  @param [in] d  Direction.
	 */
	Ray::Ray(const glm::vec3& o, const glm::vec3& d)
        : origin(o)
        , direction(glm::normalize(d))
    {}
	/** Copy constructor.
	 *  @param [in] r Source ray.
	 */
    Ray::Ray(const Ray& r)
        : origin(r.origin)
        , direction(r.direction)
    {}
	/** Copy operator.
     *  @param [in] r Source ray.
	 */
	Ray& Ray::operator= (const Ray& r)
    {
        origin = r.origin;
        direction = r.direction;
        return *this;    
    }
}}

