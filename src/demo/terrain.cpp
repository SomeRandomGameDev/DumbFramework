// ## Should fix the GL/glew.h inclusion !
#include <GL/glew.h>
#include <application.hpp>
#include <scene.hpp>

#include <iostream>

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
};


/*
 * Definition.
 */

/**
 * Constructor.
 */
TestTerrain::TestTerrain() {
    _quit = false;
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

    return _quit?0:this;
}

// Entry point
int main(void) {
    std::string title("Terrain Engine Test");
    TestTerrain testScene;
    WindowHint hint(800, 600, title);
    Application application;

    application.start(hint, &testScene);

    return 0;
}
