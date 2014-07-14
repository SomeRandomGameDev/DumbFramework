#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <application.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
#include <iostream>
#include <math.h>

#include "texture2d.hpp"

// Shaders

const char *s_vboFragmentShader =
"#version 150\n"
"uniform sampler2D texture;"
"in vec2 fs_tex;"
"void main() {"
"gl_FragColor = texture2D(texture, fs_tex); }";

const char *s_vboVertexShader =
"#version 150\n"
"in vec2 vs_position;"
"in vec2 vs_dimension;"
"in vec2 vs_toptex;"
"in vec2 vs_bottomtex;"
"noperspective centroid out vec2 gs_dimension;"
"out vec2 gs_toptex;"
"out vec2 gs_bottomtex;"
"void main() {"
"gl_Position = vec4(vs_position.x, vs_position.y, 0.0, 1.0);"
"gs_dimension = vs_dimension;"
"gs_toptex = vs_toptex;"
"gs_bottomtex = vs_bottomtex;"
"}";

const char *s_vboGeometryShader =
"#version 150\n"
"layout (points) in;"
"layout (triangle_strip, max_vertices = 4) out;"
"noperspective centroid in vec2 gs_dimension[1];"
"in vec2 gs_toptex[1];"
"in vec2 gs_bottomtex[1];"
"uniform mat4 pMatrix;"
"out vec2 fs_tex;"
"void main() {"
"vec2 dim = gs_dimension[0] / 2.0;"
"vec4 pos = gl_in[0].gl_Position;"
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

// Textures Coords

#define NB_SPRITES 4
#define ATLAS_SIZE 512.0

// 4 coordinates per sprites : Top-UV, Bottom-UV, Offset, Size
glm::vec2 s_spriteCoords[NB_SPRITES * 4] = {
  // Sprite 1
  glm::vec2(13 / ATLAS_SIZE, 5 / ATLAS_SIZE),
  glm::vec2(43 / ATLAS_SIZE, 55 / ATLAS_SIZE),
  glm::vec2(0.0, 0.0),
  glm::vec2(30.0, 50.0),
  // Sprite 2
  glm::vec2(55 / ATLAS_SIZE, 5 / ATLAS_SIZE),
  glm::vec2(85 / ATLAS_SIZE, 55 / ATLAS_SIZE),
  glm::vec2(0.0, 0.0),
  glm::vec2(30.0, 50.0),
  // Sprite 3
  glm::vec2(90 / ATLAS_SIZE, 5 / ATLAS_SIZE),
  glm::vec2(120 / ATLAS_SIZE, 55 / ATLAS_SIZE),
  glm::vec2(0.0, 0.0),
  glm::vec2(30.0, 50.0),
  // Sprite 4
  glm::vec2(125 / ATLAS_SIZE, 5 / ATLAS_SIZE),
  glm::vec2(155 / ATLAS_SIZE, 55 / ATLAS_SIZE),
  glm::vec2(0.0, 0.0),
  glm::vec2(30.0, 50.0)
};

#define NB_POINTS 8

// Program's index.
#define VERTEX_INDEX   0
#define COLOR_INDEX    1
#define SIZE_INDEX     2
#define TOP_TEX_INDEX  3
#define DOWN_TEX_INDEX 4

class TestScene : public Scene {
public:
  TestScene();
  Scene * output();
  void handleKeyAction(int, int, int, int);
  void handleWindowSize(int, int);
  void resume(GLFWwindow *);
  void pause();

private:
  bool _quit;
  bool _init;

  GLuint _vao;

  GLuint _vbo;

  GLuint _vertexShader;
  GLuint _fragmentShader;
  GLuint _geometryShader;
  GLuint _program;

  GLuint _projMatrixId;
  GLuint _textureUniform;

  Render::Texture2D _texture;

  glm::mat4 _projMatrix;

  double _lastTime;
  double _progress;
  double _pastAnimation;

  int _sprite;
};

GLuint createShader(GLenum type, const char *content) {
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

TestScene::TestScene() {
  _quit = false;
  _init = false;
}

void TestScene::handleKeyAction(int key, int, int, int) {
  _quit |= (key == GLFW_KEY_ESCAPE);
}

void TestScene::handleWindowSize(int width, int height) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, height, 0, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  _projMatrix = glm::ortho(0.0f, (float) width, (float) height, 0.0f, 0.0f, 1.0f);
}

