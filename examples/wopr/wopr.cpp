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
#include <iostream>

#include <DumbFramework/runner.hpp>
#include <DumbFramework/log.hpp>
#include <DumbFramework/sprengine.hpp>
#include <DumbFramework/render/renderer.hpp>

#include "board.hpp"
#include "displayconfig.hpp"
#include "mousecontroler.hpp"
#include "absolutekeycontroler.hpp"
#include "relativekeycontroler.hpp"

namespace WOPR {

class MainApp
{
    DECLARE_WRAPPER_METHODS
    
    public:
        MainApp();
    
    private:
        Board  _board;
        Status _status;
        int    _turn;
        
        glm::ivec2 _boardPosition;

        Input _input;
        
        enum ControlerType
        {
            Mouse,
            AbsoluteKey,
            RelativeKey,
            ControlerTypeCount
        };
        InputControler *_controler[ControlerTypeCount];
        ControlerType   _currentControler;
        
        DisplayConfig _displayCfg;
        
        Dumb::Sprite::Atlas  *_atlas;
        Dumb::Sprite::Engine *_engine;
        Dumb::Sprite::Cache  *_cache;
        
        Dumb::Sprite::Identifier _boardSpriteId;
        std::vector<Dumb::Sprite::Identifier> _itemSpriteId;
        
        double _start;
        bool   _quit;

