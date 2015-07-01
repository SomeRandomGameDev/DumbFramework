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
#ifndef _DUMBFRAMEWORK_SIDE_
#define _DUMBFRAMEWORK_SIDE_

namespace Dumb     {
namespace Core     {
namespace Geometry {

struct Side
{
    enum Value
    {
        Front = 0,
        Back,
        On
    } value;
    inline Side() : value(Back) {}
    inline Side(Value v) : value(v) {}
    inline operator Value() { return value; }
    inline operator const Value() const { return value; }
};

} // Geometry
} // Code
} // Dumb

#endif // _DUMBFRAMEWORK_SIDE_

