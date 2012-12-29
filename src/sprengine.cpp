#include <GL/glew.h>
#include <sprengine.hpp>

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
    _last(0.0),
    _elapsed(0.0),
    _capacity(capacity),
    _count(0),
    _used(-1),
    _free(0) {

  _instance = new Instance[capacity];
  _cell = new Cell[capacity];
  _table = new LookupItem[capacity];
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

Identifier Engine::create(unsigned int definitionId, unsigned int firstAnim, bool cycle) {
  // TODO This is a stub.
  return -1;
}

void Engine::destroy(Identifier id) {
  // TODO This is a stub.
}

bool Engine::set(Identifier id, unsigned int animId, bool cycle, double progress) {
  // TODO This is a stub.
  return false;
}

void Engine::viewport(int x, int y, unsigned int width, unsigned int height) {
  // TODO This is a stub.
}

void Engine::render() {
  // TODO This is a stub.
}

}
