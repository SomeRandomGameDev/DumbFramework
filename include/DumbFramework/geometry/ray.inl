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
namespace Dumb     {
namespace Core     {
namespace Geometry {

/** Constructor. */
template <class vec_t>
NDRay<vec_t>::NDRay() 
    : origin(0.0f)
    , direction(0.0f)
{}
/** Constructor. 
 *  @param [in] o  Origin.
 *  @param [in] d  Direction.
 */
template <class vec_t>
NDRay<vec_t>::NDRay(vec_t const& o, vec_t const & d)
    : origin(o)
    , direction(glm::normalize(d))
{}
/** Copy constructor.
 *  @param [in] r Source ray.
 */
template <class vec_t>
NDRay<vec_t>::NDRay(NDRay<vec_t> const& r)
    : origin(r.origin)
    , direction(r.direction)
{}
/** Copy operator.
 *  @param [in] r Source ray.
 */
template <class vec_t>
NDRay<vec_t>& NDRay<vec_t>::operator= (NDRay<vec_t> const& r)
{
    origin = r.origin;
    direction = r.direction;
    return *this;
}

} // Geometry
} // Core
} // Dumb
