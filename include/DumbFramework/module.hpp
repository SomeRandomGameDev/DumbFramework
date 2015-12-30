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
/// @ingroup DUMB_FW_LOG
/// Identifier of the current application module.
class Identifier
{
    public:
        /// Constructor.
        /// @param [in] name Module name.
        Identifier(std::string const& name);
        /// Destructor.
        ~Identifier();
        /// Check if two identifiers are equal.
        /// @param [in] id Module identifier.
        bool operator== (Identifier const& id);
        /// Check if two identifiers are different.
        /// @param [in] id Module identifier.
        bool operator!= (Identifier const& id);
        /// Get module name.
        /// @return Module name.
        std::string const& toString() const;
    private:
        /// Module name.
        std::string _name;
};

/// @defgroup DUMB_FW_LOG_MODULES Module identifiers.
/// @ingroup DUMB_FW_LOG
/// List of module identifiers.

/// Base module identifier.
/// @ingroup DUMB_FW_LOG_MODULES
static const Identifier Base("Base");
/// App module identifier.
/// @ingroup DUMB_FW_LOG_MODULES
static const Identifier App("App");
/// Render module identifier.
/// @ingroup DUMB_FW_LOG_MODULES
static const Identifier Render("Render");
} // Module
} // Dumb

#endif /* _DUMB_FW_MODULE_ */
