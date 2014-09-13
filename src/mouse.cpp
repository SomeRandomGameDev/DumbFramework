#include <DumbFramework/mouse.hpp>

namespace Framework {
namespace Input     {

/**
 * Constructor.
 */
Mouse::Listener::Listener()
{}
/**
 * Destructor.
 */
Mouse::Listener::~Listener()
{}
/**
 * Button pressed callback.
 * @param [in] button   Button id.
 * @param [in] modifier Key modifier [todo]
 */
void Mouse::Listener::onMouseButtonPressed(Mouse::Button button, int modifier)
{
    (void)button;
    (void)modifier;
}
/**
 * Button released callback.
 * @param [in] button   Button id.
 * @param [in] modifier Key modifier [todo]
 */
void Mouse::Listener::onMouseButtonReleased(Mouse::Button button, int modifier)
{
    (void)button;
    (void)modifier;
}
/**
 * Cursor movement callback.
 * @param [in] cursor Current cursor position.
 */
void Mouse::Listener::onMousePositionChanged(glm::dvec2 const& cursor)
{
    (void)cursor;
}
/**
 * Mouse wheel, touchpad scrolling callback.
 * @param [in] offset Scroll offset.
 */
void Mouse::Listener::onMouseScroll(glm::dvec2 const& offset)
{
    (void)offset;
}

/** 
 * Constructor.
 */
Mouse::Mouse()
    : _hint(NULL)
    , _listeners()
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
    _listeners.clear();
}
/**
 * Enable mouse.
 */
void Mouse::enable()
{
    // [todo]
}
/**
 * Disable mouse.
 */
void Mouse::disable()
{
    // [todo]
}
/**
 * Check if the mouse is enabled.
 */
bool Mouse::isEnabled() const
{
    // [todo]
    return true;
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
    (void)grabbed;
    // [todo]
}
/**
 * Check if the mouse cursor is grabbed.
 * @return true if the mouse cursor is grabbed.
 */
bool Mouse::isGrabbed() const
{
    // [todo]
    return false;
}
/**
 * Add mouse evnt listener.
 * @param [in] listener Mouse listener to add.
 */
void Mouse::addListener(Listener* listener)
{
    if(NULL == listener)
    {
        return;
    }
    _listeners.push_back(listener);
    // [todo] lock?
}
/**
 * Call the OnMousePositionChanged method of each registered
 * listeners when the mouse pointer is moved.
 * The pointer coordinate is relative to the upper left corner
 * of the window.
 * @param [in] cursor Current cursor position.
 * @return false if an error occured.
 */
void Mouse::processPositionListeners(glm::dvec2 const& cursor)
{
    for(size_t i=0; i<_listeners.size(); i++)
    {
        _listeners[i]->onMousePositionChanged(cursor);
    }
}
/**
 * Call the OnMouseButtonPressed or OnMouseButtonReleased 
 * methods of each registered listeners wether the mouse button 
 * is pressed or released. Mouse button release events
 * will be generated for all pressed buttons when the window 
 * loses focus.
 * @param [in] button   Button id.
 * @param [in] pressed  Button state.
 * @param [in] modifier Key modifier [todo]
 */
void Mouse::processButtonListeners(Mouse::Button button, bool pressed, int modifier)
{
    void (Mouse::Listener::*callback)(Mouse::Button, int);
    callback = pressed ? &Mouse::Listener::onMouseButtonReleased : &Mouse::Listener::onMouseButtonReleased;
    
    for(size_t i=0; i<_listeners.size(); i++)
    {
        (_listeners[i]->*callback)(button, modifier);
    }
}
/**
 * Call the onMouseScroll method of each registered listeners
 * when the mouse wheel moves or the touchpad scrolling is used.
 * @param [in] offset Scroll offset.
 */
void Mouse::processScrollListeners(glm::dvec2 const& offset)
{
    for(size_t i=0; i<_listeners.size(); i++)
    {
        _listeners[i]->onMouseScroll(offset);
    }
}

} // Input
} // Framework
