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
#include <array>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <DumbFramework/sprengine.hpp>
#include <DumbFramework/render/textureloader.hpp>

// Program's index.
#define VERTEX_INDEX   0
#define OFFSET_INDEX   1
#define SIZE_INDEX     2
#define TOP_TEX_INDEX  3
#define DOWN_TEX_INDEX 4
#define ROTATE_INDEX   5
#define SCALE_INDEX    6
#define TEXTURE_INDEX  7

// vec2 (pos) + vec2(offset) + vec2 (dim) +
// vec2 (top-tex) + vec2 (bottom-tex) + rotate + scale + texture
#define VBO_STRIDE (sizeof(float) * 12 + sizeof(unsigned int))

namespace Dumb {
    namespace Sprite {
        // Shaders
        const char *s_dse_fragmentShader = R"Shader(
#version 410 core
        layout (binding=0) uniform sampler2DArray un_texture;
        flat in int fs_index;
        in vec2 fs_tex;
        layout (location=0) out vec4 color;

        void main()
        {
            color = texture(un_texture, vec3(fs_tex, fs_index));
        }
        )Shader";

        const char *s_dse_vertexShaderInstanced = R"Shader(
#version 410 core
        uniform mat4 un_matrix;
        layout (location=0) in vec2 vs_position;
        layout (location=1) in vec2 vs_offset;
        layout (location=2) in vec2 vs_dimension;
        layout (location=3) in vec2 vs_toptex;
        layout (location=4) in vec2 vs_bottomtex;
        layout (location=5) in float vs_angle;
        layout (location=6) in float vs_scale;
        layout (location=7) in int vs_index;
        flat out int fs_index;
        out vec2 fs_tex;
        const vec2 quad[4] = { vec2(0, 0), vec2(0, 1), vec2(1, 0), vec2(1, 1) };
        void main() {
            vec2 point = quad[gl_VertexID];
            vec2 dimPt = vs_dimension * point;
            float cs = cos(vs_angle);
            float sn = sin(vs_angle);
            vec3 rot = vec3(cs, sn, -sn);
            vec2 tpos = vec2(dot(dimPt, rot.xy), dot(dimPt, rot.zx)) * vs_scale;
            fs_tex = mix(vs_toptex, vs_bottomtex, point);
            fs_index = vs_index;
            gl_Position = un_matrix * vec4(vs_position + vs_offset + tpos, 0.0, 1.0);
        }
        )Shader";

        std::vector<std::pair<unsigned int, Framework::Render::Geometry::Attribute>> s_attributes =
            std::vector<std::pair<unsigned int, Framework::Render::Geometry::Attribute>>(
                    {
                    { VERTEX_INDEX,
                    Framework::Render::Geometry::Attribute(
                            Framework::Render::Geometry::ComponentType::FLOAT,
                            2, false, VBO_STRIDE, 0,                  1) },
                    { OFFSET_INDEX,
                    Framework::Render::Geometry::Attribute(
                            Framework::Render::Geometry::ComponentType::FLOAT,
                            2, false, VBO_STRIDE, sizeof(float) *  2, 1) },
                    { SIZE_INDEX,
                    Framework::Render::Geometry::Attribute(
                            Framework::Render::Geometry::ComponentType::FLOAT,
                            2, false, VBO_STRIDE, sizeof(float) *  4, 1) },
                    { TOP_TEX_INDEX,
                    Framework::Render::Geometry::Attribute(
                            Framework::Render::Geometry::ComponentType::FLOAT,
                            2, false, VBO_STRIDE, sizeof(float) *  6, 1) },
                    { DOWN_TEX_INDEX,
                    Framework::Render::Geometry::Attribute(
                            Framework::Render::Geometry::ComponentType::FLOAT,
                            2, false, VBO_STRIDE, sizeof(float) *  8, 1) },
                    { ROTATE_INDEX,
                    Framework::Render::Geometry::Attribute(
                            Framework::Render::Geometry::ComponentType::FLOAT,
                            1, false, VBO_STRIDE, sizeof(float) * 10, 1) },
                    { SCALE_INDEX,
                    Framework::Render::Geometry::Attribute(
                            Framework::Render::Geometry::ComponentType::FLOAT,
                            1, false, VBO_STRIDE, sizeof(float) * 11, 1) },
                    { TEXTURE_INDEX,
                    Framework::Render::Geometry::Attribute(
                            Framework::Render::Geometry::ComponentType::UNSIGNED_INT,
                            1, false, VBO_STRIDE, sizeof(float) * 12, 1) }
                    });

        // ## ATLAS ###########################################################

        // ---------
        Atlas::Atlas(const std::vector<std::string> &paths, unsigned int capacity) {
            bool valid;
            valid = Framework::Render::Texture::load(_texture,
                    paths, Framework::Render::Texture::PixelFormat::RGBA_8);
            if(valid) {
                _texture.bind();
                _texture.setMinFilter(Framework::Render::Texture::MinFilter::LINEAR_TEXEL);
                _texture.setMagFilter(Framework::Render::Texture::MagFilter::LINEAR_TEXEL);
                _texture.setWrap(Framework::Render::Texture::Wrap::CLAMP_TO_EDGE,
                        Framework::Render::Texture::Wrap::CLAMP_TO_EDGE);
                _texture.unbind();
                _size = _texture.size();
                Log_Info(Framework::Module::Render, "Atlas Size : %dx%d", (int)_size.x, (int)_size.y);
            } else {
                _size = glm::vec2(0, 0);
            }
            _sprites = new Sprite[capacity];
            _capacity = capacity;
        }

        // ----------
        Atlas::~Atlas() {
            delete [] _sprites;
        }

        // --------------
        const Sprite *Atlas::define(unsigned int id, glm::vec4 const& coord,
                glm::vec2 const& size, glm::vec2 const& anchor, unsigned int layer) {
            Sprite *result = 0;
            if(id < _capacity) {
                result = _sprites + id;
                result->set(coord, size, anchor, layer);
            }
            return result;
        }

        // --------------
        const Sprite *Atlas::define(unsigned int id, glm::vec4 const& coord,
                glm::vec2 const& anchor, unsigned int layer) {
            double startX = coord.x / _size.x;
            double startY = coord.y / _size.y;
            double endX = (coord.x + coord.z) / _size.x;
            double endY = (coord.y + coord.w) / _size.y;
            glm::vec4 texCoord(startX, startY, endX, endY);
            return define(id, texCoord, glm::vec2(coord.z, coord.w),
                    anchor, layer);
        }


        // ---------------------
        const Sprite *Atlas::get(unsigned int id) const {
            const Sprite *result = 0;
            if(id < _capacity) {
                result = _sprites + id;
            }
            return result;
        }

        // ## CACHE ###########################################################

        Cache::Cache(const Atlas *atlas, unsigned int capacity) :
            _atlas(atlas), _capacity(capacity),
            _count(0), _used(-1), _free(0), _last(0) {
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
        }

        Cache::~Cache() {
            delete []_instance;
            delete []_cell;
            delete []_table;
        }

        void Cache::move(Identifier id, glm::vec2 const& pos) {
            if(!_table[id]._free) {
                Cell * cell = _cell + _table[id]._target;
                cell->_posX = pos.x;
                cell->_posY = pos.y;
            }
        }

        void Cache::rotate(Identifier id, float angle) {
            if(!_table[id]._free) {
                Cell * cell = _cell + _table[id]._target;
                cell->_angle = angle;
            }
        }

        void Cache::setLayer(Identifier id, unsigned int layer) {
            if(!_table[id]._free) {
                Cell * cell = _cell + _table[id]._target;
                Instance * instance = _instance + _table[id]._target;
                // Set the layer.
                set(id, glm::vec2(cell->_posX, cell->_posY),
                        instance->getSprite(), cell->_angle, cell->_scale,
                        layer);
            }
        }

        void Cache::scale(Identifier id, float scale) {
            if(!_table[id]._free) {
                Cell * cell = _cell + _table[id]._target;
                cell->_scale = scale;
            }
        }

        void Cache::copy(Identifier dest, Identifier src) {
            if(!(_table[dest]._free || _table[src]._free)) {
                unsigned int dx = _table[dest]._target;
                unsigned int sx = _table[src]._target;
                _cell[dx] = _cell[sx];
                _instance[dx] = _instance[sx];
            }
        }

        Identifier Cache::clone(Identifier src) {
            Identifier result = -1;
            if(!_table[src]._free) {
                result = create(0, glm::vec2(0, 0), 0, 0.0f, 1.0f);
                if(result >= 0) {
                    copy(result, src);
                }
            }
            return result;
        }

        Identifier Cache::create(unsigned int definitionId, glm::vec2 const& pos,
                float angle, float scale, unsigned int layer) {
            // Size check.
            if(_count == _capacity) {
                return -1;
            }

            // Add an entry into the instance table.
            Identifier inside = _count++;
            const Sprite *sprite = _atlas->get(definitionId);
            if(0 == sprite) {
                Log_Error(Framework::Module::Render,
                        "No Definition in Atlas for identifier (%d)", definitionId);
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
            _instance[inside].set(sprite, result, layer);

            (void) set(result, pos, sprite, angle, scale, layer);

            return result;
        }

        void Cache::destroy(Identifier id) {
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

        bool Cache::set(Identifier id, glm::vec2 const& pos, unsigned int spriteId,
                float angle, float scale, unsigned int layer) {
            return set(id, pos, _atlas->get(spriteId), angle, scale, layer);
        }

        bool Cache::set(Identifier id, glm::vec2 const& pos, const Sprite *sprite,
                float angle, float scale, unsigned int layer) {
            bool result = false;
            if((id < (int) _capacity) && (_table[id]._free != true)) {
                Identifier inside = _table[id]._target;
                Instance *instance = _instance + inside;
                instance->setSprite(sprite);
                while((inside > 0) &&
                        (_instance[inside].getLayer() > _instance[inside - 1].getLayer())) {
                    swapInstances(id, inside, inside - 1);
                    --inside;
                }
                // Check layer and swap to higher.
                while((inside < (int) (_count - 1)) &&
                        (instance[inside].getLayer() < _instance[inside + 1].getLayer())) {
                    swapInstances(id, inside, inside + 1);
                    ++inside;
                }

                assignSpriteToCell(sprite, _cell + inside, pos.x, pos.y, angle, scale);

                result = true;
            }
            return result;
        }

        void Cache::swapInstances(Identifier id, Identifier a, Identifier b) {
            Instance buffer = _instance[a];
            Cell cellBuffer = _cell[a];
            _instance[a] = _instance[b];
            _cell[a] = _cell[b];
            _instance[b] = buffer;
            _cell[b] = cellBuffer;
            _table[id]._target = b;
            _table[_instance[a].getReverse()]._target = a;        
        }

        void Cache::assignSpriteToCell(const Sprite* sprite, Cell *cell,
                double x, double y, float angle, float scale) {
            cell->_posX = x;
            cell->_posY = y;
            glm::vec2 twoVec = sprite->getAnchor();
            cell->_offsetX = -twoVec.x;
            cell->_offsetY = -twoVec.y;
            twoVec = sprite->getSize();
            cell->_sizeX = twoVec.x;
            cell->_sizeY = twoVec.y;
            glm::vec4 texture = sprite->getCoordinates();
            cell->_topU = texture.x;
            cell->_topV = texture.y;
            cell->_bottomU = texture.z;
            cell->_bottomV = texture.w;
            cell->_angle = angle;
            cell->_scale = scale;
            cell->_layer = sprite->getLayer();
        }


        // ## DELEGATE ########################################################

        //   ------------------
        void Delegate::viewport(GLfloat startX, GLfloat startY, GLfloat width, GLfloat height) {
            _matrix = glm::ortho(startX, startX + width, height + startY, startY, -1.0f, 1.0f);
        }

        //    ----------------------------------------------------------------
        std::vector<std::pair<Framework::Render::Shader::Type, const char *> >
            Delegate::shaders() const {
                return std::vector<std::pair<Framework::Render::Shader::Type, const char *> >(
                        {
                        { Framework::Render::Shader::VERTEX_SHADER,   s_dse_vertexShaderInstanced },
                        { Framework::Render::Shader::FRAGMENT_SHADER, s_dse_fragmentShader }
                        });
            }

        //    -----------------------------------------------------------------------
        std::vector<std::pair<unsigned int, Framework::Render::Geometry::Attribute> >
            Delegate::attributes() const {
                return s_attributes;
            }


        //   --------------
        void Delegate::init(Framework::Render::Program &program) {
            _uniformMatrix  = static_cast<GLint>(program.getUniformLocation("un_matrix"));
            _uniformTexture = static_cast<GLint>(program.getUniformLocation("un_texture"));
        }

        //   ----------------
        void Delegate::update(Framework::Render::Program &program) {
            _atlas->bind();
            program.uniform(_uniformMatrix, false, _matrix);
            program.uniform(_uniformTexture, 0);
        }

        //   --------------------
        void Delegate::postRender() {
            _atlas->unbind();
        }

        //      ----------------
        GLsizei Delegate::update(void *ptr, GLsizei capacity,
                const void *cache, unsigned int size) {
            GLsizei result = std::min((GLsizei) size, capacity);
            memcpy(ptr, cache, result * VBO_STRIDE);
            return result;
        }

        //
        GLsizei Delegate::update(void *ptr, GLsizei capacity,
                const Cache *cache) {
            GLsizei result = std::min(capacity, (GLsizei) cache->count());
            cache->copy(ptr, result);
            return result;
        }

    } // 'Sprite' namespace.
} // 'Dumb' namespace.
