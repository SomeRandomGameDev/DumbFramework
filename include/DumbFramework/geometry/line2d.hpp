/*
 * Copyright 2015 MooZ
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _DUMBFRAMEWORK_LINE_2D_
#define _DUMBFRAMEWORK_LINE_2D_

#include <glm/glm.hpp>
#include <DumbFramework/geometry/ray.hpp>
#include <DumbFramework/geometry/side.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

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
        Line2d(glm::vec2 const& p0, glm::vec2 const& p1);
        /** Copy constructor.
         *  @param [in] plane Source plane.
         */
        Line2d(Line2d const& line);
        /** Copy operator.
         *  @param [in] plane Source line.
         */
        Line2d& operator= (Line2d const& line);
        /** Compute distance of a point to the line.
         *  @param [in] p Point.
         */
        float distance(glm::vec2 const& p) const;
        /** Tell on which side the specified point is.
         *  @param [in] p Point.
         */
        Side classify(glm::vec2 const& p) const;
        /** Compute intersection between the specified ray and the line.
         *  @param [in]  ray       Ray to be tested.
         *  @param [out] distance  Distance from origin to plane if the ray instersects. -1.0 otherwise.
         */
        bool intersects(Ray2 const& ray, float& distance);
        /** Compute closest point on the line from the specified point. **/
        glm::vec2 closestPoint(glm::vec2 const& p) const;
        /** Get normal. **/
        glm::vec2 const& getNormal() const;
        /** Get distance. **/
        float getDistance() const;
        
    private:
        /** Normal **/
        glm::vec2 _normal;
        /** Distance from the origin. **/
        float _distance;
};

} // Geometry
} // Core
} // Dumb

#endif // _DUMBFRAMEWORK_LINE_2D_
