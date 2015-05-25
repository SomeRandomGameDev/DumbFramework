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
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <iostream>
#include <string>

#include <DumbFramework/sprengine.hpp>
#include <DumbFramework/render/renderer.hpp>

// Program's index.
#define VERTEX_INDEX   0
#define OFFSET_INDEX   1
#define SIZE_INDEX     2
#define TOP_TEX_INDEX  3
#define DOWN_TEX_INDEX 4
#define ROTATE_INDEX   5
#define SCALE_INDEX    6
#define TEXTURE_INDEX  7

// Shaders
const char *s_fragmentShader =
"#version 410 core\n"
"layout (binding=0) uniform sampler2DArray un_texture;"
"flat in int fs_index;"
"in vec2 fs_tex;"
"out vec4 out_Color;"
"void main() {"
"out_Color = texture(un_texture, vec3(fs_tex, fs_index));"
" }";

const char *s_vertexShaderInstanced =
"#version 410 core\n"
"uniform mat4 pMatrix;"
"layout (location=0) in vec2 vs_position;"
"layout (location=1) in vec2 vs_offset;"
"layout (location=2) in vec2 vs_dimension;"
"layout (location=3) in vec2 vs_toptex;"
"layout (location=4) in vec2 vs_bottomtex;"
"layout (location=5) in float vs_angle;"
"layout (location=6) in float vs_scale;"
"layout (location=7) in int vs_index;"
"flat out int fs_index;"
"out vec2 fs_tex;"
"const vec2 quad[4] = { vec2(-0.5, 0.5),"
"                       vec2( 0.5, 0.5),"
"                       vec2(-0.5,-0.5),"
"                       vec2( 0.5,-0.5) };"
"void main() {"
"vec2 point = quad[gl_VertexID];"
"vec2 dimPt = vs_dimension * point;"
"float cs = cos(vs_angle);"
"float sn = sin(vs_angle);"
"vec3 rot = vec3(cs, sn, -sn);"
"fs_index = vs_index;"
"fs_tex = mix(vs_toptex, vs_bottomtex, point + vec2(0.5));"
"vec2 tpos = vec2(dot(dimPt, rot.xy), dot(dimPt, rot.zx)) * vs_scale;"
"gl_Position = pMatrix * vec4(vs_position + vs_offset + tpos, 0.0, 1.0);"
"}";

using namespace Framework;

// vec2 (pos) + vec2(offset) + vec2 (dim) +
// vec2 (top-tex) + vec2 (bottom-tex) + rotate + scale + texture
#define VBO_STRIDE (sizeof(float) * 12 + sizeof(unsigned int))

namespace Dumb {
    namespace Sprite {

        // Dumb design ahead.

