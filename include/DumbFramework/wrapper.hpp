#ifndef _DUMB_FW_WRAPPER_
#define _DUMB_FW_WRAPPER_

#include <DumbFramework/config.hpp>
#include <DumbFramework/log.hpp>
#include <DumbFramework/mouse.hpp>

/**
 * GLFW error callback
 * @param [in] errorCode      Error code.
 * @param [in] description    UTF-8 encoded string describing the error.
 */
static void Log_GLFWErrorCallback(int errorCode, const char* description)
{
     Log_Error(Framework::Module::Base, "GLFW failed with error code %d: %s", errorCode, description);
}


namespace Framework {
  namespace Core {
    
     /**
      * Video mode information. Un-mutable set of information about an avaiable video mode.
      */
     class VideoMode {
         public:
             /**
              * Empty Constructor.
              */
             VideoMode() {}

             /**
              * Constructor.
              * @param [in] width Width (in pixels).
              * @param [in] height Height (in pixels).
              * @param [in] rate Refresh rate (in Hz).
              * @param [in] rbits Red Channel Depth (in bits).
              * @param [in] gbits Green Channel Depth (in bits).
              * @param [in] bbits Blue Channel Depth (in bits).
              */
             VideoMode(int width, int height, int rate, int rbits, int gbits, int bbits) :
                 _width(width), _height(height), _rate(rate),
                 _rbits(rbits), _gbits(gbits), _bbits(bbits) {}

             /**
              * Retrieve video mode information.
              * @param [out] width Width (in pixels).
              * @param [out] height Height (in pixels).
              * @param [out] rate Refresh rate (in Hz).
              * @param [out] rbits Red Channel Depth (in bits).
              * @param [out] gbits Green Channel Depth (in bits).
              * @param [out] bbits Blue Channel Depth (in bits).
              */
             inline void get(int *width, int *height, int *rate, int *rbits, int *gbits, int *bbits) {
                 if(0 != width) {
                     *width = _width;
                 }
                 if(0 != height) {
                     *height = _height;
                 }
                 if(0 != rate) {
                     *rate = _rate;
                 }
                 if(0 != rbits) {
                     *rbits = _rbits;
                 }
                 if(0 != gbits) {
                     *gbits = _gbits;
                 }
                 if(0 != bbits) {
                     *bbits = _bbits;
                 }
             }

         private:
             /**
              * Width (in pixels).
              */
             int _width;
             /**
              * Height (in pixels).
              */
             int _height;
             /**
              * Refresh rate (in Hz).
              */
             int _rate;
             /**
              * Red color depth (in bits).
              */
             int _rbits;
             /**
              * Green color depth (in bits).
              */
             int _gbits;
             /**
              * Blue color depth (in bits).
              */
             int _bbits;
     };

     /**
      * Monitor Information. Unmutable set of information about a specific monitor.
      */
     class Monitor {
         public:

             /**
              * Constructor.
              */
             Monitor(int width, int height, int x, int y,
                     std::string name, unsigned int size,
                     VideoMode *modes, unsigned int current) :
                         _width(width), _height(height),
                         _xpos(x), _ypos(y),
                         _name(name), _currentMode(current) {
                 _modeCount = size;
                 _modes = new VideoMode[size];  // FIXME No default constructor.
                 for(unsigned int i = 0; i < size; ++i) {
                    _modes[i] = modes[i];
                 }
             }

             /**
              * Empty Constructor.
              */
             Monitor() : 
                _width(0), _height(0),
                _xpos(0), _ypos(0),
                _name(), _modeCount(0),
                _modes(nullptr), _currentMode(-1) {
             }

             /**
              * Affectation operator.
              */
             const Monitor &operator=(Monitor &orig) {
                 _width = orig._width;
                 _height = orig._height;
                 _name = orig._name;
                 _xpos = orig._xpos;
                 _ypos = orig._ypos;
                 _currentMode = orig._currentMode;
                 _modeCount = orig._modeCount;
                 _currentMode = orig._currentMode;
                 if(0 != _modes) {
                     delete [] _modes;
                 }
                 _modes = new VideoMode[_modeCount];
                 for(unsigned int i = 0; i < _modeCount; ++i) {
                     _modes[i] = orig._modes[i];
                 }
                 return *this;
             }

             /**
              * Destructor.
              */
             ~Monitor() {
                 delete []_modes;
             }

             /**
              * Retrieve physical size.
              * @param [out] width Width in mm.
              * @param [out] height Height in mm.
              */
             inline void getPhysicalSize(int *width, int *height) {
                 *width = _width;
                 *height = _height;
             }

