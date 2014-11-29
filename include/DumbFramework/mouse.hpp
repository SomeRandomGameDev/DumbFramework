#ifndef _DUMB_FW_MOUSE_
#define _DUMB_FW_MOUSE_

#include <vector>
#include <string>
#include <functional>

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
        /**
         * Mouse event delegate base class.
         * Return type is always void.
         * To add a new listener just do:
         * @code
         * Delegate<void(int, int)> dummy;
         * dummy += [](int a, int b) { std::cout << a << ' ' << b << std::endl; };
         * 
         * // Consider we have a class Foo with a method void bar(int, int)
         * Foo bleep;
         * dummy += std::bind(&Foo::bar, bleep, std::placeholders::_1, std::placeholders::_2);
         * 
         * // Call delegate.
         * dummy(0xcafe, 0xbeef);
         * 
         * @endcode
         */
        template <typename... Args>
        class Delegate
        {
            using Func = std::function<void(Args...)>;
            public:
                /**
                 * Add listener to delegate.
                 * @param [in] rhs Listener.
                 */
                Delegate& operator+= (Func const& rhs)
                {   _listeners.push_back(rhs);
                    return *this;
                }
                /**
                 * Call listeners.
                 */
                void operator() (Args&&... rhs)
                {   for(auto call : _listeners)
                    {
                        call(rhs...);
                    }
                }
                /**
                 * Remove all listeners.
                 */
                void clear()
                {   _listeners.clear();
                }
            private:
                /** Listeners. **/
                std::vector<Func> _listeners;
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
         * This delegate is called whenever the mouse pointer is moved.
         * The pointer coordinate is relative to the upper left corner
         * of the window.
         * @param [in] cursor Current cursor position.
         */
        Delegate<glm::dvec2 const&> onMousePositionChanged;
        /**
         * This delegate is called when the mouse button is pressed.
         * @param [in] button   Button id.
         * @param [in] modifier Key modifier [todo]
         */
        Delegate<Button, int> onMouseButtonPressed;
        /**
         * This delegate is called when the mouse button  is released. 
         * Mouse button release events will be generated for all pressed
         * buttons when the window loses focus.
         * @param [in] button   Button id.
         * @param [in] modifier Key modifier [todo]
         */
        Delegate<Button, int> onMouseButtonReleased;
        /**
         * This delegate is called when the mouse wheel moves or the
         * touchpad scrolling is used.
         * @param [in] offset Scroll offset.
         */
        Delegate<glm::dvec2 const&> onMouseScroll;
        
    private:
        /** Mouse parent window. **/
        WindowHint* _hint;
        /** Mouse manager state. **/
        bool _enabled;
        // [todo] ...
};

} // Input
} // Framework

#include "mouse.inl"

#endif /* _DUMB_FW_MOUSE_ */

