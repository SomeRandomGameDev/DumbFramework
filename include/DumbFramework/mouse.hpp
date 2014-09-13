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
        /**
         * Mouse event listener base class.
         */
        class Listener
        {
            public:
                /**
                 * Constructor.
                 */
                Listener();
                /**
                 * Destructor.
                 */
                virtual ~Listener();
                /**
                 * Button pressed callback.
                 * @param [in] button   Button id.
                 * @param [in] modifier Key modifier [todo]
                 */
                virtual void onMouseButtonPressed(Button button, int modifier);
                /**
                 * Button released callback.
                 * @param [in] button   Button id.
                 * @param [in] modifier Key modifier [todo]
                 */
                virtual void onMouseButtonReleased(Button button, int modifier);
                /**
                 * Cursor movement callback.
                 * @param [in] cursor Current cursor position.
                 */
                virtual void onMousePositionChanged(glm::dvec2 const& cursor);
                /**
                 * Mouse wheel, touchpad scrolling callback.
                 * @param [in] offset Scroll offset.
                 */
                virtual void onMouseScroll(glm::dvec2 const& offset);
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
         * Add mouse evnt listener.
         * @param [in] listener Mouse listener to add.
         */
        void addListener(Listener* listener);
        
    protected:
        /**
         * Call the OnMousePositionChanged method of each registered
         * listeners when the mouse pointer is moved.
         * The pointer coordinate is relative to the upper left corner
         * of the window.
         * @param [in] cursor Current cursor position.
         * @return false if an error occured.
         */
        void processPositionListeners(glm::dvec2 const& cursor);
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
        void processButtonListeners(Button button, bool pressed, int modifier);
        /**
         * Call the onMouseScroll method of each registered listeners
         * when the mouse wheel moves or the touchpad scrolling is used.
         * @param [in] offset Scroll offset.
         */
        void processScrollListeners(glm::dvec2 const& offset);
        
    private:
        /** Mouse parent window. **/
        WindowHint* _hint;
        /** Mouse listeners list. **/
        std::vector<Listener*> _listeners;
        // [todo] ...
};

} // Input
} // Framework

#include "mouse.inl"

#endif /* _DUMB_FW_MOUSE_ */