             /**
              * Retrieve monitor name.
              * @param [out] name String instance in which to copy the name.
              */
             inline void getName(std::string *name) {
                 *name = _name;
             }

             /**
              * Retrieve the monitor position.
              * @param [out] x Position on the X-axis in pixels.
              * @param [out] y Position on the Y-axis in pixels.
              */
             inline void getPosition(int *x, int *y) {
                 *x = _xpos;
                 *y = _ypos;
             }

             /**
              * Retrieve current video mode.
              * @param [out] mode Current video mode instance in which to copy the data.
              */
             inline void getCurrentVideoMode(VideoMode *mode) {
                 *mode = _modes[_currentMode];
             }

             /**
              * Retrieve the number of available video modes.
              * @return Number video modes.
              */
             inline unsigned int getVideoModeCount() {
                 return _modeCount;
             }

             /**
              * Retrieve video modes.
              * @param [in] size Size of the fetch buffer.
              * @param [out] buffer Buffer in which to copy video modes.
              */
             inline void getModes(unsigned int size, VideoMode *buffer) {
                 if(size >= _modeCount) {
                   for(unsigned int i = 0; i < _modeCount; ++i) {
                     buffer[i] = _modes[i];
                   }
                 }
             }

         private:
             /**
              * Physical width (in millimeters).
              */
             int _width;

             /**
              * Physical height (in millimeters).
              */
             int _height;

             /**
              * Position on the X-Axis in the workspace (in pixels).
              */
             int _xpos;

             /**
              * Position on the Y-Axis in the workspace (in pixels).
              */
             int _ypos;

             /**
              * Name.
              */
             std::string _name;

             /**
              * Number of available video modes.
              */
             unsigned int _modeCount;

             /**
              * List of available video modes.
              */
             VideoMode *_modes;

             /**
              * Current video mode.
              */
             unsigned int _currentMode;
     };


     // TODO VIDEO INFORMATION CLASS (Unmutable List of available Monitors)

     // TODO WINDOW CREATION ADVISOR CLASS (Returned by the 'developper' to instruct which kind of window to create)

     // TODO -- The Window Creation Advisor will be part of the Application Creation Advisor class.

     // TODO APPLICATION CONTEXT CLASS (Provide access to Output capacities and current context informations)
  }
}


/* The delegated concept shall implement:
   GLFWwindow *createWindow();
   void destroyWindow(GLFWwindow *);
   void init();
   void render();
   void handleKeyAction(int key, int scancode, int action, int mods);
   void handleMouseButtonAction(int button, int action, int mods);
   void handleMousePositionAction(double x, double y);
   void handleMouseWheelAction(double x, double y);
   void handleWindowSize(int x, int y);
   void handleChar(unsigned int unicodeChar);
   void handleCursorEnter(int flag);
 */

/**
 * Wrapper for simple GLFW application.
 * @param <T> Delegate class.
 */
template <typename T> class Wrapper {
    public:
        /**
         * Delegate instance.
         */
        T* _delegate;
        /**
         * Mouse manager.
         */
        Framework::Input::Mouse mouse;
    public:

        /**
         * Constructor.
         * @param dlg Delegate class.
         */
        Wrapper(T *dlg) : _delegate(dlg) {}

        /**
         * Start the wrapper.
         */
        bool start();

    private:

        /**
         * Wrapper to key event callback.
         */
        static void handleKey(GLFWwindow*, int, int, int, int);

        /**
         * Wrapper to mouse button event callback.
         */
        static void handleMouseButton(GLFWwindow*, int, int, int);

        /**
         * Wrapper to mouse position event callback.
         */
        static void handleMousePosition(GLFWwindow*, double, double);

        /**
         * Wrapper to mouse wheel event callback.
         */
        static void handleMouseWheel(GLFWwindow*, double, double);

        /**
         * Wrapper to window closing event callback.
         */
        static void handleWindowClose(GLFWwindow *);

        /**
         * Wrapper to window resizing event callback.
         */
        static void handleWindowSize(GLFWwindow *, int, int);

        /**
         * Wrapper to Unicode character input.
         */
        static void handleChar(GLFWwindow *, unsigned int);

        /**
         * Wrapper to cursor enter/leave window.
         */
        static void handleCursorEnter(GLFWwindow *, int);

};

// Implementation **********************************************

