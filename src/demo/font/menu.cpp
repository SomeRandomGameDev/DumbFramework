/*
 * Copyright 2015 Stoned Xander
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
#include <DumbFramework/runner.hpp>
#include <DumbFramework/font.hpp>
#include <DumbFramework/file.hpp>
#include <DumbFramework/log.hpp>

#include <random>
#include <sstream>

#include <glm/ext.hpp>

#define MAX_GLYPHS 1024576

// Some Constants.
static const glm::vec4 COLOR_WHITE = glm::vec4(255, 255, 255, 128);
static const glm::vec4 COLOR_RED   = glm::vec4(255, 96, 96, 255);
static const glm::vec4 COLOR_GREEN = glm::vec4(0, 255, 0, 255);
static const glm::vec4 COLOR_BLUE  = glm::vec4(128, 128, 255, 255);


class Demo {
    DECLARE_WRAPPERS_METHODS
    public:
        Demo() {}
};



SIMPLE_APP(Demo)
