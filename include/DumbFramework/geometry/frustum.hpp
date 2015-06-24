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
#ifndef _DUMBFRAMEWORK_FRUSTUM_
#define _DUMBFRAMEWORK_FRUSTUM_

#include <sys/types.h>
#include <glm/glm.hpp>

#include <DumbFramework/geometry/ray.hpp>
#include <DumbFramework/geometry/containment.hpp>
#include <DumbFramework/geometry/plane.hpp>
#include <DumbFramework/geometry/boundingsphere.hpp>
#include <DumbFramework/geometry/boundingbox.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

/**
 * Frustum (truncated pyramid).
 */
class Frustum
{
    public:
        /** Constructor. */
        Frustum();
        /** Build frustum from camera and projection matrices.
         *  @param [in] camera     Camera matrix.
         *  @param [in] projection Projection matrix.
         */
        Frustum(glm::mat4 const& camera, glm::mat4 const& projection);
        /** Copy constructor.
         *  @param [in] frustum Source bounding frustum.
         */
        Frustum(Frustum const& frustum);
        /** Copy operator.
         *  @param [in] frustum Source bounding frustum.
         */
        Frustum& operator= (Frustum const& frustum);
        /** Check if the current bounding frustum contains the specified bounding box. */
        ContainmentType::Value contains(BoundingBox const& box);
        /** Check if the current bounding frustum contains the specified bounding sphere. */
        ContainmentType::Value contains(BoundingSphere const& sphere);
        /** Check if the current bounding frustum contains the specified bounding frustum. */
        ContainmentType::Value contains(Frustum const& frustum);
        /** Check if the current bounding frustum contains the specified list of points.
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
        
        /** Get camera matrix used to build frustum planes. **/
        glm::mat4 const& getCameraMatrix() const;
        /** Get projection matrix used to build frustum planes. **/
        glm::mat4 const& getProjectionMatrix() const;
        /** Get near plane. **/
        Plane const& getNear() const;
        /** Get far plane. **/
        Plane const& getFar() const;
        /** Get top plane. **/
        Plane const& getTop() const;
        /** Get bottom plane. **/
        Plane const& getBottom() const;
        /** Get left plane. **/
        Plane const& getLeft() const;
        /** Get right plane. **/
        Plane const& getRight() const;
        
    private:
        /** Plane names **/
        enum
        {
            FRUSTUM_PLANE_NEAR = 0,
            FRUSTUM_PLANE_FAR,
            FRUSTUM_PLANE_LEFT,
            FRUSTUM_PLANE_RIGHT,
            FRUSTUM_PLANE_TOP,
            FRUSTUM_PLANE_BOTTOM,
            FRUSTUM_PLANE_COUNT
        };
        /** Camera matrix used to build planes. **/
        glm::mat4 _camera;
        /** Projection matrix used to build planes. **/
        glm::mat4 _projection;
        /** Planes. **/
        Plane _planes[FRUSTUM_PLANE_COUNT];
};

} // Geometry
} // Core
} // Dumb

#endif // _DUMBFRAMEWORK_FRUSTUM_
