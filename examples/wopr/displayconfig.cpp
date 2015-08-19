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
#include "displayconfig.hpp"

namespace WOPR {

/// Position and size for each cell for a 1600x900 display.
const glm::vec2 DisplayConfig::cell[9][2] =
{
    { glm::vec2(  0,  0), glm::vec2(125,125) },
    { glm::vec2(138,  0), glm::vec2(120,125) },
    { glm::vec2(271,  0), glm::vec2(125,125) },
    { glm::vec2(  0,138), glm::vec2(125,120) },
    { glm::vec2(138,138), glm::vec2(120,120) },
    { glm::vec2(271,138), glm::vec2(125,120) },
    { glm::vec2(  0,271), glm::vec2(125,125) },
    { glm::vec2(138,271), glm::vec2(120,125) },
    { glm::vec2(271,271), glm::vec2(125,125) }
};
/// Atlas sprite infos.
const glm::vec4 DisplayConfig::spriteDefs[3] =
{
    glm::vec4(  0, 143, 111, 111), // Circle
    glm::vec4(  0,   9, 111, 111), // Cross
    glm::vec4(116,   0, 396, 396)  // Board
};
/// Sprite anchors.
const glm::ivec2 DisplayConfig::spriteAnchors[3] =
{
    glm::ivec2( 56,  56), // Circle
    glm::ivec2( 56,  56), // Cross
    glm::ivec2(198, 198)  // Board
};
/// Default constructor.
DisplayConfig::DisplayConfig()
    : screenSize(0.0f)
    , boardSize(0.0f)
    , scale(1.0f)
{}
/// Compute display configuration from current video mode.
/// @param [in] mode Application video mode.
void DisplayConfig::init(Dumb::Core::Application::Video::Mode const& mode)
{
    screenSize = mode.getResolution();
    scale      = screenSize.y / 900.0f;
    boardSize  = glm::vec2(spriteDefs[2].z, spriteDefs[2].w) * scale;
}

} // WOPR
