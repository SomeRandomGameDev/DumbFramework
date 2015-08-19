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
#ifndef _INPUT_CONTROLER_H_
#define _INPUT_CONTROLER_H_

#include "input.hpp"

namespace WOPR {

/// Input controler interface.
class InputControler
{
    public:
        /// Destructor.
        virtual ~InputControler();
        /// Compute board position from current input infos.
        /// @param [in]  input    Current input infos.
        /// @param [out] position Board position.
        /// @return @b true if a piece is put, @b false otherwise.
        virtual bool update(Input const& input, glm::ivec2& position) = 0;
};

} // WOPR

#endif // _INPUT_CONTROLER_H_
