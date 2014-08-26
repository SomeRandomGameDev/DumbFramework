#ifndef _DUMB_FW_RAY_
#define _DUMB_FW_RAY_

#include <glm/glm.hpp>

namespace Framework {

/**
 * N-Dimensional ray.
 **/
template <class vec>
struct NDRay
{
    /** Constructor. */
    NDRay();
    /** Constructor. 
     *  @param [in] o  Origin.
     *  @param [in] d  Direction.
     */
    NDRay(const vec& o, const vec& d);
    /** Copy constructor.
     *  @param [in] r Source ray.
     */
    NDRay(const NDRay& r);

    /** Copy operator.
     *  @param [in] r Source ray.
     */
    NDRay& operator= (const NDRay& r);

    /** Origin */
    vec origin;
    /** Direction */
    vec direction;   
};

/** 3d ray. **/
typedef NDRay<glm::vec3> Ray;
/** 2d ray. **/
typedef NDRay<glm::vec2> Ray2d;

}

#include "ray.inl"

#endif /* _DUMB_FW_RAY_ */
