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
#ifndef _DUMB_FW_SEVERITY_
#define _DUMB_FW_SEVERITY_

namespace Dumb {
/**
 * @brief Severity
 * Indicates the severity of the information.
 */
struct Severity
{
    /** Severity values. **/
    enum Value
    {
        Info,       /**< Purely informative. **/
        Warning,    /**< Something wrong or unexpected happened but nothing irremediable. **/
        Error       /**< Something failed. **/
    };
    Value value; /**< Severity value. **/
    /** Default constructor. **/
    inline Severity() {}
    /** 
     * Constructor.
     * @param [in] v Severity value.
     */
    inline Severity(Value v) : value(v) {}
    inline operator Value() { return value; }
    inline operator const Value() const { return value; }
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
    char const* toString() const;
};

} // Dumb

#endif /* _DUMB_FW_SEVERITY_ */
