namespace Framework {

/** Constructor. */
template <class vec>
NDRay<vec>::NDRay() 
    : origin(0.0f)
    , direction(0.0f)
{}
/** Constructor. 
 *  @param [in] o  Origin.
 *  @param [in] d  Direction.
 */
template <class vec>
NDRay<vec>::NDRay(const vec& o, const vec& d)
    : origin(o)
    , direction(glm::normalize(d))
{}
/** Copy constructor.
 *  @param [in] r Source ray.
 */
template <class vec>
NDRay<vec>::NDRay(const NDRay<vec>& r)
    : origin(r.origin)
    , direction(r.direction)
{}
/** Copy operator.
 *  @param [in] r Source ray.
 */
template <class vec>
NDRay<vec>& NDRay<vec>::operator= (const NDRay<vec>& r)
{
    origin = r.origin;
    direction = r.direction;
    return *this;    
}
    
}
