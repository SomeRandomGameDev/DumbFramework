// ## Should fix the GL/glew.h inclusion !
#include <GL/glew.h>
#include <DumbFramework/application.hpp>
#include <DumbFramework/scene.hpp>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

/*
 * Camera Management.
 * It's a rather simple camera.
 * "Why not directly using glm::lookAt instead ?" will you ask.
 * Because I might change only one of the arguments at a time. It OOP 101
 * you ignorant prick. OOP is not THAT bad if you use it well.
 * By the way, this class is pretty straighforward. Don't expect smooth moves
 * or neat stuff like that. It may comes later, if you deserve it.
 */
class Camera {
    public:
        Camera();

        glm::mat4 getProjection() { return _perspective; }
        glm::mat4 getModel() { return _model; }

        glm::mat4 get() { return _product; }

        void setPerspective(float, float, float, float);
        void setLookAt(glm::vec3, glm::vec3, glm::vec3 = glm::vec3(0, 1, 0));

        // TODO Convenience method : move camera to target,
        // orbit over target, etc.
    private:
        glm::vec3 _position;
        glm::vec3 _up;
        glm::vec3 _look;

        float _fov;
        float _aspect;
        float _zNear;
        float _zFar;

        glm::mat4 _model;
        glm::mat4 _perspective;

        glm::mat4 _product;

    private:

        void update();

        /* glm::mat4 glm::lookAt( glm::vec3 const & eye,
         *                        glm::vec3 const & center,
         *                        glm::vec3 const & up );
         *
         * glm::mat4 glm::perspective( float fovy,
         *                             float aspect,
         *                             float zNear,
         *                             float zFar );
         */
};

/**
 * El Constructor de la Death.
 */
Camera::Camera() : _position(0, 0, 0),
    _up(0, 1, 0),
    _look(1, 0, 0),
    _fov(45.0f), _aspect(4.0f / 3.0f), _zNear(0.1f), _zFar(100.0f),
    _model(glm::lookAt(_look, _position, _up)),
    _perspective(glm::perspective(_fov, _aspect, _zNear, _zFar)),
    _product(_model * _perspective)
{
    // Nothing special to do here.
}

/**
 * Update the perspective (fov/aspect ratio) matrix.
 */
void Camera::setPerspective(float fov, float aspect, float zNear, float zFar) {
    _fov = fov;
    _aspect = aspect;
    _zNear = zNear;
    _zFar = zFar;
    _perspective = glm::perspective(_fov, _aspect, _zNear, _zFar);
    update();
}

/**
 * Update the model matrix.
 */
void Camera::setLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
    _up = up;
    _position = eye;
    _look = center;
    _model = glm::lookAt( _position, _look, _up );
    update();
}

/**
 * Update the overall camera matrix.
 */
void Camera::update() {
    _product = _model * _perspective;
}

/*
 * Declaration.
 */
class TestTerrain : public Scene {
    public:
        TestTerrain();
        virtual ~TestTerrain();

        // Overriden methods.
        Scene *output();

        void handleKeyAction(int, int, int, int);

        void handleWindowSize(int, int);
        void handleMousePositionAction(double, double);

        void resume(GLFWwindow *);

        void pause();

    private:
        bool _quit;

        GLFWwindow* _window;
};


/*
 * Definition.
 */

/**
 * Constructor.
 */
TestTerrain::TestTerrain() {
    _quit = false;
    _window = 0;
}

/**
 * Destructor.
 */
TestTerrain::~TestTerrain() {
}

/**
 * Handle Key Action.
 * @param key Code of the interacted key.
 */
void TestTerrain::handleKeyAction(int key, int, int, int) {
    if(GLFW_KEY_ESCAPE == key) {
        _quit = true;
    }
}

/**
 * Handle window size.
 * Update the graphic context accordingly.
 */
void TestTerrain::handleWindowSize(int w, int h) {
    // glViewPort.
}

/**
 * Handle mouse position.
 * In this case, it will consist mostly in camera movement.
 */
void TestTerrain::handleMousePositionAction(double x, double y) {
}

/**
 * Resume/Init the scene.
 */
void TestTerrain::resume(GLFWwindow *window) {
    _window = window;
    if(_window != 0) {
        int width;
        int height;
        glfwGetWindowSize(window, &width, &height);

        // We can initialize GL context.
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_ALPHA_TEST);
    }
}

/**
 * Pause the scene.
 * The scene is no longer hooked to the application.
 */
void TestTerrain::pause() {
}

/**
 * Main output procedure.
 * @return Self to continue or NULL to stop the application.
 */
Scene *TestTerrain::output() {

    // Set the camera.


    // Draw the scene.

    return _quit?0:this;
}

// Entry point
int main(void) {
    Camera camera;
    std::string title("Terrain Engine Test");
    TestTerrain testScene;
    Framework::WindowHint hint(800, 600, title);
    Application application;

    application.start(hint, &testScene);

    return 0;
}
