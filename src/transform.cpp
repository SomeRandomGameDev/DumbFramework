#include <DumbFramework/transform.hpp>

namespace Framework {

/**
 * Default constructor.
 */
Transform::Transform()
    : _q()
{}
/**
 * Constructor.
 * @param [in] orientation Orientation.
 * @param [in] position    Position.
 */
Transform::Transform(glm::fquat const& orientation, glm::vec3 const& position)
    : _q(orientation, position)
{}
/**
 * Copy constructor.
 */
Transform::Transform(Transform const& t)
    : _q(t._q)
{}
/**
 * Copy operator.
 */
Transform& Transform::operator=(Transform const& t)
{
    _q = t._q;
    return *this;
}
/**
 * Retrieve up vector.
 */
glm::vec3 Transform::up() const
{
    return glm::normalize(glm::rotate(_q.real, glm::vec3( 0.0f, 1.0f, 0.0f)));
}
/**
 * Retrieve right vector.
 */
glm::vec3 Transform::right() const
{
    return glm::normalize(glm::rotate(_q.real, glm::vec3(-1.0f, 0.0f, 0.0f)));
}
/**
 * Retrieve forward vector.
 */
glm::vec3 Transform::forward() const
{
    return glm::normalize(glm::rotate(_q.real, glm::vec3( 0.0f, 0.0f, 1.0f)));
}
/**
 * Retrieve euler angles.
 */
glm::vec3 Transform::eulerAngles() const
{
    return glm::vec3(glm::pitch(_q.real), glm::yaw(_q.real), glm::roll(_q.real));
}
/**
 * Return local transform matrix.
 */
glm::mat4 Transform::local() const
{
    return glm::mat4(glm::mat3x4_cast(_q));
}
/**
 * Interpolate between 2 transform.
 */
Transform mix(Transform const& from, Transform const& to, float t)
{
    Transform tr;
    tr._q = glm::lerp(from._q, to._q, t);
    return tr;
}
/**
 * Concatenate transforms.
 */
Transform concat(Transform const& a, Transform const& b)
{
    Transform t;
    t._q = a._q * b._q;
    return t;
}
/** Orientation **/
glm::fquat Transform::orientation() const
{
    return _q.real;
}
/** Position **/
glm::vec3  Transform::position() const
{
    glm::fquat q = (_q.dual * 2.0f) * glm::conjugate(_q.real);
    return glm::vec3(q.x, q.y, q.z);
}
/** 
 * Set orientation.
 * @param [in] q Orientation.
 */
void Transform::orientation(glm::fquat const& q)
{
    glm::vec3 p = position();
    _q = glm::fdualquat(q, p);
}
/** 
 * Set position. 
 * @param [in] p Position.
 */
void Transform::position(glm::vec3 const& p)
{
    _q = glm::dualquat(_q.real, p);
}
/**
 * Directly transform a point.
 * @param [in] p Input position.
 */
glm::vec3 Transform::transform(glm::vec3 const& p) const
{
    return _q * p;
}
 
} // Framework
