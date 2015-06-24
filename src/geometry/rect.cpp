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
#include <DumbFramework/geometry/rect.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

/** Default constructor. **/
Rect::Rect()
{}
/**
* Constructor.
* @param [in] p Upper left corner.
* @param [in] s Rectangle size.
*/
Rect::Rect(glm::vec2 const& p, glm::vec2 const& s)
    : position(p)
    , size(s)
{}
/**
* Copy constructor.
* @param [in] r Source rectangle.
*/
Rect::Rect(Rect const& r)
    : position(r.position)
    , size(r.size)
{}
/**
* Copy operator.
* @param [in] r Source rectangle.
*/
Rect& Rect::operator=(Rect const& r)
{
    position = r.position;
    size     = r.size;
    return *this;
}

} // Geometry
} // Core
} // Dumb
