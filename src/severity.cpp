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
#include <DumbFramework/severity.hpp>

namespace Dumb {

/**
 * Convert severity value to string.
 * Severity | String
 * -------- | -------
 * Info     | info
 * Warning  | warning
 * Error    | error
 * 
 * @return Severity as string. 
 */
char const* Severity::toString() const
{
    switch(value)
    {
        case Severity::Info:
            return "info";
        case Severity::Warning:
            return "warning";
        case Severity::Error:
            return "error";
        default:
            return "     ";
    }
}

} // Dumb
