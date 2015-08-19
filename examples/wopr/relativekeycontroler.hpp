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
#ifndef _RELATIVE_KEY_CONTROLER_H_
#define _RELATIVE_KEY_CONTROLER_H_

#include "inputcontroler.hpp"

namespace WOPR {
/// Move cursor around the board.
class RelativeKeyControler : public InputControler
{
    public:
        /// Key indices.
        enum Keys
        {
            Up,
            Down,
            Left,
            Right,
            Put,
            KeyCount
        };
    public:
        /// Default constructor.
        RelativeKeyControler();
        /// Constructor. 
        /// @param [in] up    Move up key.
        /// @param [in] down  Move down key.
        /// @param [in] left  Move left key.
        /// @param [in] right Move right key.
        /// @param [in] put   Put piece key.
        RelativeKeyControler(int up, int down, int left, int right, int put);
        /// Destructor.
        ~RelativeKeyControler();
        /// Initialize controler.
        /// @param [in] up    Move up key.
        /// @param [in] down  Move down key.
        /// @param [in] left  Move left key.
        /// @param [in] right Move right key.
        /// @param [in] put   Put piece key.
        void init(int up, int down, int left, int right, int put);
        /// Compute board position from current input infos.
        /// [todo]
        /// @param [in]  input    Current input infos.
        /// @param [out] position Board position.
        /// @return @b true if a piece is put, @b false otherwise.
        bool update(Input const& input, glm::ivec2& position);
    public:
        /// Default key values.
        static const int DefaultKeys[KeyCount];
    protected:
        /// Key configuration.
        int _keys[KeyCount];
        /// Current board position.
        glm::ivec2 _position;
};

} // WOPR

#endif // _RELATIVE_KEY_CONTROLER_H_
