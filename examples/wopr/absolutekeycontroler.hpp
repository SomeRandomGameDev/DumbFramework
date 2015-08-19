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
#ifndef _ABSOLUTE_KEY_CONTROLER_H_
#define _ABSOLUTE_KEY_CONTROLER_H_

#include "inputcontroler.hpp"

namespace WOPR {
/// Absolute key controler.
/// A key is associated to each board cell.
/// A piece is put on a cell if the corresponding keys is pressed. 
class AbsoluteKeyControler : public InputControler
{
    public:
        /// Default constructor.
        AbsoluteKeyControler();
        /// Constructor.
        /// @param [in] b0 Key for the upper left cell.
        /// @param [in] b1 Key for the upper center cell.
        /// @param [in] b2 Key for the upper right cell.
        /// @param [in] b3 Key for the middle left cell.
        /// @param [in] b4 Key for the middle center cell.
        /// @param [in] b5 Key for the middle right cell.
        /// @param [in] b6 Key for the lower left cell.
        /// @param [in] b7 Key for the lower center cell.
        /// @param [in] b8 Key for the lower right cell.
        AbsoluteKeyControler(int b0, int b1, int b2, 
                             int b3, int b4, int b5,
                             int b6, int b7, int b8);
        /// Destructor.
        ~AbsoluteKeyControler();
        /// Initialize keys.
        /// @param [in] b0 Key for the upper left cell.
        /// @param [in] b1 Key for the upper center cell.
        /// @param [in] b2 Key for the upper right cell.
        /// @param [in] b3 Key for the middle left cell.
        /// @param [in] b4 Key for the middle center cell.
        /// @param [in] b5 Key for the middle right cell.
        /// @param [in] b6 Key for the lower left cell.
        /// @param [in] b7 Key for the lower center cell.
        /// @param [in] b8 Key for the lower right cell.
        void init(int b0, int b1, int b2, 
                  int b3, int b4, int b5,
                  int b6, int b7, int b8);
        /// Compute board position from current input infos.
        /// A piece is put on a cell if the corresponding keys is pressed. 
        /// @param [in]  input    Current input infos.
        /// @param [out] position Board position.
        /// @return @b true if a piece is put, @b false otherwise.
        bool update(Input const& input, glm::ivec2& position);
    public:
        /// Default key vales.
        static const int DefaultKeys[9];
    protected:
        /// One key per board cell.
        int _keys[9];
};

} // WOPR

#endif // _ABSOLUTE_KEY_CONTROLER_H_
