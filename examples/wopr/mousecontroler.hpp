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
#ifndef _MOUSE_CONTROLER_H_
#define _MOUSE_CONTROLER_H_

#include "inputcontroler.hpp"
#include "displayconfig.hpp"

namespace WOPR {
/// Mouse controler.
/// Choose cell by clicking on screen.
class MouseControler : public InputControler
{
    public:
        /// Default constructor.
        MouseControler();
        /// Constructor.
        /// @param [in] displayCfg  Current display configuration.
        /// @param [in] button      Mouse button.
        MouseControler(DisplayConfig *displayCfg, int button);
        /// Destructor.
        ~MouseControler();
        /// Initialize mouse controler.
        /// @param [in] displayCfg  Current display configuration.
        /// @param [in] button      Mouse button.
        void init(DisplayConfig *displayCfg, int button);
        /// Compute board position from current input infos.
        /// The board position is computed according to the mouse pointer position 
        /// inside board display.
        /// @param [in]  input    Current input infos.
        /// @param [out] position Board position.
        /// @return @b true if a piece is put, @b false otherwise.
        bool update(Input const& input, glm::ivec2& position);
    public:
        /// Default mouse button.
        static const int DefaultButton;
    protected:
        /// Current display configuration.
        DisplayConfig *_display;
        /// Mouse button.
        int _button;
};

} // WOPR

#endif // _MOUSE_CONTROLER_H_
