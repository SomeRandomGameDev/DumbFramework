#include <DumbFramework/joystick.hpp>

namespace Framework {

/** 
 * Constructor. 
 * @param [in] id Joystick identifier. 
 */
Joystick::Joystick(int id)
    :_id(GLFW_JOYSTICK_1 + id)
{
    char const* str = glfwGetJoystickName(_id);
    _name = (NULL != str) ? str : "";
}

/** 
 * Destructor.
 */
Joystick::~Joystick()
{}
/**
 * Is the joystick present?
 * @return true if the joystick is present.
 */
bool Joystick::isPresent() const
{
    return (glfwJoystickPresent(_id) == GL_TRUE);
}
/**
 * Update joystick states.
 */
void Joystick::update()
{
    const float *axes = NULL;
    int axesCount = 0;
    
    const unsigned char *keys = NULL;
    int keysCount = 0;

    _axes.clear();
    _keys.clear();
    
    if(isPresent())
    {
        axes = glfwGetJoystickAxes(_id, &axesCount);
        keys = glfwGetJoystickButtons(_id, &keysCount);
    }
    if(axesCount)
    {
        _axes.insert(_axes.begin(), axes, axes+axesCount);
    }
    if(keysCount)
    {
        _keys.insert(_keys.begin(), keys, keys+keysCount);
    }
}
/**
 * Get joystick name.
 * @return Joystick name in UTF-8.
 */
std::string const& Joystick::name() const
{
    return _name;
}
/**
 * Get axes count.
 * @return axis count.
 */
size_t Joystick::getAxesCount() const
{
    return _axes.size();
}
/**
 * Get axes.
 * @return const pointer to axes.
 */
float const* Joystick::getAxes() const
{
    return &_axes[0];
}
/**
 * Get key count.
 * @return key count.
 */
size_t Joystick::getKeyCount() const
{
    return _keys.size();
}
/**
 * Get keys.
 * @return const pointer ot keys.
 */
uint8_t const* Joystick::getKeys() const
{
    return &_keys[0];
}
    
} // Framework
