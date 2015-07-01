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
#ifndef _DUMBFRAMEWORK_TRANSFORM_
#define _DUMBFRAMEWORK_TRANSFORM_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

/**
 * 3D transformation.
 */
class Transform
{
    public:
        /**
         * Default constructor.
         */
        Transform();
        /**
         * Constructor.
         * @param [in] orientation Orientation.
         * @param [in] position    Position.
         */
        Transform(glm::fquat const& orientation, glm::vec3 const& position);
        /**
         * Copy constructor.
         */
        Transform(Transform const& t);
        /**
         * Copy operator.
         */
        Transform& operator=(Transform const& t);
        /**
         * Retrieve up vector.
         */
        glm::vec3 up() const;
        /**
         * Retrieve right vector.
         */
        glm::vec3 right() const;
        /**
         * Retrieve forward vector.
         */
        glm::vec3 forward() const;
        /**
         * Retrieve euler angles.
         */
        glm::vec3 eulerAngles() const;
        /**
         * Return local transform matrix.
         */
        glm::mat4 local() const;
        /**
         * Interpolate between 2 transform.
         */
        friend Transform mix(Transform const& from, Transform const& to, float t);
        /**
         * Concatenate transforms.
         */
        friend Transform concat(Transform const& from, Transform const& to);
        /** Get orientation. **/
        glm::fquat orientation() const;
        /** Get position. **/
        glm::vec3  position() const;
        /** 
         * Set orientation.
         * @param [in] q Orientation.
         */
        void orientation(glm::fquat const& q);
        /** 
         * Set position. 
         * @param [in] p Position.
         */
        void position(glm::vec3 const& p);
        /**
         * Directly transform a point.
         * @param [in] p Input position.
         */
         glm::vec3 transform(glm::vec3 const& p) const;
         
    private:
        /** Transformations are represented internally as a dual quaternion. **/
        glm::fdualquat _q;
};

} // Geometry
} // Core
} // Dumb

#endif // _DUMBFRAMEWORK_TRANSFORM_