        Engine::Engine(Atlas *atlas, unsigned int capacity) :
            _atlas(atlas),
            _stream(),
            _buffer(),
            _program(),
            _texture(&atlas->texture()),
            _uniformTexture(0),
            _uniformMatrix(0),
            _matrix(glm::mat4(1.0)),
            _time(0.0),
            _elapsed(0.0),
            _capacity(capacity),
            _count(0),
            _used(-1),
            _free(0),
            _last(-1),
            _centerX(0.0f),
            _centerY(0.0f),
            _scale(1.0f),
            _width(1.0f),
            _height(1.0f) {

                _instance = new Instance[capacity];
                _cell = new Cell[capacity];
                _table = new LookupItem[capacity];
                for(unsigned int i = 0; i < capacity; ++i) {
                    _table[i]._next = i+1;
                    _table[i]._previous = -1;
                    _table[i]._target = -1;
                    _table[i]._free = true;
                }
                _table[capacity-1]._next = -1;

                // vertex buffer + stream init.
                _buffer.create(VBO_STRIDE*capacity);

                _stream.create();
                _stream.add(&_buffer,
                        {
                        { VERTEX_INDEX,   Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT,        2, VBO_STRIDE, 0,                  1) },
                        { OFFSET_INDEX,   Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT,        2, VBO_STRIDE, sizeof(float) *  2, 1) },
                        { SIZE_INDEX,     Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT,        2, VBO_STRIDE, sizeof(float) *  4, 1) },
                        { TOP_TEX_INDEX,  Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT,        2, VBO_STRIDE, sizeof(float) *  6, 1) },
                        { DOWN_TEX_INDEX, Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT,        2, VBO_STRIDE, sizeof(float) *  8, 1) },
                        { ROTATE_INDEX,   Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT,        1, VBO_STRIDE, sizeof(float) * 10, 1) },
                        { SCALE_INDEX,    Render::Geometry::Attribute(Render::Geometry::ComponentType::FLOAT,        1, VBO_STRIDE, sizeof(float) * 11, 1) },
                        { TEXTURE_INDEX,  Render::Geometry::Attribute(Render::Geometry::ComponentType::UNSIGNED_INT, 1, VBO_STRIDE, sizeof(float) * 12, 1) }
                        });
                _stream.compile();

                _time = glfwGetTime();

                // Create program.
                std::array<Render::Shader, 2> shaders;
                shaders[0].create(Render::Shader::VERTEX_SHADER,   s_vertexShaderInstanced);
                shaders[1].create(Render::Shader::FRAGMENT_SHADER, s_fragmentShader);

                _program.create();
                for(unsigned long i=0; i<shaders.size(); i++)
                {
                    shaders[i].infoLog(Severity::Info);
                    _program.attach(shaders[i]);
                }

                _program.link();
                _program.infoLog(Severity::Info);
                _uniformMatrix  = _program.getUniformLocation("pMatrix");
                _uniformTexture = _program.getUniformLocation("un_texture");
            }

        Engine::~Engine() {
            delete []_instance;
            delete []_cell;
            delete []_table;

            // Program deletion.
            _program.destroyShaders();
            _program.destroy();

            // Vertex buffer and stream deletion.
            _stream.destroy();
            _buffer.destroy();
        }

        void Engine::displayTable() {
            std::cout << "-------" << std::endl
                << "Used : " << _used << std::endl
                << "Free : " << _free << std::endl;
            for(unsigned int i = 0; i < _capacity; ++i) {
                std::cout << (_table[i]._free?"#":">") << i << " : ( " << _table[i]._previous
                    << " ) <-- [ " << _table[i]._target
                    << " ] --> ( " << _table[i]._next << " )" << std::endl;
            }
            // Display free.
            std::cout << "Free";
            for(int id = _free; id != -1; id = _table[id]._next) {
                std::cout << " --> " << id;
            }
            std::cout << std::endl << "Used";
            for(int id = _used; id != -1; id = _table[id]._next) {
                std::cout << " --> " << id << " ( " << _table[id]._target << " )";
            }
            std::cout << std::endl;

            // Instance Table.
            std::cout << "## Instances" << std::endl;
            for(unsigned int i = 0; i < _count; ++i) {
                std::cout << "#" << i << " Reverse : " << _instance[i]._reverse << std::endl;
            }
            std::cout << "########" << std::endl;
        }

        void Engine::move(Identifier id, glm::vec2 pos) {
            if(!_table[id]._free) {
                Cell * cell = _cell + _table[id]._target;
                cell->_posX = pos.x;
                cell->_posY = pos.y;
            }
        }

        void Engine::rotate(Identifier id, float angle) {
            if(!_table[id]._free) {
                Cell * cell = _cell + _table[id]._target;
                cell->_angle = angle;
            }
        }

        void Engine::setLayer(Identifier id, unsigned int layer) {
            if(!_table[id]._free) {
                Cell * cell = _cell + _table[id]._target;
                Instance * instance = _instance + _table[id]._target;
                set(id, glm::vec2(cell->_posX, cell->_posY), instance->_animation,
                        instance->_cycle, 0.0, cell->_angle, cell->_scale,
                        layer);
            }
        }

        void Engine::scale(Identifier id, float scale) {
            if(!_table[id]._free) {
                Cell * cell = _cell + _table[id]._target;
                cell->_scale = scale;
            }
        }

        void Engine::copy(Identifier dest, Identifier src) {
            if(!(_table[dest]._free || _table[src]._free)) {
                unsigned int dx = _table[dest]._target;
                unsigned int sx = _table[src]._target;
                _cell[dx] = _cell[sx];
                _instance[dx] = _instance[sx];
            }
        }

        Identifier Engine::clone(Identifier src) {
            Identifier result = -1;
            if(!_table[src]._free) {
                result = create(0, glm::vec2(0, 0), 0, false, 0.0f, 1.0f);
                if(result >= 0) {
                    copy(result, src);
                }
            }
            return result;
        }

        Identifier Engine::create(unsigned int definitionId, glm::vec2 pos,
                unsigned int firstAnim, bool cycle, float angle,
                float scale, unsigned int layer) {
            // Size check.
            if(_count == _capacity) {
                return -1;
            }

            // Add an entry into the instance table.
            Identifier inside = _count++;
            Instance *instance = _instance + inside;
            instance->_definition = _atlas->get(definitionId);
            if(0 == instance->_definition) {
                Log_Error(Module::Render, "No Definition in Atlas for identifier (%d)", definitionId);
            }

            // Add an entry in the lookup table.
            // Get the first free slot.
            Identifier result = _free;
            _free = _table[result]._next;
            _last = result;
            _table[result]._next = _used;
            _table[result]._previous = -1;
            _table[result]._target = inside;
            _table[result]._free = false;
            if(-1 != _used) {
                _table[_used]._previous = result;
            }
            _used = result;
            _instance[inside]._reverse = result;

            (void) set(result, pos, firstAnim, cycle, 0.0, angle, scale, layer);

            return result;
        }

        void Engine::destroy(Identifier id) {
            if((id >= (int) _capacity) || _table[id]._free) {
                return;
            }
            // Search the lookup table.
            Identifier target = _table[id]._target;

            // Instance switch.
            --_count;
            _instance[target] = _instance[_count];
            _cell[target] = _cell[_count];
            _table[_last]._target = target;

            // Lookup table removal.
            _table[id]._free = true;
            int next = _table[id]._next;
            int previous = _table[id]._previous;
            _table[id]._next = _free;
            _table[id]._previous = -1;
            _free = id;
            if(next != -1) {
                _table[next]._previous = previous;
            }
            if(previous != -1) {
                _table[previous]._next = next;
            }
            if(id == _used) {
                _used = next;
            }
        }

        bool Engine::set(Identifier id, glm::vec2 pos, unsigned int animId, bool cycle, double /* progress */, float angle, float scale, unsigned int layer) {
            bool result = false;
            // TODO Include progress computation.
            if((id < (int) _count) && (_table[id]._free != true)) {
                Identifier inside = _table[id]._target;
                Instance *instance = _instance + inside;
                Animation const* animation = 0;

                if(0 != instance->_definition)
                {
                    instance->_animation = animId;
                    instance->_frame = 0;
                    instance->_elapsed = 0;
                    instance->_cycle = cycle;
                    instance->_layer = layer;
                    if(animId < instance->_definition->animationCount())
                    {
                        animation = instance->_definition->getAnimation(animId);
                        instance->_still = (animation->frameCount() < 2);
                    } 
                    else
                    {
                        return false;
                    }
                } else {
                    return false;
                }
                // From here, we're safe.
                // 'animation' is not null anymore.
                // Get the first frame.
                Frame const* frame = animation->getFrame(0);
                // Check layer and swap to lower.
                while((inside > 0) &&
                        (_instance[inside]._layer > _instance[inside - 1]._layer)) {
                    swapInstances(id, inside, inside - 1);
                    --inside;
                }
                // Check layer and swap to higher.
                while((inside < (int) (_count - 1)) &&
                        (instance[inside]._layer < _instance[inside + 1]._layer)) {
                    swapInstances(id, inside, inside + 1);
                    ++inside;
                }

                assignFrameToCell(frame, _cell + inside, pos.x, pos.y, angle, scale);

                result = true;
            }
            return result;
        }

        void Engine::swapInstances(Identifier id, Identifier a, Identifier b) {
            Instance buffer = _instance[a];
            Cell cellBuffer = _cell[a];
            _instance[a] = _instance[b];
            _cell[a] = _cell[b];
            _instance[b] = buffer;
            _cell[b] = cellBuffer;
            _table[id]._target = b;
            _table[_instance[a]._reverse]._target = a;        
        }

        void Engine::assignFrameToCell(Frame const* frame, Cell *cell, double x, double y, float angle, float scale) {
            cell->_posX = x;
            cell->_posY = y;
            cell->_offsetX = frame->offset.x;
            cell->_offsetY = frame->offset.y;
            cell->_sizeX = frame->size.x;
            cell->_sizeY = frame->size.y;
            cell->_topU = frame->top.x;
            cell->_topV = frame->top.y;
            cell->_bottomU = frame->bottom.x;
            cell->_bottomV = frame->bottom.y;
            cell->_angle = angle;
            cell->_scale = scale;
            cell->_layer = frame->layer;
        }

        void Engine::viewport(float x, float y,
                unsigned int width, unsigned int height,
                float scale) {
            float dw = (width / 2.0f) * scale;
            float dh = (height / 2.0f) * scale;;

            _scale = scale;
            _width = width;
            _height = height;
            _centerX = x;
            _centerY = y;
            _matrix = glm::ortho(-dw + x, dw + x, dh + y, -dh + y, 0.0f, 256.0f);
        }

        void Engine::translate(float relX, float relY) {
            _centerX += relX;
            _centerY += relY;
            viewport(_centerX, _centerY, _width, _height);
        }

        void Engine::translate(int relX, int relY) {
            translate(relX * _scale, relY * _scale);
        }

        void Engine::zoom(int x, int y, float scale) {

            float decalX = (x - (_width/2)) * (_scale - scale);
            float decalY = (y - (_height/2)) * (_scale - scale);

            viewport(_centerX + decalX, _centerY + decalY,
                    _width, _height, scale);
        }

        void Engine::animate() {
            // Update animations.
            double current = glfwGetTime();
            double span = current - _time;

            _time = current;
            _elapsed += span;

            // No need to browse the lookup table. We've got all we need right here !
            for(unsigned int i = 0; i < _count; ++i) {
                //_instance
                //_cell
                double elapsed = _instance[i]._elapsed + span;
                if(true != _instance[i]._still) {
                    // Look for next frame.
                    Animation const* animation = _instance[i]._definition->getAnimation(_instance[i]._animation);
                    int capacity = animation->frameCount();
                    int frameNum = _instance[i]._frame;
                    bool search = true;
                    while(search) {
                        Frame const* frame = animation->getFrame(frameNum);
                        double fTime = frame->time;
                        if(elapsed > fTime) {
                            if(frameNum == (capacity - 1)) {
                                if(_instance[i]._cycle) {
                                    elapsed -= fTime;
                                    frameNum = 0;
                                } else {
                                    elapsed = 0;
                                    search = false;
                                    _instance[i]._still = true;
                                }
                            } else {
                                ++frameNum;
                            }
                        } else {
                            search = false;
                        }
                    }
                    _instance[i]._frame = frameNum;
                    _instance[i]._elapsed = elapsed;
                    Frame const* frame = animation->getFrame(frameNum);
                    Cell *cell = _cell + i;
                    assignFrameToCell(frame, cell, cell->_posX, cell->_posY,
                            cell->_angle, cell->_scale);
                }
            }
        }

        void Engine::render() {
            Render::Renderer& renderer = Render::Renderer::instance();

            // Animate the sprites
            animate();
            // Retrieve buffer and memcpy.
            renderer.depthBufferWrite(false);

            _program.begin();
            _program.uniform(_uniformMatrix, false, _matrix);
            _program.uniform(_uniformTexture, 0);

            renderer.setActiveTextureUnit(0);
            _texture->bind();

            _buffer.bind();
            GLfloat *ptr = (GLfloat *) _buffer.map(Render::BufferObject::Access::Policy::WRITE_ONLY);
            memcpy(ptr, _cell, VBO_STRIDE * _count);
            _buffer.unmap();
            _buffer.unbind();

            // Send VAO.
            _stream.bind();
            glDrawArraysInstanced (GL_TRIANGLE_STRIP, 0, 4, _count);
            _stream.unbind();
            _program.end();
            renderer.depthBufferWrite(true);
        }

    } // 'Sprite' namespace.
} // 'Dumb' namespace.
