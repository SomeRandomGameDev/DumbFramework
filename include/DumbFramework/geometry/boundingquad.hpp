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
#ifndef _DUMBFRAMEWORK_BOUNDING_QUAD_
#define _DUMBFRAMEWORK_BOUNDING_QUAD_

#include <glm/glm.hpp>

#include <DumbFramework/geometry/side.hpp>
#include <DumbFramework/geometry/containment.hpp>
#include <DumbFramework/geometry/ray.hpp>
#include <DumbFramework/geometry/line2.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

class BoundingCircle;

/**
 * Axis aligned bounding quad.
 */
class BoundingQuad
{
    public:
        /** Constructor. */
        BoundingQuad();
        /** Constructor. 
         *  @param [in] bmin Bounding quad minimum point.
         *  @param [in] bmax Bounding quad maximum point.
         */
        BoundingQuad(glm::vec2 const& bmin, glm::vec2 const& bmax);
        /** Constructor.
         *  @param [in] buffer Pointer to the point array.
         *  @param [in] count  Number of points 
         *  @param [in] stride Offset between two consecutive points. (default=0)
         */
        BoundingQuad(const float* buffer, size_t count, size_t stride=0);
        /** Constructor.
         *  @param [in] circle Bounding circle.
         */
        BoundingQuad(BoundingCircle const& circle);
        /** Constructor.
         *  Merge two bounding quads.
         */
        BoundingQuad(BoundingQuad const& q0, BoundingQuad const& q1);
        /** Copy constructor.
         *  @param [in] quad Source bounding quad.
         */
        BoundingQuad(BoundingQuad const& quad);
        /** Copy operator.
         *  @param [in] quad Source bounding quad.
         */
        BoundingQuad& operator= (BoundingQuad const& quad);
        /** Check if the current bounding quad contains the specified bounding quad. */
        ContainmentType::Value contains(BoundingQuad const& quad);
        /** Check if the current bounding quad contains the specified bounding circle. */
        ContainmentType::Value contains(BoundingCircle const& circle);
        /** Check if the current bounding quad contains the specified list of points.
         *  @param [in] buffer Pointer to the point array.
         *  @param [in] count  Number of points 
         *  @param [in] stride Offset between two consecutive points. (default=0)
         */
        ContainmentType::Value contains(const float* buffer, size_t count, size_t stride=0);
        /** Check if the current bounding quad contains the specified point.
         *  @param [in] point Point to be tested.
         */
        ContainmentType::Value contains(glm::vec2 const& point);
        /** Check if the current bounding quad intersects the specified ray.
         *  @param [in] ray Ray to be tested.
         */
        bool intersects(Ray2 const& ray);
        /** Tell on which side of the specified line the current bounding quad is.
         *  @param [in] line Line.
         */
        Side classify(Line2 const& line) const;
        /** Apply transformation.
         *  @param [in] m 3*3 transformation matrix.
         */
        void transform(const glm::mat3& m);
        /** Get lowest quad corner. **/
        glm::vec2 const& getMin() const;
        /** Get highest quad corner. **/
        glm::vec2 const& getMax() const;
        /** Get quad center. **/
        glm::vec2 const& getCenter() const;
        /** Get quad extent. **/
        glm::vec2 const& getExtent() const;

    private:
        /** Update center and extent. **/
        void _update();

    private:
        /** Minimum point **/
        glm::vec2 _min;
        /** Maximum point **/
        glm::vec2 _max;
        /** Center **/
        glm::vec2 _center;
        /** Extent **/
        glm::vec2 _extent;
};

} // Geometry
} // Core
} // Dumb

#endif // _DUMBFRAMEWORK_BOUNDING_QUAD_
