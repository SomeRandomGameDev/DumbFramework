#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sprengine.hpp>

#include <iostream>
#include <string>

// Shaders

const char *s_fragmentShader =
"#version 150\n"
"uniform sampler2D texture;"
"in vec2 fs_tex;"
"void main() {"
"gl_FragColor = texture2D(texture, fs_tex); }";

const char *s_vertexShader =
"#version 150\n"
"in vec2 vs_position;"
"in vec2 vs_offset;"
"in vec2 vs_dimension;"
"in vec2 vs_toptex;"
"in vec2 vs_bottomtex;"
"in float vs_angle;"
"in float vs_scale;"
"noperspective centroid out vec2 gs_dimension;"
"out vec2 gs_toptex;"
"out vec2 gs_bottomtex;"
"out vec2 gs_offset;"
"out float gs_angle;"
"out float gs_scale;"
"void main() {"
"gl_Position = vec4(vs_position.x, vs_position.y, 0.0, 1.0);"
"gs_dimension = vs_dimension;"
"gs_toptex = vs_toptex;"
"gs_bottomtex = vs_bottomtex;"
"gs_offset = vs_offset;"
"gs_angle = vs_angle;"
"gs_scale = vs_scale;"
"}";

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
"uniform mat4 pMatrix;"
"out vec2 fs_tex;"
"void main() {"
"vec2 dim = gs_dimension[0] / 2.0;"
"float ca = cos(gs_angle[0]);"
"float sa = sin(gs_angle[0]);"
"float sc = gs_scale[0];"
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

// vec2 (pos) + vec2(offset) + vec2 (dim) +
// vec2 (top-tex) + vec2 (bottom-tex) + rotate + scale
#define VBO_STRIDE (sizeof(GLfloat) * 12)

GLuint makeShader(GLenum type, const char *content) {
    GLuint shader;
    GLint value;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar **) &content, 0);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &value);
    if(!value) {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &value);
        char *log = new char[value];
        glGetShaderInfoLog(shader, value, &value, log);
        std::cout << "##! Error while compiling shader." << std::endl;
        std::cout << log << std::endl;
        glDeleteShader(shader);
        delete[]log;
        shader = 0;
    }
    return shader;
}

namespace Sprite {

    // Dumb design ahead.

    Engine::Engine(Atlas *atlas, unsigned int capacity) :
        _atlas(atlas),
        _vao(0),
        _buffer(),
        _vertexShader(0),
        _fragmentShader(0),
        _geometryShader(0),
        _program(0),
        _texture(0),
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
            _buffer.unbind();
            glBindVertexArray(0);

            _time = glfwGetTime();

            // Create program.
            _vertexShader = makeShader(GL_VERTEX_SHADER, s_vertexShader);
            _fragmentShader = makeShader(GL_FRAGMENT_SHADER, s_fragmentShader);
            _geometryShader = makeShader(GL_GEOMETRY_SHADER, s_geometryShader);
            _program = glCreateProgram();
            glAttachShader(_program, _vertexShader);
            glAttachShader(_program, _fragmentShader);
            glAttachShader(_program, _geometryShader);

            glBindAttribLocation(_program, VERTEX_INDEX, "vs_position");
            glBindAttribLocation(_program, OFFSET_INDEX, "vs_offset");
            glBindAttribLocation(_program, SIZE_INDEX, "vs_dimension");
            glBindAttribLocation(_program, TOP_TEX_INDEX, "vs_toptex");
            glBindAttribLocation(_program, DOWN_TEX_INDEX, "vs_bottomtex");
            glBindAttribLocation(_program, ROTATE_INDEX, "vs_angle");
            glBindAttribLocation(_program, SCALE_INDEX, "vs_scale");

