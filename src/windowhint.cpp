#include <DumbFramework/windowhint.hpp>
#include <GLFW/glfw3.h>

namespace Framework {

static const glm::ivec2 g_defaultWindowSize   = glm::ivec2(640, 480);
static const char const* g_defaultWindowTitle = "DumbFramework";

/**
 * Default constructor.
 * Fills hint with default values.
 */
WindowHint::WindowHint()
    : _window(NULL)
    , _size(g_defaultWindowSize)
    , _colorDepth(0)
    , _depth(0)
    , _stencil(0)
    , _mode(0)
    , _title(g_defaultWindowTitle)
{}
/**
 * Minimal constructor.
 * @param w Width.
 * @param h Height.
 * @param title Window title.
 */
WindowHint::WindowHint(int w, int h, std::string const& title)
    : _window(NULL)
    , _size(w, h)
    , _colorDepth(0)
    , _depth(0)
    , _stencil(0)
    , _mode(0)
    , _title(title)
{}

/**
 * Open a window with the current settings.
 * @return <code>true</code> if the operation succeeded.
 */
bool WindowHint::openWindow()
{
    _window = glfwCreateWindow(_size.x, _size.y, _title.c_str(), NULL, NULL); // [todo] monitor + share
    glfwMakeContextCurrent(_window);
    if(NULL == _window)
    {
        return false;
    }
    return true;
}

} // Framework
