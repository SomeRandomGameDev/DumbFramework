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

#include "board.h"

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
        
        glm::ivec2 _screenSize;
        glm::ivec2 _mousePosition;
        glm::ivec2 _boardPosition;
        
        Dumb::Sprite::Atlas  *_atlas;
        Dumb::Sprite::Engine *_engine;
        Dumb::Sprite::Cache  *_cache;
        
        Dumb::Sprite::Identifier _boardSpriteId;
        glm::ivec2               _boardSize;
        
        std::vector<Dumb::Sprite::Identifier> _itemSpriteId;
        
        double _start;
        bool   _quit;
        
        static const glm::ivec2 _cell[9][2];
        static const glm::vec4  _spriteDefs[3];
        static const glm::ivec2 _spriteAnchors[3];
};

const glm::ivec2 MainApp::_cell[9][2] =
{
    { glm::ivec2(  0,  0), glm::ivec2(125,125) },
    { glm::ivec2(138,  0), glm::ivec2(120,125) },
    { glm::ivec2(271,  0), glm::ivec2(125,125) },
    { glm::ivec2(  0,138), glm::ivec2(125,120) }, 
    { glm::ivec2(138,138), glm::ivec2(120,120) }, 
    { glm::ivec2(271,138), glm::ivec2(125,120) },
    { glm::ivec2(  0,271), glm::ivec2(125,125) },
    { glm::ivec2(138,271), glm::ivec2(120,125) },
    { glm::ivec2(271,271), glm::ivec2(125,125) }
};

const glm::vec4 MainApp::_spriteDefs[3] =
{
    glm::vec4(  0, 143, 111, 111), // Circle
    glm::vec4(  0,   9, 111, 111), // Cross
    glm::vec4(116,   0, 396, 396)  // Board
};

const glm::ivec2 MainApp::_spriteAnchors[3] =
{
    glm::ivec2( 56,  56), // Circle
    glm::ivec2( 56,  56), // Cross
    glm::ivec2(198, 198)  // Board
};

MainApp::MainApp()
    : _board()
    , _status(WOPR::OK)
    , _turn(0)
    , _screenSize(0)
    , _mousePosition(0)
    , _boardPosition(-1)
    , _atlas(nullptr)
    , _engine(nullptr)
    , _cache(nullptr)
    , _boardSpriteId(-1)
    , _boardSize(0)
    , _start(0.0)
    , _quit(false)
{}

void MainApp::init(Dumb::Core::Application::Adviser *adviser)
{
#if 0
    Dumb::Core::Application::Video::Monitor monitor = adviser->getPrimaryMonitor();
    Dumb::Core::Application::Video::Mode mode = monitor.getCurrentMode();
#else
    Dumb::Core::Application::Video::Mode mode(glm::vec2(1024, 768), glm::vec3(8, 8, 8), 60);
    Dumb::Core::Application::Video::Monitor monitor(0);
#endif
    
    adviser->setMonitor(monitor);
    adviser->setVideoMode(mode);
    
    _screenSize = mode.getResolution();
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
        std::string filename = Framework::File::executableDirectory() + "/resources/atlas.png";
        filenameList.push_back(filename);
        
        // Create an Atlas from one texture file with 8 possible Sprite definition slots.
        _atlas = new Dumb::Sprite::Atlas(filenameList, 8);
        
        // Create a Sprite cache from the atlas with 16 sprite instances max.
        _cache = new Dumb::Sprite::Cache(_atlas, 16);
        
        for(int i=0; i<3; i++)
        {
            (void) _atlas->define(i, _spriteDefs[i], _spriteAnchors[i], 0);
        }
        
        _boardSize = glm::ivec2(_spriteDefs[2].z, _spriteDefs[2].w);
        
        // Create board sprite.
        _boardSpriteId = _cache->create(2, _screenSize/2);
        
        _engine = new Dumb::Sprite::Engine(32, _atlas);
    }
    
    _engine->viewport(0, 0, _screenSize.x, _screenSize.y);

    glViewport(0, 0, _screenSize.x, _screenSize.y);
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
    if( (WOPR::Draw  == _status) || 
        (WOPR::Win   == _status) || 
        (WOPR::Loose == _status) )
    {
        double elapsed = glfwGetTime() - _start;
        if(elapsed >= 2.0)
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
            _board.find_best(item, _boardPosition.x, _boardPosition.y);
        }
        _status = _board.put(_boardPosition.x, _boardPosition.y, item);

        bool gameEnded = (WOPR::Draw  == _status) || (WOPR::Win   == _status) || (WOPR::Loose == _status);

        if((WOPR::OK    == _status) || gameEnded)
        {
            Dumb::Sprite::Identifier id;
            unsigned int index = _boardPosition.x + (3*_boardPosition.y);
            glm::ivec2 itemPos = _cell[index][0] + _cell[index][1]/2;
            
            itemPos += (_screenSize - _boardSize) / 2;
            id = _cache->create(_turn, itemPos);
            
            _itemSpriteId.push_back(id);
            
            _boardPosition.x = _boardPosition.y = -1;
            _turn ^= 1;
            
            if(gameEnded)
            {
                _start = glfwGetTime();
                Log_Info(Framework::Module::Base, "%s", (WOPR::Draw == _status) ? "Draw" : ((WOPR::Win == _status) ? "You loose" : "You win"));
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
    _mousePosition.x = x;
    _mousePosition.y = y;
}

void MainApp::handleMouseButton(int button, int action, int /* mods */)
{
    if((GLFW_PRESS == action) && (GLFW_MOUSE_BUTTON_LEFT == button))
    {
        glm::ivec2 bmin = (_screenSize - _boardSize) / 2;
        glm::ivec2 bmax = (_screenSize + _boardSize) / 2;
        
        _boardPosition.x = _boardPosition.y = -1;

        if(((_mousePosition.x > bmin.x) && (_mousePosition.x < bmax.x)) &&
           ((_mousePosition.y > bmin.y) && (_mousePosition.y < bmax.y)))
        {
            glm::ivec2 pos = _mousePosition - bmin;
            bool found = false;
            for(int j=0, k=0; (j<3) && (!found); j++)
            {
                for(int i=0; (i<3) && (!found); i++, k++)
                {
                    bmin = _cell[k][0];
                    bmax = bmin + _cell[k][1];
                    
                    if( ((pos.x >= bmin.x) && (pos.x <= bmax.x)) &&
                        ((pos.y >= bmin.y) && (pos.y <= bmax.y)) )
                    {
                        _boardPosition.x = i;
                        _boardPosition.y = j;
                        found = true;
                    }
                }
            }
        }
    }
}

void MainApp::handleKey(int key, int /* scancode */, int action, int /* mods */)
{
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
