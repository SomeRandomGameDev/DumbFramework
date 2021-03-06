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
#include <DumbFramework/module.hpp>

namespace Dumb   {
namespace Module {

/// Constructor.
/// @param [in] name Module name.
Identifier::Identifier(std::string const& name)
    : _name(name)
{}
/// Destructor.
Identifier::~Identifier()
{}
/// Get module name.
/// @return Module name.
std::string const& Identifier::toString() const
{
    return _name;
}
/// Check if two identifiers are equal.
/// @param [in] id Module identifier.
bool Identifier::operator== (Identifier const& id)
{
    return (_name == id._name);
}
/// Check if two identifiers are different.
/// @param [in] id Module identifier.
bool Identifier::operator!= (Identifier const& id)
{
    return (_name != id._name);
}

} // Module
} // Dumb
