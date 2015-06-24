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

namespace Dumb     {
namespace Core     {
namespace Geometry {

/**
 * Simple rectangle.
 */
class Rect
{
    public:
        /** Default constructor. **/
        Rect();
        /**
         * Constructor.
         * @param [in] p Upper left corner.
         * @param [in] s Rectangle size.
         */
        Rect(glm::vec2 const& position, glm::vec2 const& size);
        /**
         * Copy constructor.
         * @param [in] r Source rectangle.
         */
        Rect(Rect const& r);
        /**
         * Copy operator.
         * @param [in] r Source rectangle.
         */
        Rect& operator=(Rect const& r);
        /**
         * Compute minimum point.
         */
        inline glm::vec2 min() const;
        /**
         * Compute maximum point.
         */
        inline glm::vec2 max() const;
        /**
         * Compute center.
         */
        inline glm::vec2 center() const;
        /**
         * Compute extent.
         */
        inline glm::vec2 extent() const;
        
    public:
        /** Upper left corner. **/
        glm::vec2 position;
        /** Size. **/
        glm::vec2 size;
};

} // Geometry
} // Core
} // Dumb

#include "rect.inl"

#endif // _DUMBFRAMEWORK_BOUNDING_QUAD_
