#ifndef _DUMB_FW_WHINT_
#define _DUMB_FW_WHINT_

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

namespace Framework {

class WindowHint
{
    public:
        /**
         * Default constructor.
         * Fills hint with default values.
         */
        WindowHint();
        /**
         * Minimal constructor.
         * @param w Width.
         * @param h Height.
         * @param title Window title.
         */
        WindowHint(int w, int h, std::string const& title);

        /** 
         * Retrieve desired window size.
         * @return Window size in pixels.
         */
        inline glm::ivec2 getSize() const { return _size; }
        /**
         * Retrieve color channel depth.
         * @return Channel depth for each component in bit.
         */
        inline glm::ivec4 getColorDepth() const { return _colorDepth; }
        /**
         * Retrieve depth buffer size.
         * @return Depth buffer size in bit.
         */
        inline int getDepth() const { return _depth; }
        /**
         * Retrieve stencil buffer depth.
         * @return Stencil buffer depth in bit.
         */
        inline int getStencil() const { return _stencil; }
        /**
         * Retrieve window mode.
         * @return Window mode (as described in GLFW documentation).
         */
        inline int getMode() const { return _mode; }
        /**
         * Retrieve window context.
         * @return Window context.
         */
        inline GLFWwindow* getWindow() { return _window; }
        /**
         * Retrieve window title.
         * @return Window title.
         */
        inline std::string const& getTitle() const { return _title; }

        /**
         * Open a window with the current settings.
         * @return <code>true</code> if the operation succeeded.
         */
        bool openWindow();

    private:
        /** Window handle. **/
        GLFWwindow* _window;
        /** Desired window size in pixels. **/
        glm::ivec2  _size;
        /** Color components size in bit. **/
        glm::ivec4  _colorDepth;
        /** Depth buffer size in bit. **/
        int _depth;
        /** Stencil buffer size in bit. **/
        int _stencil;
        /** Window mode (see GLFW documentation). **/
        int _mode;
        /** Window title. **/
        std::string _title;
};

} // Framework

#endif /* _DUMB_FW_WHINT_ */
