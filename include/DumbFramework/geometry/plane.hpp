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
#ifndef _DUMBFRAMEWORK_PLANE_
#define _DUMBFRAMEWORK_PLANE_

#include <glm/glm.hpp>
#include <DumbFramework/geometry/side.hpp>
#include <DumbFramework/geometry/ray.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

/**
 * Plane.
 */
class Plane
{
    public:
        /** Constructor. */
        Plane();
        /** Build plane from 3 points.
         *  @param [in] p0 First point.
         *  @param [in] p1 Second point.
         *  @param [in] p2 Third point.
         */
        Plane(glm::vec3 const& p0, glm::vec3 const& p1, glm::vec3 const& p2);
        /** Constructor.
         *  @param [in] n Normal.
         *  @param [in] d Distance.
         */
        Plane(glm::vec3 const& n, float d);
        /** Build plane from a vec4.
         *  @param [in] p Raw data.
         */
        Plane(glm::vec4 const& p);
        /** Copy constructor.
         *  @param [in] plane Source plane.
         */
        Plane(Plane const& plane);
        /** Copy operator.
         *  @param [in] plane Source plane.
         */
        Plane& operator= (Plane const& plane);
        /** Initialize from vec4.
         *  @param [in] p Raw data.
         */
        Plane& operator= (glm::vec4 const& p);
        /** Normalize plane. **/
        friend Plane normalize(Plane const& plane);
        /** Normalize plane. **/
        void normalize();
        /** Compute distance of a point to the plane.
         *  @param [in] p Point.
         */
        float distance(glm::vec3 const& p) const;
        /** Tell on which side the specified point is.
         *  @param [in] p Point.
         */
        Side classify(glm::vec3 const& p) const;
        /** Compute intersection between the specified ray and the plane.
         *  @param [in]  ray       Ray to be tested.
         *  @param [out] distance  Distance from origin to plane if the ray instersects. -1.0 otherwise.
         */
        bool intersects(Ray3 const& ray, float& distance);
        /** Compute closest point on the plane from the specified point. **/
        glm::vec3 closestPoint(glm::vec3 const& p) const;
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

} // Geometry
} // Core
} // Dumb

#endif // _DUMBFRAMEWORK_PLANE_
