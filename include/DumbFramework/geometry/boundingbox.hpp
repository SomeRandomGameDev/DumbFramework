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
#ifndef _DUMBFRAMEWORK_BOUNDING_BOX_
#define _DUMBFRAMEWORK_BOUNDING_BOX_

#include <sys/types.h>
#include <glm/glm.hpp>

#include <DumbFramework/geometry/ray.hpp>
#include <DumbFramework/geometry/containment.hpp>
#include <DumbFramework/geometry/plane.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

class BoundingSphere;

/**
 * Axis aligned bounding box.
 */
class BoundingBox
{
    public:
        /** Constructor. */
        BoundingBox();
        /** Constructor. 
         *  @param [in] bmin Bounding box minimum point.
         *  @param [in] bmax Bounding box maximum point.
         */
        BoundingBox(glm::vec3 const& bmin, glm::vec3 const& bmax);
        /** Constructor.
         *  @param [in] buffer Pointer to the point array.
         *  @param [in] count  Number of points 
         *  @param [in] stride Offset between two consecutive points.
         */
        BoundingBox(const uint8_t* buffer, size_t count, size_t stride);
        /** Constructor.
         *  @param [in] sphere Bounding sphere.
         */
        BoundingBox(BoundingSphere const& sphere);
        /** Constructor.
         *  Merge two bounding boxes.
         */
        BoundingBox(BoundingBox const& b0, BoundingBox const& b1);
        /** Copy constructor.
         *  @param [in] box Source bounding box.
         */
        BoundingBox(BoundingBox const& box);
        /** Copy operator.
         *  @param [in] box Source bounding box.
         */
        BoundingBox& operator= (BoundingBox const& box);
        /** Check if the current bounding box contains the specified bounding box. */
        ContainmentType::Value contains(BoundingBox const& box);
        /** Check if the current bounding box contains the specified bounding sphere. */
        ContainmentType::Value contains(BoundingSphere const& sphere);
        /** Check if the current bounding box contains the specified list of points.
         *  @param [in] buffer Pointer to the point array.
         *  @param [in] count  Number of points 
         *  @param [in] stride Offset between two consecutive points. (default=0)
         */
        ContainmentType::Value contains(const float* buffer, size_t count, size_t stride=0);
        /** Check if the current bounding box contains the specified point.
         *  @param [in] point Point to be tested.
         */
        ContainmentType::Value contains(glm::vec3 const& point);
        /** Check if the current bounding box intersects the specified ray.
         *  @param [in] ray Ray to be tested.
         */
        bool intersects(Ray3 const& ray);
        /** Tell on which side of the specified plane the current bounding box is.
         *  @param [in] plane Plane.
         */
        Side classify(Plane const& plane) const;
        /** Apply transformation.
         *  @param [in] m 4*4 transformation matrix.
         */
        void transform(glm::mat4 const& m);
        /** Get lowest box corner. **/
        const glm::vec3& getMin() const;
        /** Get highest box corner. **/
        const glm::vec3& getMax() const;
        /** Get box center. **/
        const glm::vec3& getCenter() const;
        /** Get box extent. **/
        const glm::vec3& getExtent() const;

    private:
        /** Update center and extent. **/
        void _update();
        
    private:
        /** Minimum point **/
        glm::vec3 _min;
        /** Maximum point **/
        glm::vec3 _max;
        /** Center **/
        glm::vec3 _center;
        /** Extent **/
        glm::vec3 _extent;
};

} // Geometry
} // Core
} // Dumb

#endif // _DUMBFRAMEWORK_BOUNDING_BOX_
