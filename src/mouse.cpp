#include <DumbFramework/mouse.hpp>

namespace Framework {
namespace Input     {

/** 
 * Constructor.
 */
Mouse::Mouse()
    : _window(nullptr)
    , _enabled(true)
{}

/** 
 * Destructor.
 */
Mouse::~Mouse()
{}
/**
 * Attach to window.
 * @param [in] window Parent window.
 */
void Mouse::attach(GLFWwindow *window)
{
    _window = window;
}
/**
 * Clear listeners.
 */
void Mouse::clear()
{
    onMousePositionChanged.clear();
    onMouseButton.clear();
    onMouseScroll.clear();
}
/**
 * Enable mouse.
 */
void Mouse::enable()
{
    _enabled = true;
}
/**
 * Disable mouse.
 */
void Mouse::disable()
{
    _enabled = false;
}
/**
 * Check if the mouse is enabled.
 */
bool Mouse::isEnabled() const
{
    return _enabled;
}
/**
 * Set mouse cursor visibility.
 * @param [in] visible Cursor visibility.
 */
void Mouse::show(bool visible)
{
    (void)visible;
    // [todo]
}
/**
 * Check if the mouse cursor is visible.
 * @return true if the cursor is vible.
 */
bool Mouse::isVisible() const
{
    // [todo]
    return true;
}
/**
 * Specify if the mouse cursor is grabbed.
 * @param [in] grabbed If true the mouse cursor is hidden and
 *                     constrained to the window. However the 
 *                     mouse position can go outside window
 *                     bounds.
 */
void Mouse::grab(bool grabbed)
{
     glfwSetInputMode(_window, GLFW_CURSOR, grabbed ? GL_TRUE : GL_FALSE);
}
/**
 * Check if the mouse cursor is grabbed.
 * @return true if the mouse cursor is grabbed.
 */
bool Mouse::isGrabbed() const
{
    int value = glfwGetInputMode(_window, GLFW_CURSOR);
    return (value == GL_TRUE);
}
/**
 * Get mouse position.
 */
glm::dvec2 Mouse::position() const
{
    glm::dvec2 p;
    glfwGetCursorPos(_window, &p.x, &p.y);
    return p;
}
/**
 * Set mouse position.
 * @param [in] p Mouse position.
 */
void Mouse::position(glm::dvec2 const& p)
{
    glfwSetCursorPos(_window, p.x, p.y);
}
/**
 * Get button state.
 * @param [in] id Button id.
 * @return true if the button is pressed, false otherwise.
 */
 bool Mouse::buttonState(Button id)
 {
     if(Button::NONE == id)
     {
         return false;
     }
     int state = glfwGetMouseButton(_window, id);
     return (GLFW_PRESS == state);
 }

} // Input
} // Framework
