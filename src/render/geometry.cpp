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
#include <DumbFramework/log.hpp>
#include <DumbFramework/render/geometry.hpp>

namespace Dumb      {
namespace Render    {
namespace Geometry  {

/** Default constructor. **/
Attribute::Attribute()
    : type(ComponentType::FLOAT)
    , size(0)
    , normalized(false)
    , stride(0)
    , offset(0)
    , divisor(0)
{}
/** 
 * Constructor
 * @param [in] t   Components type.
 * @param [in] sz  Number of components.
 * @param [in] n   Specify if the data should be normalized or not.
 * @param [in] st  Stride.
 * @param [in] off Offset.  
 * @param [in] d   Divisor (default=0).
 */
Attribute::Attribute(ComponentType t, size_t sz, bool n, size_t st, size_t off, unsigned int d)
    : type(t)
    , size(sz)
    , normalized(n)
    , stride(st)
    , offset(off)
    , divisor(d)
{}
/**
 * Copy constructor.
 * @param [in] attr Input attribute.
 */
Attribute::Attribute(Attribute const& attr)
    : type(attr.type)
    , size(attr.size)
    , normalized(attr.normalized)
    , stride(attr.stride)
    , offset(attr.offset)
    , divisor(attr.divisor)
{}
/** 
 * Copy operator.
 * @param [in] attr Input attribute
 */
Attribute& Attribute::operator=(Attribute const& attr)
{
    type       = attr.type;
    size       = attr.size;
    normalized = attr.normalized;
    stride     = attr.stride;
    offset     = attr.offset;
    divisor    = attr.divisor;
    return *this;
}

} // Geometry
} // Render
} // Dumb
