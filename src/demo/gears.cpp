#include <application.hpp>
#include <demo/gears.hpp>
#include <math.h>
#include <string>
#include <iostream>

static void
gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
     GLint teeth, GLfloat tooth_depth)
{
   GLint i;
   GLfloat r0, r1, r2;
   GLfloat angle, da;
   GLfloat u, v, len;

   r0 = inner_radius;
   r1 = outer_radius - tooth_depth / 2.0;
   r2 = outer_radius + tooth_depth / 2.0;

   da = 2.0 * M_PI / teeth / 4.0;

   glShadeModel(GL_FLAT);

   glNormal3f(0.0, 0.0, 1.0);

   /* draw front face */
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;
      glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
      glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
      if (i < teeth) {
	 glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
	 glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		    width * 0.5);
      }
   }
   glEnd();

   /* draw front sides of teeth */
   glBegin(GL_QUADS);
   da = 2.0 * M_PI / teeth / 4.0;
   for (i = 0; i < teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;

      glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
      glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
      glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
		 width * 0.5);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		 width * 0.5);
   }
   glEnd();

   glNormal3f(0.0, 0.0, -1.0);

   /* draw back face */
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;
      glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
      if (i < teeth) {
	 glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		    -width * 0.5);
	 glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
      }
   }
   glEnd();

   /* draw back sides of teeth */
   glBegin(GL_QUADS);
   da = 2.0 * M_PI / teeth / 4.0;
   for (i = 0; i < teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;

      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		 -width * 0.5);
      glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
		 -width * 0.5);
      glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
      glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
   }
   glEnd();

   /* draw outward faces of teeth */
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i < teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;

      glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
      glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
      u = r2 * cos(angle + da) - r1 * cos(angle);
      v = r2 * sin(angle + da) - r1 * sin(angle);
      len = sqrt(u * u + v * v);
      u /= len;
      v /= len;
      glNormal3f(v, -u, 0.0);
      glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
      glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
      glNormal3f(cos(angle), sin(angle), 0.0);
      glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
		 width * 0.5);
      glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
		 -width * 0.5);
      u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
      v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
      glNormal3f(v, -u, 0.0);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		 width * 0.5);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		 -width * 0.5);
      glNormal3f(cos(angle), sin(angle), 0.0);
   }

   glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
   glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

   glEnd();

   glShadeModel(GL_SMOOTH);

   /* draw inside radius cylinder */
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;
      glNormal3f(-cos(angle), -sin(angle), 0.0);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
   }
   glEnd();
}

GearScene::GearScene() {
  _viewRotX = 20.0f;
  _viewRotY = 30.0f;
  _viewRotZ = 0.0f;
  _angle = 0;
  _gear1 = _gear2 = _gear3 = -1;
}

void GearScene::handleKeyAction(int key, int /* scancode */, int action, int /* mods */) {
  _quit |= (key == GLFW_KEY_ESCAPE);
  if(GLFW_PRESS == action) {
    switch(key) {
      case GLFW_KEY_UP:
        _viewRotX += 5.0;
        break;
      case GLFW_KEY_DOWN:
        _viewRotX -= 5.0;
        break;
      case GLFW_KEY_LEFT:
        _viewRotY += 5.0;
        break;
      case GLFW_KEY_RIGHT:
        _viewRotY -= 5.0;
        break;
      default:
        break;
    }
  }
}

void GearScene::handleMouseButtonAction(int, int) {
}

void GearScene::handleMousePositionAction(double, double) {
}

void GearScene::handleMouseWheelAction(double, double) {
}

void GearScene::handleWindowSize(int width, int height) {
  // This is NOT thread-safe.
  _init = false;
  _width = width;
  _height = height;
}

Scene* GearScene::output() {
  if(!_init) {
    _init = true;
    GLfloat h = (GLfloat)_height / (GLfloat) _width;

    glViewport(0, 0, (GLint) _width, (GLint) _height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -40.0);
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glRotatef(_viewRotX, 1.0, 0.0, 0.0);
  glRotatef(_viewRotY, 0.0, 1.0, 0.0);
  glRotatef(_viewRotZ, 0.0, 0.0, 1.0);

  glPushMatrix();
  glTranslatef(-3.0, -2.0, 0.0);
  glRotatef(_angle, 0.0, 0.0, 1.0);
  glCallList(_gear1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(3.1, -2.0, 0.0);
  glRotatef(-2.0 * _angle - 9.0, 0.0, 0.0, 1.0);
  glCallList(_gear2);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-3.1, 4.2, 0.0);
  glRotatef(-2.0 * _angle - 25.0, 0.0, 0.0, 1.0);
  glCallList(_gear3);
  glPopMatrix();

  glPopMatrix();

  double current = glfwGetTime();
  _angle += 90.0 * (_time - current);
  _time = current;
  ++_frame;

  if(_time - _start > 5.0) {
    double fps = _frame / (_time - _start);
    std::cout << fps << " FPS" << std::endl;
    _frame = 0;
    _start = _time;
  }
  return _quit?0:this;
}
 
void GearScene::pause() {
}

void GearScene::resume(GLFWwindow * /*window*/) {
  static GLfloat pos[4] = { 5.0, 5.0, 10.0, 0.0 };
  static GLfloat red[4] = { 0.8, 0.1, 0.0, 1.0 };
  static GLfloat green[4] = { 0.0, 0.8, 0.2, 1.0 };
  static GLfloat blue[4] = { 0.2, 0.2, 1.0, 1.0 };

  _quit = false;

  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  /* make the gears */
  if(_gear1 == -1) {
    _gear1 = glGenLists(1);
    glNewList(_gear1, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
    gear(1.0, 4.0, 1.0, 20, 0.7);
    glEndList();

    _gear2 = glGenLists(1);
    glNewList(_gear2, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    gear(0.5, 2.0, 2.0, 10, 0.7);
    glEndList();

    _gear3 = glGenLists(1);
    glNewList(_gear3, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    gear(1.3, 2.0, 0.5, 10, 0.7);
    glEndList();

    glEnable(GL_NORMALIZE);
  }
  _start = _time = glfwGetTime();
  _frame = 0;
}


// Main routine.
int main(void) {
  std::string title("GLXgears demo");
  Framework::WindowHint hint(800,600,title);
  Application application;
  GearScene scene;
  application.start(hint, &scene);
  return 0;
}
