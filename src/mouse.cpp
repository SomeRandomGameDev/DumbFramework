#include <DumbFramework/mouse.hpp>

namespace Framework {
namespace Input     {

/** 
 * Constructor.
 */
Mouse::Mouse()
    : _hint(nullptr)
    , _enabled(true)
{}
/** 
 * Destructor.
 */
Mouse::~Mouse()
{}
/**
 * Clear listeners.
 */
void Mouse::clear()
{
    onMousePositionChanged.clear();
    onMouseButtonPressed.clear();
    onMouseButtonReleased.clear();
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
     glfwSetInputMode(_hint->getWindow(), GLFW_CURSOR, grabbed ? GL_TRUE : GL_FALSE);
}
/**
 * Check if the mouse cursor is grabbed.
 * @return true if the mouse cursor is grabbed.
 */
bool Mouse::isGrabbed() const
{
    int value = glfwGetInputMode(_hint->getWindow(), GLFW_CURSOR);
    return (value == GL_TRUE);
}

} // Input
} // Framework
