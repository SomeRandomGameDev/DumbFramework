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
#include <GLFW/glfw3.h>
#include "absolutekeycontroler.hpp"

namespace WOPR {
/// Default key vales.
const int AbsoluteKeyControler::DefaultKeys[9] =
{
    GLFW_KEY_E, GLFW_KEY_R, GLFW_KEY_T,
    GLFW_KEY_D, GLFW_KEY_F, GLFW_KEY_G,
    GLFW_KEY_C, GLFW_KEY_V, GLFW_KEY_B
};
/// Default constructor.
AbsoluteKeyControler::AbsoluteKeyControler()
{
    for(int i=0; i<9; i++)
    {
        _keys[i] = DefaultKeys[i];
    }
}
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
AbsoluteKeyControler::AbsoluteKeyControler(int b0, int b1, int b2, 
                                           int b3, int b4, int b5,
                                           int b6, int b7, int b8)
{
    init(b0, b1, b2, b3, b4, b5, b6, b7, b8);
}
/// Destructor.
AbsoluteKeyControler::~AbsoluteKeyControler()
{}
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
void AbsoluteKeyControler::init(int b0, int b1, int b2, 
                                int b3, int b4, int b5,
                                int b6, int b7, int b8)
{
    _keys[0] = b0; _keys[1] = b1; _keys[2] = b2;
    _keys[3] = b3; _keys[4] = b4; _keys[5] = b5;
    _keys[6] = b6; _keys[7] = b7; _keys[8] = b8;
}
/// Compute board position from current input infos.
/// A piece is put on a cell if the corresponding keys is pressed. 
/// @param [in]  input    Current input infos.
/// @param [out] position Board position.
/// @return @b true if a piece is put, @b false otherwise.
bool AbsoluteKeyControler::update(Input const& input, glm::ivec2 &position)
{
    if(GLFW_PRESS == input.key.action)
    {
        for(position.y=0; position.y<3; position.y++)
        {
            for(position.x=0; position.x<3; position.x++)
            {
                int index = position.x + (3 * position.y);
                if(_keys[index] == input.key.code)
                {
                    return true;
                }
            }
        }
    }    
    position.x = position.y = -1;
    return false;
}

} // WOPR
