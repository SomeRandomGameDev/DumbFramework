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
#ifndef _DUMBFRAMEWORK_BOUNDING_SPHERE_
#define _DUMBFRAMEWORK_BOUNDING_SPHERE_

#include <sys/types.h>
#include <glm/glm.hpp>

#include <DumbFramework/geometry/ray.hpp>
#include <DumbFramework/geometry/containment.hpp>
#include <DumbFramework/geometry/plane.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

class BoundingBox;

/**
 * Bounding sphere.
 */
class BoundingSphere
{
    public:
        /** Constructor. */
        BoundingSphere();
        /** Constructor. 
         *  @param [in] c  Bounding sphere center.
         *  @param [in] r  Bounding sphere radius.
         */
        BoundingSphere(glm::vec3 const& c, float r);
        /** Constructor.
         *  @param [in] buffer Pointer to the point array.
         *  @param [in] count  Number of points 
         *  @param [in] stride Offset between two consecutive points.
         */
        BoundingSphere(const uint8_t* buffer, size_t count, size_t stride);
        /** Constructor.
         *  Merge two bounding spheres.
         */
        BoundingSphere(BoundingSphere const& s0, BoundingSphere const& s1);
        /** Copy constructor.
         *  @param [in] sphere Source bounding sphere.
         */
        BoundingSphere(BoundingSphere const& sphere);

        /** Copy operator.
         *  @param [in] sphere Source bounding sphere.
         */
        BoundingSphere& operator= (BoundingSphere const& sphere);
        /** Check if the current bounding sphere contains the specified bounding box. */
        ContainmentType::Value contains(BoundingBox const& box);
        /** Check if the current bounding sphere contains the specified bounding sphere. */
        ContainmentType::Value contains(BoundingSphere const& sphere);
        /** Check if the current bounding sphere contains the specified list of points.
         *  @param [in] buffer Pointer to the point array.
         *  @param [in] count  Number of points 
         *  @param [in] stride Offset between two consecutive points. (default=0)
         */
        ContainmentType::Value contains(const float* buffer, size_t count, size_t stride=0);
        /** Check if the current bounding sphere contains the specified point.
         *  @param [in] point Point to be tested.
         */
        ContainmentType::Value contains(glm::vec3 const& point);
        /** Check if the current bounding sphere intersects the specified ray.
         *  @param [in] ray Ray to be tested.
         */
        bool intersects(Ray3 const& ray);
        /** Tell on which side of the specified plane the current bounding sphere is.
         *  @param [in] plane Plane.
         */
        Side classify(Plane const& plane) const;
        /** Apply transformation.
         *  @param [in] m 4*4 transformation matrix.
         */
        void transform(glm::mat4 const& m);

        /** Get sphere center. **/
        const glm::vec3& getCenter() const;
        /** Get sphere radius. **/
        float getRadius() const;
        /** Get sphere squared radius. **/
        float getSquareRadius() const;
        
    private:    
        /** Center  */
        glm::vec3 _center;
        /** Radius */
        float _radius;
        /** Square radius */
        float _squareRadius;
};

} // Geometry
} // Core
} // Dumb

#endif // _DUMBFRAMEWORK_BOUNDING_SPHERE_
