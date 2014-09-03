#ifndef _DUMB_FW_MOUSE_
#define _DUMB_FW_MOUSE_

#include <vector>
#include <string>

#include <DumbFramework/config.hpp>
#include <DumbFramework/windowhint.hpp>

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
                EXTRA
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
         * Add mouse movement listener. It will be called when the mouse
         * pointer is moved. The pointer coordinate is relative to the 
         * upper left corner of the window.
         * @param [in] instance Object instance.
         * @param [in] method   A pointer to the method that will be
         *                      called when the mouse pointer position
         *                      changed.
         * @return false if an error occured.
         */
        template<class T>
        bool addPositionListener(T* instance, bool (T::*method)(glm::dvec2 const& cursor));
        /**
         * Add mouse button listener. It will be called when a mouse
         * button is pressed or released. Mouse button release events
         * will be generated for all pressed buttons when the window 
         * loses focus.
         * @param [in] instance Object instance.
         * @param [in] method   A pointer to the method that will be
         *                      called when a mouse button state changed.
         * @return false if an error occured.
         */
        template<class T>
        bool addButtonListener(T* instance, bool (T::*method)(Button button, bool pressed, int modifier));
        /**
         * Add mouse wheel, touchpad scrolling listener.
         * @param [in] instance Object instance.
         * @param [in] method   A pointer to the method that will be
         *                      called when the scrolling device is used.
         * @return false if an error occured.
         */
        template<class T>
        bool addScrollListener(T* instance, bool (T::*method)(glm::dvec2 const& offset));
    
    private:
        /** Mouse position listener. **/
        class PositionListener;
        /** Mouse button listener. **/
        class ButtonListener;
        /** Scroll listener. **/
        class ScrollListener;
        /**
         * Add mouse movement listener. It will be called when the mouse
         * pointer is moved. The pointer coordinate is relative to the 
         * upper left corner of the window.
         * @param [in] listener Position listener.
         * @return false if an error occured.
         */
        bool addPositionListener(PositionListener* listener);
        /**
         * [todo]
         * @param [in] cursor Current cursor position.
         * @return [todo]
         */
        bool processPositionListeners(glm::dvec2 const& cursor);
        /**
         * Add mouse button listener. It will be called when a mouse
         * button is pressed or released. Mouse button release events
         * will be generated for all pressed buttons when the window 
         * loses focus.
         * @param [in] listener Button listener.
         * @return false if an error occured.
         */
        bool addButtonListener(ButtonListener* listener);
        /**
         * [todo]
         * @param [in] button   Button id.
         * @param [in] pressed  Button state (true: pressed, false: released).
         * @param [in] modifier Key modifier [todo]
         * @return [todo]
         */
        bool processButtonListeners(Button button, bool pressed, int modifier);
        /**
         * Add mouse wheel, touchpad scrolling listener.
         * @param [in] listener Scrolling listener.
         * @return false if an error occured.
         */
        bool addScrollListener(ScrollListener* listener);
        /**
         * [todo]
         * @param [in] offset [todo]
         * @return [todo]
         */
        bool processScrollListners(glm::dvec2 const& offset);
        
        
    private:
        /** Mouse parent window. **/
        WindowHint* _hint;
        /** [todo] **/
        bool _enable;
        /** Position listener list. **/
        std::vector<PositionListener*> _positionListeners;
        /** Button listener list. **/
        std::vector<ButtonListener*>   _buttonListeners;
        /** Scroll listener list. **/
        std::vector<ScrollListener*>   _scrollListeners;
};

} // Input
} // Framework

#include "mouse.inl"

#endif /* _DUMB_FW_MOUSE_ */