void TestScene::resume(GLFWwindow * /* window */) {
  if(_init) {
    return;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  // vec2 (pos) + vec2 (dim) + vec2 (top-tex) + vec2 (bottom-tex)
  GLfloat points[NB_POINTS * (2 + 2 + 2 + 2)];

  int stride = sizeof(float) * (2 + 2 + 2 + 2);

  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), 0, GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glEnableVertexAttribArray(VERTEX_INDEX);
  glEnableVertexAttribArray(SIZE_INDEX);
  glEnableVertexAttribArray(TOP_TEX_INDEX);
  glEnableVertexAttribArray(DOWN_TEX_INDEX);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glVertexAttribPointer(VERTEX_INDEX,
                        2, GL_FLOAT,
                        GL_FALSE,
                        stride,
                        (GLvoid *) 0);
  glVertexAttribPointer(SIZE_INDEX,
                        2, GL_FLOAT,
                        GL_FALSE,
                        stride,
                        (GLvoid *) (sizeof(float) * 2));
  glVertexAttribPointer(TOP_TEX_INDEX,
                        2, GL_FLOAT,
                        GL_FALSE,
                        stride,
                        (GLvoid *) (sizeof(float) * 4));
  glVertexAttribPointer(DOWN_TEX_INDEX,
                        2, GL_FLOAT,
                        GL_FALSE,
                        stride,
                        (GLvoid *) (sizeof(float) * 6));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  _lastTime = glfwGetTime();
  _progress = 0;
  _pastAnimation = 0;
  _sprite = 0;

  // Create program.
  _vertexShader = createShader(GL_VERTEX_SHADER, s_vboVertexShader);
  _fragmentShader = createShader(GL_FRAGMENT_SHADER, s_vboFragmentShader);
  _geometryShader = createShader(GL_GEOMETRY_SHADER, s_vboGeometryShader);
  _program = glCreateProgram();
  glAttachShader(_program, _vertexShader);
  glAttachShader(_program, _fragmentShader);
  glAttachShader(_program, _geometryShader);

  glBindAttribLocation(_program, VERTEX_INDEX, "vs_position");
  glBindAttribLocation(_program, SIZE_INDEX, "vs_dimension");
  glBindAttribLocation(_program, TOP_TEX_INDEX, "vs_toptex");
  glBindAttribLocation(_program, DOWN_TEX_INDEX, "vs_bottomtex");
  // TODO Bind other attributes.
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
    _quit = true;
  } else {
    _projMatrixId = glGetUniformLocation(_program, "pMatrix");
    _textureUniform = glGetUniformLocation(_program, "texture");
    GLuint tId = SOIL_load_OGL_texture("testsprite.png",
                                       SOIL_LOAD_AUTO,
                                       SOIL_CREATE_NEW_ID,
                                       0 );
    if(!_texture.create(tId))
    {
        std::cerr << "ARG!" << std::endl;
        _quit = true;
    }
    else
    {
        _init = true;
    }
  }
}

void TestScene::pause() {
  if(!_init) {
    return;
  }

  glDeleteBuffers(1, &_vbo);


  _init = false;
}

/*
 * DRAW
 */
Scene *TestScene::output() {
  double currentTime = glfwGetTime();
  double elapsed = currentTime - _lastTime;
  _lastTime = currentTime;
  _progress += elapsed;
  _pastAnimation += elapsed;
  if(_pastAnimation > 0.1) {
    _pastAnimation = 0;
    _sprite = (_sprite + 1) % NB_SPRITES;
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  _texture.bind();
  glUseProgram(_program);
  glUniformMatrix4fv(_projMatrixId, 1, GL_FALSE, glm::value_ptr(_projMatrix));
  glUniform1i(_textureUniform, 0);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);

  float *ptr = (float *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
  float *tmp = ptr;
  for(int i = 0; i < NB_POINTS; ++i, tmp += 8) {
    tmp[0] = 100 + (50 * i);
    tmp[1] = 100 + (50 * sin(_progress * (i + 1)));
/*    tmp[0] = 100 + (i/10);
    tmp[1] = 100;*/
    tmp[2] = s_spriteCoords[(_sprite * 4)+ 3].x;
    tmp[3] = s_spriteCoords[(_sprite * 4)+ 3].y;
    tmp[4] = s_spriteCoords[_sprite * 4].x;
    tmp[5] = s_spriteCoords[_sprite * 4].y;
    tmp[6] = s_spriteCoords[(_sprite * 4) + 1].x;
    tmp[7] = s_spriteCoords[(_sprite * 4) + 1].y;
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);

  glBindVertexArray(_vao);
  glDrawArrays(GL_POINTS, 0, NB_POINTS);
  glBindVertexArray(0);

  glUseProgram(0);

  _texture.destroy();

  return _quit?0:this;
}

int main(void) {
  std::string title("Sprite Test");
  TestScene testScene;
  WindowHint hint(800, 600, title);
  Application application;

  application.start(hint, &testScene);

  return 0;
}
