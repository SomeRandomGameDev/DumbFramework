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
#ifndef _DUMBFRAMEWORK_CIRCLE_
#define _DUMBFRAMEWORK_CIRCLE_

#include <glm/glm.hpp>

#include <DumbFramework/geometry/side.hpp>
#include <DumbFramework/geometry/containment.hpp>
#include <DumbFramework/geometry/ray.hpp>
#include <DumbFramework/geometry/line2.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

/**
 * Bounding circle.
 */
class BoundingCircle
{
    public:
        /** Constructor. */
        BoundingCircle();
        /** Constructor. 
         *  @param [in] c  Bounding circle center.
         *  @param [in] r  Bounding sphere radius.
         */
        BoundingCircle(glm::vec2 const& c, float r);
        /** Constructor.
         *  @param [in] buffer Pointer to the point array.
         *  @param [in] count  Number of points 
         *  @param [in] stride Offset between two consecutive points. (default=0)
         */
        BoundingCircle(const float* buffer, size_t count, size_t stride=0);
        /** Constructor.
         *  Merge two bounding circles.
         */
        BoundingCircle(BoundingCircle const& c0, BoundingCircle const& c1);
        /** Copy constructor.
         *  @param [in] circle Source bounding circle.
         */
        BoundingCircle(BoundingCircle const& circle);

        /** Copy operator.
         *  @param [in] circle Source bounding circle.
         */
        BoundingCircle& operator= (BoundingCircle const& circle);

        /** Check if the current bounding circle contains the specified bounding circle. */
        ContainmentType::Value contains(BoundingCircle const& circle);
        /** Check if the current bounding circle contains the specified list of points.
         *  @param [in] buffer Pointer to the point array.
         *  @param [in] count  Number of points 
         *  @param [in] stride Offset between two consecutive points. (default=0)
         */
        ContainmentType::Value contains(const float* buffer, size_t count, size_t stride=0);
        /** Check if the current bounding circle contains the specified point.
         *  @param [in] point Point to be tested.
         */
        ContainmentType::Value contains(glm::vec2 const& point);
        /** Check if the current bounding circle intersects the specified ray.
         *  @param [in] ray Ray to be tested.
         */
        bool intersects(Ray2 const& ray);
        /** Tell on which side of the specified line the current bounding circle is.
         *  @param [in] line Line.
         */
        Side classify(Line2 const& line);
        /** Apply transformation.
         *  @param [in] m 3*3 transformation matrix.
         */
        void transform(glm::mat3 const& m);

        /** Get circle center. **/
        glm::vec2 const& getCenter() const;
        /** Get circle radius. **/
        float getRadius() const;
        /** Get circle squared radius. **/
        float getSquareRadius() const;
        
        /// @todo : contains/classify rectangle, ray
        
    private:
        /** Center  */
        glm::vec2 _center;
        /** Radius */
        float _radius;
        /** Square radius */
        float _squareRadius;
};

} // Geometry
} // Core
} // Dumb

#endif // _DUMBFRAMEWORK_CIRCLE_