        static const float      _cpuDelay;
        static const float      _respawnDelay;
};

const float MainApp::_cpuDelay     = 0.350f;
const float MainApp::_respawnDelay = 2.0f;

MainApp::MainApp()
    : _board()
    , _status(WOPR::OK)
    , _turn(0)
    , _boardPosition(-1)
    , _input()
    , _currentControler(Mouse)
    , _displayCfg()
    , _atlas(nullptr)
    , _engine(nullptr)
    , _cache(nullptr)
    , _boardSpriteId(-1)
    , _start(0.0)
    , _quit(false)
{
    for(int i=0; i<ControlerTypeCount; i++)
    {
        _controler[i] = nullptr;
    }
}

void MainApp::init(Dumb::Core::Application::Adviser *adviser)
{
#if 0
    Dumb::Core::Application::Video::Monitor monitor = adviser->getPrimaryMonitor();
    Dumb::Core::Application::Video::Mode mode = monitor.getCurrentMode();
#else
    Dumb::Core::Application::Video::Mode mode(glm::vec2(1024, 576), glm::vec3(8, 8, 8), 60);
    Dumb::Core::Application::Video::Monitor monitor(0);
#endif
    
    adviser->setMonitor(monitor);
    adviser->setVideoMode(mode);
    
    _displayCfg.init(mode);
    
    // [todo] temporary
    _controler[Mouse]       = new MouseControler      (&_displayCfg, GLFW_MOUSE_BUTTON_LEFT);
    _controler[AbsoluteKey] = new AbsoluteKeyControler();
    _controler[RelativeKey] = new RelativeKeyControler();
    
    adviser->setTitle("WOPR");
    
    _itemSpriteId.reserve(9);
    _itemSpriteId.clear();
    _board.clear();
    _turn = 0;
    _status = WOPR::OK;
    _quit = false;
}

void MainApp::postInit()
{
    if(nullptr == _atlas)
    {
        std::vector<std::string> filenameList;
// [todo]
#if defined(BUILD_PACKAGE)
        std::string filename = "/usr/share/wopr/atlas.png";
#else
        std::string filename = Dumb::File::executableDirectory() + "/resources/atlas.png";
#endif // BUILD_PACKAGE

        filenameList.push_back(filename);
        
        // Create an Atlas from one texture file with 8 possible Sprite definition slots.
        _atlas = new Dumb::Sprite::Atlas(filenameList, 8);
        
        // Create a Sprite cache from the atlas with 16 sprite instances max.
        _cache = new Dumb::Sprite::Cache(_atlas, 16);
        
        for(int i=0; i<3; i++)
        {
            (void) _atlas->define(i, _displayCfg.spriteDefs[i], glm::vec2(_displayCfg.spriteAnchors[i]), 0);
        }
        
        // Create board sprite.
        _boardSpriteId = _cache->create(2, _displayCfg.screenSize/2.0f, 0.0f, _displayCfg.scale);
        
        _engine = new Dumb::Sprite::Engine(32, _atlas);
    }
    
    _engine->viewport(0, 0, _displayCfg.screenSize.x, _displayCfg.screenSize.y);

    glViewport(0, 0, _displayCfg.screenSize.x, _displayCfg.screenSize.y);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    Framework::Render::Renderer& renderer = Framework::Render::Renderer::instance();
    renderer.depthTest(false);
    renderer.culling(false);
    renderer.texture2D(true);
    renderer.blend(true);
    renderer.blendFunc(Framework::Render::BlendFunc::SRC_ALPHA,
                       Framework::Render::BlendFunc::ONE_MINUS_SRC_ALPHA);
}

bool MainApp::render()
{
    double elapsed = glfwGetTime() - _start;
    bool put = _controler[_currentControler]->update(_input, _boardPosition);
    
    if( (WOPR::Draw  == _status) || 
        (WOPR::Win   == _status) || 
        (WOPR::Loose == _status) )
    {
        if(elapsed >= _respawnDelay)
        {
            _boardPosition.x = _boardPosition.y = -1;
            _turn = 0;
            _board.clear();
            for(auto id : _itemSpriteId)
            {
                _cache->destroy(id);
            }
            _itemSpriteId.clear();
            _status = WOPR::OK;
        }
    }
    else
    {
        Board::Item item = _turn ? Board::Cross : Board::Circle;
        if(_turn)
        {
            if(elapsed < _cpuDelay)
            {
                _status = WOPR::OutOfBound;
            }
            else
            {
                _board.find_best(item, _boardPosition.x, _boardPosition.y);
                _status = _board.put(_boardPosition.x, _boardPosition.y, item);
            }
        }
        else
        {
            if(true == put)
            {
                _status = _board.put(_boardPosition.x, _boardPosition.y, item);
            }
            else
            {
                _status = WOPR::OutOfBound; // [todo] add a new "Playing" or "OnHold" state.
            }
        }

        bool gameEnded = (WOPR::Draw  == _status) || (WOPR::Win   == _status) || (WOPR::Loose == _status);
        if((WOPR::OK == _status) || gameEnded)
        {
            Dumb::Sprite::Identifier id;
            unsigned int index = _boardPosition.x + (3*_boardPosition.y);
            glm::vec2 itemPos = (_displayCfg.cell[index][0] + _displayCfg.cell[index][1] / 2.0f) * _displayCfg.scale;
            
            itemPos += (_displayCfg.screenSize - _displayCfg.boardSize) / 2.0f;
            id = _cache->create(_turn, itemPos, 0.0f, _displayCfg.scale);
            
            _itemSpriteId.push_back(id);
            
            _boardPosition.x = _boardPosition.y = -1;
            _turn ^= 1;
            
            _start = glfwGetTime();
            if(gameEnded)
            {
                Log_Info(Dumb::Module::App, "%s", (WOPR::Draw == _status) ? "Draw" : ((WOPR::Win == _status) ? "You loose" : "You win"));
            }
        }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _engine->render(_cache);
    return !_quit;
}

void MainApp::close()
{}

void MainApp::handleMouseScroll(double, double)
{}

void MainApp::handleMousePosition(double x, double y)
{
    _input.mouse.position.x = x;
    _input.mouse.position.y = y;
}

void MainApp::handleMouseButton(int button, int action, int mods)
{
    _input.mouse.button = button;
    _input.mouse.action = action;
    _input.mouse.mods   = mods;
}

void MainApp::handleKey(int key, int scancode, int action, int mods)
{
    _input.key.code     = key;
    _input.key.scancode = scancode;
    _input.key.action   = action;
    _input.key.mods     = mods;

    _quit = (GLFW_PRESS == action) && (GLFW_KEY_ESCAPE == key);
}

void MainApp::handleWindowClose()
{}

void MainApp::handleWindowSize(int, int) {}
void MainApp::handleWindowIconify(int) {}
void MainApp::handleUnicodeCharacter(unsigned int) {}
void MainApp::handleUnicodeModifierCharacter(unsigned int,int) {}

} // WOPR

SIMPLE_APP(WOPR::MainApp)
