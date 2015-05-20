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

#include <iostream>

#include <DumbFramework/runner.hpp>
#include <DumbFramework/log.hpp>
#include <DumbFramework/sprengine.hpp>
#include <DumbFramework/render/renderer.hpp>

class TestEngine {
    DECLARE_WRAPPER_METHODS
    public:
        TestEngine() : TestEngine(1024, 768) {}
        TestEngine(int, int);
    private:
        Sprite::Atlas *_atlas;
        Sprite::Engine *_engine;

        Sprite::Identifier _identifier;

        Sprite::Identifier _evilTwin;

        bool _init;
        bool _quit;

        double _start;
        double _elapsed;

        bool _pressed;

        float _initX;
        float _initY;

        float _lastX;
        float _lastY;

        float _centerX;
        float _centerY;

        int _width;
        int _height;

        float _scale;
        float _evilScale;

        int _lastWheelPosition;

        bool _rightPressed;
        bool _middlePressed;
        float _rotate;
};

#define SCALE_FACTOR 0.2f

TestEngine::TestEngine(int w, int h) : _atlas(0), _engine(0), _centerX(0), _centerY(0),
    _width(w), _height(h),
    _scale(1.0), _evilScale(1.0),
    _lastWheelPosition(0), _rightPressed(false),
    _rotate(0) { _pressed = _init = _quit = false; }

    int TestEngine::render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current = glfwGetTime();
        _elapsed += current - _start;
        _start = current;

        //_engine->move(_identifier, glm::vec2(100, 100 + 50*sin(_elapsed)));
        _engine->render();
        return _quit?0:1;
    }

void TestEngine::handleMouseScroll(double, double wheel) {
    int diff = wheel;
    _lastWheelPosition = wheel;
    if(_rightPressed) {
        _rotate += (diff * 5);
        _engine->rotate(_identifier, (_rotate * 3.1415)/180.0f);
    } else {
        float factor = 1;
        if(diff < 0) {
            factor -= SCALE_FACTOR;
        } else if(diff > 0) {
            factor += SCALE_FACTOR;
        }
        if(_pressed) {
            _evilScale *= factor;
        } else {
            _scale *= factor;
        }
        if(_init) {
            if(_pressed) {
                _engine->scale(_evilTwin, _evilScale);
            } else {
                _engine->zoom(_initX, _initY, _scale);
            }
        }
    }
}

void TestEngine::handleMousePosition(double x, double y) {
    if(_pressed) {
        _centerX = _lastX + (_initX - x) * _scale;
        _centerY = _lastY + (_initY - y) * _scale;
        if(_init) _engine->viewport(_centerX, _centerY, _width, _height, _scale);
    } else {
        _initX = x;
        _initY = y;
        _lastX = _centerX;
        _lastY = _centerY;
    }
}

void TestEngine::init(Dumb::Core::Application::Adviser *adviser) {
#ifndef DFE_DEBUG
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
    adviser->setTitle("Sprite Test");
}

void TestEngine::handleMouseButton(int button, int action, int /* mods */) {
    _pressed = (GLFW_PRESS == action) && (GLFW_MOUSE_BUTTON_LEFT == button);
    _rightPressed = (GLFW_PRESS == action) && (GLFW_MOUSE_BUTTON_RIGHT == button);
    _middlePressed = (GLFW_PRESS == action) && (GLFW_MOUSE_BUTTON_MIDDLE == button);
}

void TestEngine::handleKey(int key, int /* scancode */, int action, int /* mods */) {
    static int last = 2;
    _quit = (GLFW_PRESS == action) && (GLFW_KEY_ESCAPE == key);

    if((GLFW_PRESS == action) && (GLFW_KEY_SPACE == key)) {
        _engine->copy(_evilTwin, _identifier);
    }

    if((GLFW_PRESS == action) && (GLFW_KEY_RIGHT_CONTROL == key)) {
        /*  Sprite::Identifier fantom = _engine->clone(_evilTwin);
            if(fantom >= 0) {
            _engine->move(fantom, glm::vec2(0, 0));
            }*/
        _engine->setLayer(_identifier, last);
        last = (last == 0)?2:0;
    }
}

void TestEngine::postInit() {
    if(0 == _atlas) {
        _atlas = new Sprite::Atlas();
        _atlas->read("test.xml");
        _engine = new Sprite::Engine(_atlas, 8);
        _init = true;
    }
    _engine->viewport(_centerX, _centerY, _width, _height);
    glViewport(0, 0, _width, _height);
    glClearColor(1.0f, 0.0f, 1.0f, 0.0f);

    _identifier = _engine->create(0, glm::vec2(100, 100), 0);
    _evilTwin = _engine->create(0, glm::vec2(50, 0), 0, true, 0.0f, 2.0f, 1);
    // Layer 1 is behind
    _start = glfwGetTime();
    _elapsed = 0;

    Framework::Render::Renderer& renderer = Framework::Render::Renderer::instance();
    renderer.depthTest(true);

    renderer.texture2D(true);
    renderer.blend(true);
    renderer.blendFunc(Framework::Render::BlendFunc::SRC_ALPHA, Framework::Render::BlendFunc::ONE_MINUS_SRC_ALPHA);
}

void TestEngine::handleWindowClose() {}
void TestEngine::handleWindowSize(int, int) {}
void TestEngine::handleWindowIconify(int) {}
void TestEngine::handleUnicodeCharacter(unsigned int) {}
void TestEngine::handleUnicodeModifierCharacter(unsigned int,int) {}

void TestEngine::close() {
    _engine->destroy(_identifier);
    _engine->destroy(_evilTwin);
    delete _engine;
    delete _atlas;

    _engine = 0;
    _atlas = 0;
}

SIMPLE_APP(TestEngine)