template <typename T> bool Wrapper<T>::start()
{
    // Initialize glfw and set error callback.
    glfwSetErrorCallback(Log_GLFWErrorCallback);
    bool result = glfwInit();
    if(!result)
    {
        return false;
    }
    // Query delegate to create window.
    GLFWwindow *window = _delegate->createWindow();
    if(0 == window)
    {
        Log_Error(Framework::Module::Base, "Failed to create window.");
        glfwTerminate();
        return false;
    }

    mouse.attach(window);

    glfwMakeContextCurrent(window);
    // Set window user data.
    glfwSetWindowUserPointer(window, this);

    // Initialize OpenGL extensions.
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if(GLEW_OK != glewError)
    {
        Log_Error(Framework::Module::Base, "Failed to initialize OpenGL extensions: %s.", glewGetErrorString(glewError));
        glfwTerminate();
        return false;
    }
    
    // Set window callbacks.
    glfwSetKeyCallback(window, Wrapper<T>::handleKey);
    glfwSetMouseButtonCallback(window, Wrapper<T>::handleMouseButton);
    glfwSetCursorPosCallback(window, Wrapper<T>::handleMousePosition);
    glfwSetScrollCallback(window, Wrapper<T>::handleMouseWheel);
    glfwSetWindowSizeCallback(window, Wrapper<T>::handleWindowSize);
    glfwSetWindowCloseCallback(window, Wrapper<T>::handleWindowClose);
    glfwSetCharCallback(window, Wrapper<T>::handleChar);
    glfwSetCursorEnterCallback(window, Wrapper<T>::handleCursorEnter);

    // Delegate init.
    _delegate->init();

    // This is the application loop.
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        _delegate->render();
        glfwSwapBuffers(window);
    }
    
    // Destroy window.
    _delegate->destroyWindow(window);
    glfwTerminate();
    
    return true;
}

template <typename T> void Wrapper<T>::handleKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleKeyAction(key, scancode, action, mods);
}

template <typename T> void Wrapper<T>::handleMouseButton(GLFWwindow* window, int button, int action, int mods) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleMouseButtonAction(button, action, mods);
    if(app->mouse.isEnabled())
    {
        Framework::Input::Mouse::Button bvalue;
        switch(button)
        {
            case  GLFW_MOUSE_BUTTON_LEFT:
                bvalue = Framework::Input::Mouse::Button::LEFT;
                break;
            case  GLFW_MOUSE_BUTTON_RIGHT:
                bvalue = Framework::Input::Mouse::Button::RIGHT;
                break;
            case  GLFW_MOUSE_BUTTON_MIDDLE:
                bvalue = Framework::Input::Mouse::Button::MIDDLE;
                break;
            case GLFW_MOUSE_BUTTON_4:
                bvalue = Framework::Input::Mouse::Button::EXTRA_0;
                break;
            case GLFW_MOUSE_BUTTON_5:
                bvalue = Framework::Input::Mouse::Button::EXTRA_1;
                break;
            case GLFW_MOUSE_BUTTON_6:
                bvalue = Framework::Input::Mouse::Button::EXTRA_2;
                break;
            case GLFW_MOUSE_BUTTON_7:
                bvalue = Framework::Input::Mouse::Button::EXTRA_3;
                break;
            default:
                bvalue = Framework::Input::Mouse::Button::EXTRA_4;
                break;
        }
        app->mouse.onMouseButton(bvalue, (GLFW_PRESS == action), mods);
    }
}

template <typename T> void Wrapper<T>::handleMousePosition(GLFWwindow* window, double x, double y) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleMousePositionAction(x, y);
    if(app->mouse.isEnabled())
    {
        app->mouse.onMousePositionChanged(glm::dvec2(x, y));
    }
}

template <typename T> void Wrapper<T>::handleMouseWheel(GLFWwindow* window, double x, double y) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleMouseWheelAction(x, y);
    if(app->mouse.isEnabled())
    {
        app->mouse.onMouseScroll(glm::dvec2(x, y));
    }
}

template <typename T> void Wrapper<T>::handleWindowSize(GLFWwindow* window, int width, int height) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleWindowSize(width, height);
}

template <typename T> void Wrapper<T>::handleWindowClose(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GL_TRUE);
}

template <typename T> void Wrapper<T>::handleChar(GLFWwindow *window, unsigned int chr) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleChar(chr);
}

template <typename T> void Wrapper<T>::handleCursorEnter(GLFWwindow *window, int flag) {
    Wrapper<T> *app = static_cast<Wrapper<T> *>(glfwGetWindowUserPointer(window));
    app->_delegate->handleCursorEnter(flag);
}


#endif
