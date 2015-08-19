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
#include "mousecontroler.hpp"

namespace WOPR {
/// Default mouse button;
const int MouseControler::DefaultButton = GLFW_MOUSE_BUTTON_LEFT;
/// Default constructor.
MouseControler::MouseControler()
    : _display(nullptr)
    , _button(DefaultButton)
{}
/// Constructor.
/// @param [in] displayCfg  Current display configuration.
/// @param [in] button      Mouse button (Left button by default).
MouseControler::MouseControler(DisplayConfig *displayCfg, int button)
{
    init(displayCfg, button);
}
/// Destructor.
MouseControler::~MouseControler()
{}
/// Initialize mouse controler.
/// @param [in] displayCfg  Current display configuration.
/// @param [in] button      Mouse button.
void MouseControler::init(DisplayConfig *displayCfg, int button)
{
    _display = displayCfg;
    _button  = button;
}
/// Compute board position from current input infos.
/// The board position is computed according to the mouse pointer position 
/// inside board display.
/// @param [in]  input    Current input infos.
/// @param [out] position Board position.
/// @return @b true if a piece is put, @b false otherwise.
bool MouseControler::update(Input const& input, glm::ivec2& position)
{
    if((GLFW_PRESS != input.mouse.action) || (_button != input.mouse.button))
    {
        return false;
    }
    glm::vec2 bmin = (_display->screenSize - _display->boardSize) / 2.0f;
    glm::vec2 bmax = (_display->screenSize + _display->boardSize) / 2.0f;
    
    position.x = position.y = -1;

    if(((input.mouse.position.x > bmin.x) && (input.mouse.position.x < bmax.x)) &&
       ((input.mouse.position.y > bmin.y) && (input.mouse.position.y < bmax.y)))
    {
        glm::vec2 pos = input.mouse.position - bmin;
        for(int j=0, k=0; j<3; j++)
        {
            for(int i=0; i<3; i++, k++)
            {
                bmin = _display->cell[k][0] * _display->scale;
                bmax = bmin + _display->cell[k][1] * _display->scale;
                
                if( ((pos.x >= bmin.x) && (pos.x <= bmax.x)) &&
                    ((pos.y >= bmin.y) && (pos.y <= bmax.y)) )
                {
                    position.x = i;
                    position.y = j;
                    return true;
                }
            }
        }
    }
    return false;
}

} // WOPR
