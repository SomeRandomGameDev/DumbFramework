#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <array>
#include <iostream>
#include <string>

#include <DumbFramework/sprengine.hpp>

using namespace Framework; // temporary

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

const char *s_vertexShader =
"#version 150\n"
"in vec2 vs_position;"
"in vec2 vs_offset;"
"in vec2 vs_dimension;"
"in vec2 vs_toptex;"
"in vec2 vs_bottomtex;"
"in float vs_angle;"
"in float vs_scale;"
"in int vs_index;"
"noperspective centroid out vec2 gs_dimension;"
"out vec2 gs_toptex;"
"out vec2 gs_bottomtex;"
"out vec2 gs_offset;"
"out float gs_angle;"
"out float gs_scale;"
"flat out int gs_index;"
"void main() {"
"gl_Position = vec4(vs_position.x, vs_position.y, 0.0, 1.0);"
"gs_dimension = vs_dimension;"
"gs_toptex = vs_toptex;"
"gs_bottomtex = vs_bottomtex;"
"gs_offset = vs_offset;"
"gs_angle = vs_angle;"
"gs_scale = vs_scale;"
"gs_index = vs_index;"
"}";

// 205, 26 -> 0.20019 , 0,02539
const char *s_geometryShader =
"#version 150\n"
"layout (points) in;"
"layout (triangle_strip, max_vertices = 4) out;"
"noperspective centroid in vec2 gs_dimension[1];"
"in vec2 gs_toptex[1];"
"in vec2 gs_bottomtex[1];"
"in vec2 gs_offset[1];"
"in float gs_angle[1];"
"in float gs_scale[1];"
"flat in int gs_index[1];"
"uniform mat4 pMatrix;"
"out vec2 fs_tex;"
"flat out int fs_index;"
"void main() {"
"vec2 dim = gs_dimension[0] / 2.0;"
"float ca = cos(gs_angle[0]);"
"float sa = sin(gs_angle[0]);"
"float sc = gs_scale[0];"
"fs_index = gs_index[0];"
"vec2 tpos = vec2(-dim.x * ca + dim.y * sa, -dim.y * ca - dim.x * sa) * sc;"
"vec4 pos = gl_in[0].gl_Position + vec4(gs_offset[0].x, gs_offset[0].y, 0, 0);"
"gl_Position = pMatrix * vec4(pos.x + tpos.x, pos.y + tpos.y, 0.0, 1.0);"
"fs_tex = gs_toptex[0];"
"EmitVertex();"
"tpos = vec2(dim.x * ca + dim.y * sa, -dim.y * ca + dim.x * sa) * sc;"
"gl_Position = pMatrix * vec4(pos.x + tpos.x, pos.y + tpos.y, 0.0, 1.0);"
"fs_tex = vec2(gs_bottomtex[0].x, gs_toptex[0].y);"
"EmitVertex();"
"tpos = vec2(-dim.x * ca - dim.y * sa, dim.y * ca - dim.x * sa) * sc;"
"gl_Position = pMatrix * vec4(pos.x + tpos.x, pos.y + tpos.y, 0.0, 1.0);"
"fs_tex = vec2(gs_toptex[0].x, gs_bottomtex[0].y);"
"EmitVertex();"
"tpos = vec2(dim.x * ca - dim.y * sa, dim.y * ca + dim.x * sa) * sc;"
"gl_Position = pMatrix * vec4(pos.x + tpos.x, pos.y + tpos.y, 0.0, 1.0);"
"fs_tex = gs_bottomtex[0];"
"EmitVertex();"
"EndPrimitive();"
"}"
;

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
#define VBO_STRIDE (sizeof(GLfloat) * 12 + sizeof(GLuint))

namespace Sprite {

    // Dumb design ahead.

    Engine::Engine(Atlas *atlas, unsigned int capacity) :
        _atlas(atlas),
        _vao(0),
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

            // VAO+VBO init.
            _buffer.create(GL_ARRAY_BUFFER, VBO_STRIDE*capacity, GL_STREAM_DRAW);