            glLinkProgram(_program);
            GLint status;
            glGetProgramiv(_program, GL_LINK_STATUS, &status);
            if(!status) {
                GLchar *log = 0;
                GLint size = 0;
                glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &size);
                log = new char[size];
                glGetProgramInfoLog(_program, size, &size, log);
                std::cout << "##! Can't link program." << std::endl;
                std::cout << log << std::endl;
                delete []log;
            } else {
                _uniformMatrix = glGetUniformLocation(_program, "pMatrix");
                _uniformTexture = glGetUniformLocation(_program, "texture");
            }
        }

    Engine::~Engine() {
        delete []_instance;
        delete []_cell;
        delete []_table;

        // Program deletion.
        if(_program != 0) {
            glDeleteShader(_vertexShader);
            glDeleteShader(_fragmentShader);
            glDeleteShader(_geometryShader);
            glDeleteProgram(_program);
        }

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

    Identifier Engine::create(unsigned int definitionId, glm::vec2 pos, unsigned int firstAnim, bool cycle, float angle, float scale) {
        // Size check.
        if(_count == _capacity) {
            return -1;
        }

        // Add an entry into the instance table.
        Identifier inside = _count++;
        Instance *instance = _instance + inside;
        instance->_definition = _atlas->get(definitionId);
        if(0 == instance->_definition) {
            std::cerr << "No Definition in Atlas for identifier (" << definitionId << ")" << std::endl;
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

        (void) set(result, pos, firstAnim, cycle, 0.0, angle, scale);

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

    bool Engine::set(Identifier id, glm::vec2 pos, unsigned int animId, bool cycle, double /* progress */, float angle, float scale) {
        bool result = false;
        // TODO Include progress computation.
        if((id < (int) _count) && (_table[id]._free != true)) {
            Identifier inside = _table[id]._target;
            Instance *instance = _instance + inside;
            Cell *cell = _cell + inside;
            Animation *animation = 0;

            if(0 != instance->_definition) {
                instance->_animation = animId;
                instance->_frame = 0;
                instance->_elapsed = 0;
                instance->_cycle = cycle;
                animation = instance->_definition->get(animId);
                if(0 != animation) {
                    instance->_still = animation->capacity()<2;
                } else {
                    return false;
                }
            } else {
                return false;
            }
            // From here, we're safe.
            // 'animation' is not null anymore.
            // Get the first frame.
            Frame *frame = animation->get(0);
            assignFrameToCell(frame, cell, pos.x, pos.y, angle, scale);

            result = true;
        }
        return result;
    }

    void Engine::assignFrameToCell(Frame *frame, Cell *cell, double x, double y, float angle, float scale) {
        glm::ivec2 offset = frame->getOffset();
        glm::ivec2 size = frame->getSize();
        glm::dvec2 top = frame->getTop();
        glm::dvec2 bottom = frame->getBottom();

        cell->_posX = x;
        cell->_posY = y;
        cell->_offsetX = offset.x;
        cell->_offsetY = offset.y;
        cell->_sizeX = size.x;
        cell->_sizeY = size.y;
        cell->_topU = top.x;
        cell->_topV = top.y;
        cell->_bottomU = bottom.x;
        cell->_bottomV = bottom.y;
        cell->_angle = angle;
        cell->_scale = scale;
    }

    void Engine::viewport(float x, float y,
            unsigned int width, unsigned int height,
            float scale) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width, height, 0, 0, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        float dw = (width / 2.0f) * scale;
        float dh = (height / 2.0f) * scale;;

        _scale = scale;
        _width = width;
        _height = height;
        _centerX = x;
        _centerY = y;
        _matrix = glm::ortho(-dw + x, dw + x, dh + y, -dh + y, 0.0f, 1.0f);
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
                Animation *animation = _instance[i]._definition->get(_instance[i]._animation);
                int capacity = animation->capacity();
                int frameNum = _instance[i]._frame;
                bool search = true;
                while(search) {
                    Frame *frame = animation->get(frameNum);
                    double fTime = frame->getTime();
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
                Frame *frame = animation->get(frameNum);
                Cell *cell = _cell + i;
                assignFrameToCell(frame, cell, cell->_posX, cell->_posY, cell->_angle, cell->_scale);
            }
        }
    }

    void Engine::render() {
        // Animate the sprites
        animate();

        // Retrieve buffer and memcpy.
        glBindTexture(0, _texture);
        glUseProgram(_program);
        glUniformMatrix4fv(_uniformMatrix, 1, GL_FALSE, glm::value_ptr(_matrix));
        glUniform1i(_uniformTexture, 0);
        GLfloat *ptr = (GLfloat *) _buffer.map(Render::BufferObject::BUFFER_WRITE);
        memcpy(ptr, _cell, VBO_STRIDE * _count);
        _buffer.unmap();

        // Send VAO.
        glBindVertexArray(_vao);
        glDrawArrays(GL_POINTS, 0, _count);
        glBindVertexArray(0);

        glUseProgram(0);
    }

}
