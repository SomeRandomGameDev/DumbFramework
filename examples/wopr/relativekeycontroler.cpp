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
#include "relativekeycontroler.hpp"

namespace WOPR {
/// Default key values.
const int RelativeKeyControler::DefaultKeys[KeyCount] =
{
    GLFW_KEY_E, GLFW_KEY_D, GLFW_KEY_S, GLFW_KEY_F, GLFW_KEY_SPACE
};
/// Default constructor.
RelativeKeyControler::RelativeKeyControler()
    : _position(1,1)
{
    for(int i=0; i<KeyCount; i++)
    {
        _keys[i] = DefaultKeys[i];
    }
}
/// Constructor. 
/// @param [in] up    Move up key.
/// @param [in] down  Move down key.
/// @param [in] left  Move left key.
/// @param [in] right Move right key.
/// @param [in] put   Put piece key.
RelativeKeyControler::RelativeKeyControler(int up, int down, int left, int right, int put)
{
    init(up, down, left, right, put);
}
/// Destructor.
RelativeKeyControler::~RelativeKeyControler()
{}
/// Initialize controler.
/// @param [in] up    Move up key.
/// @param [in] down  Move down key.
/// @param [in] left  Move left key.
/// @param [in] right Move right key.
/// @param [in] put   Put piece key.
void RelativeKeyControler::init(int up, int down, int left, int right, int put)
{
    _keys[Up   ] = up;
    _keys[Down ] = down;
    _keys[Left ] = left;
    _keys[Right] = right;
    _keys[Put  ] = put;
    _position = glm::ivec2(1,1);
}
/// Compute board position from current input infos.
/// [todo]
/// @param [in]  input    Current input infos.
/// @param [out] position Board position.
/// @return @b true if a piece is put, @b false otherwise.
bool RelativeKeyControler::update(Input const& input, glm::ivec2& position)
{
    if(GLFW_PRESS != input.key.action)
    {
        position = _position;
        return false;
    }
    if((input.key.code == _keys[Up]) && (_position.y > 0))
    {
        _position.y--;
    }
    else if((input.key.code == _keys[Down]) && (_position.y < 3)) 
    {
        _position.y++;
    }
    else if((input.key.code == _keys[Left]) && (_position.x > 0))
    {
        _position.x--;
    }
    else if((input.key.code == _keys[Right]) && (_position.x < 3)) 
    {
        _position.x++;
    }
    position = _position;
    return (input.key.code == _keys[Put]);
}

} // WOPR
