#ifndef _DUMB_FW_LINE_2D_
#define _DUMB_FW_LINE_2D_

#include <glm/glm.hpp>
#include "ray.hpp"
#include "side.hpp"

namespace Framework {

/**
 * 2d line.
 */
class Line2d
{
    public:
        /** Constructor. */
        Line2d();
        /** Build plane from 2 points.
         *  @param [in] p0 First point.
         *  @param [in] p1 Second point.
         */
        Line2d(const glm::vec2& p0, const glm::vec2& p1);
        /** Copy constructor.
         *  @param [in] plane Source plane.
         */
        Line2d(const Line2d& line);
        /** Copy operator.
         *  @param [in] plane Source line.
         */
        Line2d& operator= (const Line2d& line);
        /** Compute distance of a point to the line.
         *  @param [in] p Point.
         */
        float distance(const glm::vec2& p) const;
        /** Tell on which side the specified point is.
         *  @param [in] p Point.
         */
        Side classify(const glm::vec2& p) const;
        /** Compute intersection between the specified ray and the line.
         *  @param [in]  ray       Ray to be tested.
         *  @param [out] distance  Distance from origin to plane if the ray instersects. -1.0 otherwise.
         */
        bool intersects(const Ray2d& ray, float& distance);
        /** Compute closest point on the line from the specified point. **/
        glm::vec2 closestPoint(const glm::vec2& p) const;
        /** Get normal. **/
        const glm::vec2& getNormal() const;
        /** Get distance. **/
        float getDistance() const;
        
    private:
        /** Normal **/
        glm::vec2 _normal;
        /** Distance from the origin. **/
        float _distance;
};

}

#endif /* _DUMB_FW_LINE_2D_ */
