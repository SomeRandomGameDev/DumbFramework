#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sprengine.hpp>

#include <iostream>

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
"noperspective centroid out vec2 gs_dimension;"
"out vec2 gs_toptex;"
"out vec2 gs_bottomtex;"
"out vec2 gs_offset;"
"void main() {"
"gl_Position = vec4(vs_position.x, vs_position.y, 0.0, 1.0);"
"gs_dimension = vs_dimension;"
"gs_toptex = vs_toptex;"
"gs_bottomtex = vs_bottomtex;"
"gs_offset = vs_offset;"
"}";

const char *s_geometryShader =
"#version 150\n"
"layout (points) in;"
"layout (triangle_strip, max_vertices = 4) out;"
"noperspective centroid in vec2 gs_dimension[1];"
"in vec2 gs_toptex[1];"
"in vec2 gs_bottomtex[1];"
"in vec2 gs_offset[1];"
"uniform mat4 pMatrix;"
"out vec2 fs_tex;"
"void main() {"
"vec2 dim = gs_dimension[0] / 2.0;"
"vec4 pos = gl_in[0].gl_Position + vec4(gs_offset[0].x, gs_offset[0].y, 0, 0);"
"gl_Position = pMatrix * vec4(pos.x - dim.x, pos.y - dim.y, 0.0, 1.0);"
"fs_tex = gs_toptex[0];"
"EmitVertex();"
"gl_Position = pMatrix * vec4(pos.x + dim.x, pos.y - dim.y, 0.0, 1.0);"
"fs_tex = vec2(gs_bottomtex[0].x, gs_toptex[0].y);"
"EmitVertex();"
"gl_Position = pMatrix * vec4(pos.x - dim.x, pos.y + dim.y, 0.0, 1.0);"
"fs_tex = vec2(gs_toptex[0].x, gs_bottomtex[0].y);"
"EmitVertex();"
"gl_Position = pMatrix * vec4(pos.x + dim.x, pos.y + dim.y, 0.0, 1.0);"
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

// vec2 (pos) + vec2(offset) + vec2 (dim) + vec2 (top-tex) + vec2 (bottom-tex)
#define VBO_STRIDE (sizeof(GLfloat) * 10)

GLuint makeShader(GLenum type, const char *content) {
  GLuint shader;
  GLint length;
  GLint value;

  shader = glCreateShader(type);
  glShaderSource(shader, 1, (const GLchar **) &content, &length);
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
    _vbo(0),
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
    _last(-1) {

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

  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, VBO_STRIDE * capacity, 0, GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glEnableVertexAttribArray(VERTEX_INDEX);
  glEnableVertexAttribArray(OFFSET_INDEX);
  glEnableVertexAttribArray(SIZE_INDEX);
  glEnableVertexAttribArray(TOP_TEX_INDEX);
  glEnableVertexAttribArray(DOWN_TEX_INDEX);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
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
  glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    glDeleteBuffers(1, &_vbo);
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

Identifier Engine::create(unsigned int definitionId, glm::vec2 pos, unsigned int firstAnim, bool cycle) {
  // Size check.
  if(_count == _capacity) {
    return -1;
  }

  // Add an entry into the instance table.
  Identifier inside = _count++;
  Instance *instance = _instance + inside;
  instance->_definition = _atlas->get(definitionId);

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

  (void) set(result, pos, firstAnim, cycle);

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

bool Engine::set(Identifier id, glm::vec2 pos, unsigned int animId, bool cycle, double /* progress */) {
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
        return -1;
      }
    } else {
      return -1;
    }
    // From here, we're safe.
    // 'animation' is not null anymore.
    // Get the first frame.
    Frame *frame = animation->get(0);
    glm::ivec2 offset = frame->getOffset();
    glm::ivec2 size = frame->getSize();
    glm::dvec2 top = frame->getTop();
    glm::dvec2 bottom = frame->getBottom();

    cell->_posX = pos.x;
    cell->_posY = pos.y;
    cell->_offsetX = offset.x;
    cell->_offsetY = offset.y;
    cell->_sizeX = size.x;
    cell->_sizeY = size.y;
    cell->_topU = top.x;
    cell->_topV = top.y;
    cell->_bottomU = bottom.x;
    cell->_bottomV = bottom.y;

    result = true;
  }
  return result;
}

void Engine::viewport(int x, int y, unsigned int width, unsigned int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, height, 0, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  float dw = width / 2.0f;
  float dh = height / 2.0f;

  _matrix = glm::ortho(-dw + x, dw + x, dh + y, -dh + y, 0.0f, 1.0f);
}

void Engine::render() {
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
      glm::ivec2 offset = frame->getOffset();
      glm::ivec2 size = frame->getSize();
      glm::dvec2 top = frame->getTop();
      glm::dvec2 bottom = frame->getBottom();

      cell->_offsetX = offset.x;
      cell->_offsetY = offset.y;
      cell->_sizeX = size.x;
      cell->_sizeY = size.y;
      cell->_topU = top.x;
      cell->_topV = top.y;
      cell->_bottomU = bottom.x;
      cell->_bottomV = bottom.y;  
    }
  }

  // Retrieve buffer and memcpy.
  glBindTexture(0, _texture);
  glUseProgram(_program);
  glUniformMatrix4fv(_uniformMatrix, 1, GL_FALSE, glm::value_ptr(_matrix));
  glUniform1i(_uniformTexture, 0);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  GLfloat *ptr = (GLfloat *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
  memcpy(ptr, _cell, VBO_STRIDE * _count);
  glUnmapBuffer(GL_ARRAY_BUFFER); 

  // Send VAO.
  glBindVertexArray(_vao);
  glDrawArrays(GL_POINTS, 0, _count);
  glBindVertexArray(0);

  glUseProgram(0);
}

}
