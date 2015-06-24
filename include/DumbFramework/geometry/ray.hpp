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
#ifndef _DUMBFRAMEWORK_RAY_
#define _DUMBFRAMEWORK_RAY_

#include <glm/glm.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {
/**
 * N-Dimensional ray.
 **/
template <class vec_t>
struct NDRay
{
    /** Constructor. */
    NDRay();
    /** Constructor. 
     *  @param [in] o  Origin.
     *  @param [in] d  Direction.
     */
    NDRay(vec_t const& o, vec_t const& d);
    /** Copy constructor.
     *  @param [in] r Source ray.
     */
    NDRay(NDRay const& r);

    /** Copy operator.
     *  @param [in] r Source ray.
     */
    NDRay& operator= (NDRay const& r);

    /** Origin */
    vec_t origin;
    /** Direction */
    vec_t direction;
};

/** 3 dimensional floating point ray. **/
typedef NDRay<glm::vec3> Ray3;
/** 2 dimensional floating point ray. **/
typedef NDRay<glm::vec2> Ray2;

} // Geometry
} // Core
} // Dumb

#include "ray.inl"

#endif // _DUMBFRAMEWORK_RAY_
