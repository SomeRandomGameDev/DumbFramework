#ifndef _DUMB_FW_SCENE_
#define _DUMB_FW_SCENE_

#include <GLFW/glfw3.h>

/**
 * Scene interface.
 */
class Scene {
    public:

        /**
         * Handle key event.
         * @param key Key identifier.
         * @param scancode Scancode.
         * @param action GLFW_PRESS or GLFW_RELEASE.
         * @param mods Key modifiers.
         */
        virtual void handleKeyAction(int /* key */, int /* scancode */, int /* action */, int /* mods */) {}

        /**
         * Handle mouse button action.
         * @param button Button identifier : GLFW_MOUSE_BUTTON_n with n from 1 to
         * the maximum number of available button or LEFT/RIGHT/MIDDLE.
         * @param action GLFW_PRESS or GLFW_RELEASE.
         */
        virtual void handleMouseButtonAction(int /* button */, int /* action */) {}

        /**
         * Handle mouse cursor movement.
         * @param x Coordinate of the mouse cursor on the X Axis in the window.
         * @param y Coordinate of the mouse cursor on the Y Axis in the window.
         */
        virtual void handleMousePositionAction(double /* x */, double /* y */) {}

        /**
         * Handle mouse wheel movement.
         * @param x Absolute position of the wheel on x axis.
         * @param y Absolute position of the wheel on y axis.
         */
        virtual void handleMouseWheelAction(double /* x */, double /* y */) {}

        /**
         * Handle window size change.
         * @param width New window width.
         * @param height New window height.
         */
        virtual void handleWindowSize(int /* width */, int /* height */) {}

        /**
         * Output the scene.
         * @return The new scene to display. If null, the application ends.
         */
        virtual Scene* output() { return 0; }

        /**
         * Pause the scene. Occurs if the scene must be
         * changed or initialized.
         */
        virtual void pause() {}

        /**
         * Start/Resume the scene.
         */
        virtual void resume(GLFWwindow *) {}

        /**
         * Destructor.
         */
        virtual ~Scene() {}

    private:

};

#endif
