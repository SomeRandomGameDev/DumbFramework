#ifndef _DUMB_FW_PLANE_
#define _DUMB_FW_PLANE_

#include <glm/glm.hpp>
#include "ray.hpp"

namespace Dumb      {
namespace Framework {

/**
 * Plane.
 */
class Plane
{
public:
    enum Side
    {
        Front = 0,
        Back,
        On
    };
    
public:
    /** Constructor. */
    Plane();
    /** Build plane from 3 points.
     *  @param [in] p0 First point.
     *  @param [in] p1 Second point.
     *  @param [in] p2 Third point.
     */
    Plane(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
    /** Constructor.
     *  @param [in] n Normal.
     *  @param [in] d Distance.
     */
    Plane(const glm::vec3& n, float d);
    /** Build plane from a vec4.
     *  @param [in] p Raw data.
     */
    Plane(const glm::vec4& p);
    /** Copy constructor.
     *  @param [in] plane Source plane.
     */
    Plane(const Plane& plane);
	/** Copy operator.
     *  @param [in] plane Source plane.
	 */
	Plane& operator= (const Plane& plane);
	/** Initialize from vec4.
     *  @param [in] p Raw data.
	 */
	Plane& operator= (const glm::vec4& p);
    /** Normalize plane. **/
    friend Plane normalize(const Plane& plane);
    /** Normalize plane. **/
    void normalize();
    /** Compute distance of a point to the plane.
     *  @param [in] p Point.
     */
    float distance(const glm::vec3& p) const;
    /** Tell on which side the specified point is.
     *  @param [in] p Point.
     */
    Side classifiy(const glm::vec3& p) const;
    /** Compute intersection between the specified ray and the plane.
     *  @param [in]  ray       Ray to be tested.
     *  @param [out] distance  Distance from origin to plane if the ray instersects. -1.0 otherwise.
     */
    bool intersects(const Ray& ray, float& distance);
    /** Compute closest point on the plane from the specified point. **/
    glm::vec3 closestPoint(const glm::vec3& p) const;
    /** Get normal. **/
    const glm::vec3& getNormal() const;
    /** Get distance. **/
    float getDistance() const;
    
private:
    /** Normal. **/
    glm::vec3 _normal;
    /** Distance from the origin. **/
    float _distance;
};

}}

#endif /* _DUMB_FW_PLANE_ */
