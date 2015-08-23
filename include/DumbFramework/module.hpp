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
#ifndef _DUMB_FW_MODULE_
#define _DUMB_FW_MODULE_

#include <string>

namespace Dumb   {
namespace Module {
/// @brief Application module identifier.
/// Identifier of the current application module.
class Identifier
{
    public:
        /// Constructor.
        /// @param [in] name Module name.
        Identifier(std::string const& name);
        /// Destructor.
        ~Identifier();
        /// Get module name.
        /// @return Module name.
        std::string const& toString() const;
    private:
        /// Module name.
        std::string _name;
};

/// Base module identifier.
static const Identifier Base("Base");
/// App module identifier.
static const Identifier App("App");

} // Module
} // Dumb

#endif /* _DUMB_FW_MODULE_ */
