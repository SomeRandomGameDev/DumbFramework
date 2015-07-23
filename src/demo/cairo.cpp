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

// Here is a simple usage of the libcairo2 within the DumbFramework.
#include <DumbFramework/runner.hpp>

class TestCairo {
    DECLARE_WRAPPER_METHODS
    public:
        TestCairo();
    private:
        /** Quit flag. */
        bool _quit;
        /** Window width */
        int _width;
        /** Window height */
        int _height;
};

TestCairo::TestCairo() : _quit(false), _width(800), _height(600) {}

bool TestCairo::render() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return !_quit;
}

void TestCairo::handleMouseScroll(double, double) {}
void TestCairo::handleMousePosition(double, double) {}

void TestCairo::init(Dumb::Core::Application::Adviser *adviser) {
#if 0
    Dumb::Core::Application::Video::Monitor monitor = adviser->getPrimaryMonitor();
    adviser->setMonitor(monitor);
    Dumb::Core::Application::Video::Mode mode = monitor.getCurrentMode();
    adviser->setVideoMode(mode);
#else
    Dumb::Core::Application::Video::Mode mode(glm::vec2(1024, 768), glm::vec3(8, 8, 8), 60);
    Dumb::Core::Application::Video::Monitor monitor(0);
    adviser->setMonitor(monitor);
    adviser->setVideoMode(mode);
#endif
    glm::vec2 screenSize = mode.getResolution();
    _width = screenSize.x;
    _height = screenSize.y;
    adviser->setTitle("Cairo Test");
}
void TestCairo::handleMouseButton(int, int, int) {}
void TestCairo::handleKey(int key, int, int action, int) {
    _quit = (GLFW_PRESS == action) && (GLFW_KEY_ESCAPE == key);
}
void TestCairo::postInit() {}
void TestCairo::handleWindowClose() {}
void TestCairo::handleWindowSize(int, int) {}
void TestCairo::handleWindowIconify(int) {}
void TestCairo::handleUnicodeCharacter(unsigned int) {}
void TestCairo::handleUnicodeModifierCharacter(unsigned int,int) {}

void TestCairo::close() {}

SIMPLE_APP(TestCairo)
