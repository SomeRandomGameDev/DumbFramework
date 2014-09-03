#include <DumbFramework/mouse.hpp>

namespace Framework {
namespace Input     {

/** Destructor. **/
Mouse::PositionListener::~PositionListener()
{}

/** Destructor. **/
Mouse::ButtonListener::~ButtonListener()
{}
/** Destructor. **/
Mouse::ScrollListener::~ScrollListener()
{}
/** 
 * Constructor.
 */
Mouse::Mouse()
    : _hint(NULL)
    , _enable(false)
    , _positionListeners()
    , _buttonListeners()
    , _scrollListeners()
{}
/** 
 * Destructor.
 */
Mouse::~Mouse()
{}
/**
 * Enable mouse.
 */
void Mouse::enable()
{}
/**
 * Disable mouse.
 */
void Mouse::disable()
{}
/**
 * Check if the mouse is enabled.
 */
bool Mouse::isEnabled() const
{
    return true;
}
/**
 * Set mouse cursor visibility.
 * @param [in] visible Cursor visibility.
 */
void Mouse::show(bool visible)
{}
/**
 * Check if the mouse cursor is visible.
 * @return true if the cursor is vible.
 */
bool Mouse::isVisible() const
{
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
{}
/**
 * Check if the mouse cursor is grabbed.
 * @return true if the mouse cursor is grabbed.
 */
bool Mouse::isGrabbed() const
{
    return true;
}
/**
 * Add mouse movement listener. It will be called when the mouse
 * pointer is moved. The pointer coordinate is relative to the 
 * upper left corner of the window.
 * @param [in] listener Position listener.
 * @return false if an error occured.
 */
bool Mouse::addPositionListener(Mouse::PositionListener* listener)
{
    return true;
}
/**
 * Add mouse button listener. It will be called when a mouse
 * button is pressed or released. Mouse button release events
 * will be generated for all pressed buttons when the window 
 * loses focus.
 * @param [in] listener Button listener.
 * @return false if an error occured.
 */
bool Mouse::addButtonListener(Mouse::ButtonListener* listener)
{
    return true;
}
/**
 * Add mouse wheel, touchpad scrolling listener.
 * @param [in] listener Scrolling listener.
 * @return false if an error occured.
 */
bool Mouse::addScrollListener(Mouse::ScrollListener* listener)
{
    return true;
}

} // Input
} // Framework
