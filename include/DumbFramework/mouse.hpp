#ifndef _DUMB_FW_MOUSE_
#define _DUMB_FW_MOUSE_

#include <DumbFramework/inputdelegate.hpp>

namespace Framework {
namespace Input     {

/**
 * Simple mouse class.
 */
class Mouse
{
    public:
        /** 
         * @brief Mouse button id.
         */
        struct Button
        {
            /**  Mouse button values. **/
            enum Value
            {
                NONE   =-1,
                LEFT   = 0,
                RIGHT,
                MIDDLE,
                EXTRA_0,
                EXTRA_1,
                EXTRA_2,
                EXTRA_3,
                EXTRA_4
            };
            Value value; /**< Mouse button value. **/
            /**
             * @brief Default constructor.
             * By default the mouse button id is set to Button::None.
             */
            inline Button() : value(NONE) {}
            /**
             * @brief Constructor.
             */
            inline Button(Value v) : value(v) {}
            inline operator Value() { return value; }
        };
       
    public:
        /** 
         * Constructor.
         */
        Mouse();
        /** 
         * Destructor.
         */
        ~Mouse();
        /**
         * Attach to window.
         * @param [in] window Parent window.
         */
        void attach(GLFWwindow *window);
        /**
         * Clear listeners.
         */
        void clear();
        /**
         * Enable mouse.
         */
        void enable();
        /**
         * Disable mouse.
         */
        void disable();
        /**
         * Check if the mouse is enabled.
         */
        bool isEnabled() const;
        /**
         * Set mouse cursor visibility.
         * @param [in] visible Cursor visibility.
         */
        void show(bool visible);
        /**
         * Check if the mouse cursor is visible.
         * @return true if the cursor is vible.
         */
        bool isVisible() const;
        /**
         * Specify if the mouse cursor is grabbed.
         * @param [in] grabbed If true the mouse cursor is hidden and
         *                     constrained to the window. However the 
         *                     mouse position can go outside window
         *                     bounds.
         */
        void grab(bool grabbed);
        /**
         * Check if the mouse cursor is grabbed.
         * @return true if the mouse cursor is grabbed.
         */
        bool isGrabbed() const;
        /**
         * Get mouse position.
         */
        glm::dvec2 position() const;
        /**
         * Set mouse position.
         * @param [in] p Mouse position.
         */
        void position(glm::dvec2 const& p);
        /**
         * Get button state.
         * @param [in] id Button id.
         * @return true if the button is pressed, false otherwise.
         */
         bool buttonState(Button id);
        /**
         * This delegate is called whenever the mouse pointer is moved.
         * The pointer coordinate is relative to the upper left corner
         * of the window.
         * @param [in] cursor Current cursor position.
         */
        Delegate<glm::dvec2 const&> onMousePositionChanged;
        /**
         * This delegate is called when the mouse button is pressed or 
         * released.
         * Mouse button release events will be generated for all pressed
         * buttons when the window loses focus.
         * @param [in] button   Button id.
         * @param [in] pressed  true if the button is pressed or
         *                      false if released.
         * @param [in] modifier Key modifier [todo]
         */
        Delegate<Button, bool, int> onMouseButton;
        /**
         * This delegate is called when the mouse wheel moves or the
         * touchpad scrolling is used.
         * @param [in] offset Scroll offset.
         */
        Delegate<glm::dvec2 const&> onMouseScroll;
        
    private:
        /** Mouse parent window. **/
        GLFWwindow *_window;
        /** Mouse manager state. **/
        bool _enabled;
        // [todo] ...
};

} // Input
} // Framework

#include "mouse.inl"

#endif /* _DUMB_FW_MOUSE_ */

