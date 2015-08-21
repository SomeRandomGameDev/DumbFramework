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
#ifndef _DISPLAY_CONFIG_H_
#define _DISPLAY_CONFIG_H_

#include <DumbFramework/runner.hpp>

namespace WOPR {

/// Display configuration.
struct DisplayConfig
{
    /// Default constructor.
    DisplayConfig();
    /// Compute display configuration from current video mode.
    /// @param [in] mode Application video mode.
    void init(Dumb::Core::Application::Video::Mode const& mode);
    /// Screen/window size.
    glm::vec2 screenSize;
    /// Board display size.
    glm::vec2 boardSize;
    /// Global scale.
    float     scale;
    /// Position and size for each cell for a 1600x900 display.
    static const glm::vec2  cell[9][2];
    /// Atlas sprite infos.
    static const glm::vec4  spriteDefs[3];
    /// Sprite anchors.
    static const glm::ivec2 spriteAnchors[3];
};

} // WOPR

#endif // _DISPLAY_CONFIG_H_
