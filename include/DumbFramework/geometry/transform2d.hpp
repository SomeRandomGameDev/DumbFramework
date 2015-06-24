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
#ifndef _DUMBFRAMEWORK_TRANSFORM_2D_
#define _DUMBFRAMEWORK_TRANSFORM_2D_

#include <glm/glm.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

/**
 * 2D transformation.
 */
class Transform2D
{
    public:

        /**
         * Retrieve up vector.
         */
        inline glm::vec2 up() const;
        /**
         * Retrieve right vector.
         */
        inline glm::vec2 right() const;
        /**
         * Return local transform matrix.
         */
        inline glm::mat3 local() const;
        /**
         * Compute transform matrix.
         */
        inline void update();
        
    public:
        /** Position. **/
        glm::vec2 position;
        /** Rotation angle. **/
        float angle;
        /** Scale. **/
        glm::vec2 scale;
        
    private:
        /** Transformation matrix. **/
        glm::mat3 _local;
};

} // Geometry
} // Core
} // Dumb

#include "transform2d.inl"

#endif // _DUMBFRAMEWORK_TRANSFORM_2D_