            glGenVertexArrays(1, &_vao);
            glBindVertexArray(_vao);
            glEnableVertexAttribArray(VERTEX_INDEX);
            glEnableVertexAttribArray(OFFSET_INDEX);
            glEnableVertexAttribArray(SIZE_INDEX);
            glEnableVertexAttribArray(TOP_TEX_INDEX);
            glEnableVertexAttribArray(DOWN_TEX_INDEX);
            glEnableVertexAttribArray(ROTATE_INDEX);
            glEnableVertexAttribArray(SCALE_INDEX);
            _buffer.bind();
            glVertexAttribPointer(VERTEX_INDEX,
                    2, GL_FLOAT,
                    GL_FALSE,
                    VBO_STRIDE,
                    (GLvoid *) 0);
            glVertexAttribPointer(OFFSET_INDEX,
                    2, GL_FLOAT,
                    GL_FALSE,
                    VBO_STRIDE,
                    (GLvoid *) (sizeof(float) * 2));
            glVertexAttribPointer(SIZE_INDEX,
                    2, GL_FLOAT,
                    GL_FALSE,
                    VBO_STRIDE,
                    (GLvoid *) (sizeof(float) * 4));
            glVertexAttribPointer(TOP_TEX_INDEX,
                    2, GL_FLOAT,
                    GL_FALSE,
                    VBO_STRIDE,
                    (GLvoid *) (sizeof(float) * 6));
            glVertexAttribPointer(DOWN_TEX_INDEX,
                    2, GL_FLOAT,
                    GL_FALSE,
                    VBO_STRIDE,
                    (GLvoid *) (sizeof(float) * 8));
            glVertexAttribPointer(ROTATE_INDEX,
                    1, GL_FLOAT,
                    GL_FALSE,
                    VBO_STRIDE,
                    (GLvoid *) (sizeof(float) * 10));
            glVertexAttribPointer(SCALE_INDEX,
                    1, GL_FLOAT,
                    GL_FALSE,
                    VBO_STRIDE,
                    (GLvoid *) (sizeof(float) * 11));
            glVertexAttribPointer(TEXTURE_INDEX,
                    1, GL_UNSIGNED_INT,
                    GL_FALSE,
                    VBO_STRIDE,
                    (GLvoid *) (sizeof(float) * 12));
            _buffer.unbind();
            glBindVertexArray(0);

            _time = glfwGetTime();

            // Create program.
            std::array<Framework::Shader, 3> shaders;
            shaders[0].create(Framework::Shader::VERTEX_SHADER,   s_vertexShader  );
            shaders[1].create(Framework::Shader::FRAGMENT_SHADER, s_fragmentShader);
            shaders[2].create(Framework::Shader::GEOMETRY_SHADER, s_geometryShader);

            _program.create();
            for(unsigned long i=0; i<shaders.size(); i++)
            {
                shaders[i].infoLog(Framework::Severity::Info);
                _program.attach(shaders[i]);
            }

            _program.bindAttribLocation(VERTEX_INDEX,   "vs_position" );
            _program.bindAttribLocation(OFFSET_INDEX,   "vs_offset"   );
            _program.bindAttribLocation(SIZE_INDEX,     "vs_dimension");
            _program.bindAttribLocation(TOP_TEX_INDEX,  "vs_toptex"   );
            _program.bindAttribLocation(DOWN_TEX_INDEX, "vs_bottomtex");
            _program.bindAttribLocation(ROTATE_INDEX,   "vs_angle"    );
            _program.bindAttribLocation(SCALE_INDEX,    "vs_scale"    );
            _program.bindAttribLocation(TEXTURE_INDEX,  "vs_index"    );

            _program.link();
            _program.infoLog(Framework::Severity::Info);
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
        
        // VBO/VAO deletion.
        if(_vao != 0) {
            glDeleteVertexArrays(1, &_vao);
            _buffer.destroy();
        }
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
            Log_Error(Framework::Module::Render, "No Definition in Atlas for identifier (%d)", definitionId);
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
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, height, 0, 0, 256.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
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
        // Animate the sprites
        animate();

        // Retrieve buffer and memcpy.
        glDepthMask(GL_FALSE);
        glEnable(GL_TEXTURE_2D_ARRAY);
        _program.begin();
            glUniformMatrix4fv(_uniformMatrix, 1, GL_FALSE, glm::value_ptr(_matrix));
            glUniform1i(_uniformTexture, 0);
            glActiveTexture(GL_TEXTURE0);
            _texture->bind();
            GLfloat *ptr = (GLfloat *) _buffer.map(Render::BufferObject::BUFFER_WRITE);
            memcpy(ptr, _cell, VBO_STRIDE * _count);
            _buffer.unmap();

            // Send VAO.
            glBindVertexArray(_vao);
            glDrawArrays(GL_POINTS, 0, _count);
            glBindVertexArray(0);
        _program.end();
        glDepthMask(GL_TRUE);
    }

}
