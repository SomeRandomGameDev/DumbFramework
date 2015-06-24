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
#ifndef _DUMBFRAMEWORK_CAMERA_
#define _DUMBFRAMEWORK_CAMERA_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

/**
 * Camera.
 */
class Camera
{
    public:
        /**
         * Default constructor.
         */
        Camera();
        /**
         * Copy constructor.
         * @param [in] cam    Input camera.
         */
        Camera(Camera const& cam);
        /**
         * Copy operator.
         * @param [in] cam    Input camera.
         */
        Camera& operator=(Camera const& cam);
        /**
         * Set camera position and make it look at a given position.
         * @param [in] eye       Camera position.
         * @param [in] center    Position where the camera is looking at.
         * @param [in] up        Normalized up vector (default (0,1,0)).
         */
        void lookAt(glm::vec3 const& eye, glm::vec3 const& center, glm::vec3 const& up = glm::vec3(0.0f, 1.0f, 0.0f));
        /**
         * Set perspective information.
         * @param [in] fovy    Field of view angle in the y direction.
         * @param [in] near    Distance to the near clipping plane.
         * @param [in] far     Distance to the far clipping plane.
         */
        void perspective(float fovy, float near, float far);
        /**
         * Compute view matrix.
         */
        glm::mat4 viewMatrix() const;
        /**
         * Compute projection matrix for a given screen size.
         * @param [in] size    Screen size.
         */
        glm::mat4 projectionMatrix(glm::ivec2 const& size) const;
        /**
         * Compute screen space position for a given screen size.
         * @param [in] world   World position.
         * @param [in] size    Screen size.
         * @return Screen position between [-1, 1].
         */
        glm::vec3 screenPosition(glm::vec3 const& world, glm::ivec2 const& size) const;
        
        /** Forward vector. **/
        glm::vec3 forward() const;
        /** Up vector. **/
        glm::vec3 up() const;
        /** Right vector. **/
        glm::vec3 right() const;
        
    public:
        /** Position. **/
        glm::vec3 eye;
        /** Orientation. **/
        glm::fquat orientation;
        /** Field of view. **/
        float fov;
        /** Near plane distance. **/
        float near;
        /** Far plane distance. **/
        float far;
};

} // Geometry
} // Core
} // Dumb

#endif // _DUMBFRAMEWORK_CAMERA_

