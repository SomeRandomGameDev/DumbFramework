#include <DumbFramework/transform.hpp>

namespace Framework {

        /** Orientation **/
        glm::fquat orientation;
        /** Position **/
        glm::vec3  position;
        
/**
 * Default constructor.
 */
Transform::Transform()
    : orientation()
    , position(0.0)
{}
/**
 * Copy constructor.
 */
Transform::Transform(Transform const& t)
    : orientation(t.orientation)
    , position(t.position)
{}
/**
 * Copy operator.
 */
Transform& Transform::operator=(Transform const& t)
{
    orientation = t.orientation;
    position    = t.position;
    return *this;
}
/**
 * Retrieve up vector.
 */
glm::vec3 Transform::up() const
{
    return glm::normalize(glm::rotate(orientation, glm::vec3( 0.0f, 1.0f, 0.0f)));
}
/**
 * Retrieve right vector.
 */
glm::vec3 Transform::right() const
{
    return glm::normalize(glm::rotate(orientation, glm::vec3(-1.0f, 0.0f, 0.0f)));
}
/**
 * Retrieve forward vector.
 */
glm::vec3 Transform::forward() const
{
    return glm::normalize(glm::rotate(orientation, glm::vec3( 0.0f, 0.0f, 1.0f)));
}
/**
 * Retrieve euler angles.
 */
glm::vec3 Transform::eulerAngles() const
{
    return glm::vec3(glm::pitch(orientation), glm::yaw(orientation), glm::roll(orientation));
}
/**
 * Return local transform matrix.
 */
glm::mat4 Transform::local() const
{
    // [todo] might be wrong!
    glm::mat4 local = glm::mat4_cast(orientation);
    local[3][0] = position.x;
    local[3][1] = position.y;
    local[3][2] = position.z;
    local[3][3] = 1.0f;
    return local;
}

// Build a dual quaternion from a transform
// [todo] remove this as soon as the shipped version of glm contains dual quaternions
struct DualQuat
{
    glm::fquat real;
    glm::fquat dual;
    
    DualQuat(Transform const& t)
    {
        real = t.orientation;
        dual.w = -0.5f * ( t.position.x*t.orientation.x + t.position.y*t.orientation.y + t.position.z*t.orientation.z),
        dual.x =  0.5f * ( t.position.x*t.orientation.w + t.position.y*t.orientation.z - t.position.z*t.orientation.y),
        dual.y =  0.5f * (-t.position.x*t.orientation.z + t.position.y*t.orientation.w + t.position.z*t.orientation.x),
        dual.z =  0.5f * ( t.position.x*t.orientation.y - t.position.y*t.orientation.x + t.position.z*t.orientation.w);
    }
    
    DualQuat(glm::fquat const& p, glm::fquat const& q)
        : real(p)
        , dual(q)
    {}
    
    glm::vec3 translation() const
    {
        glm::fquat q = (dual * 2.0f) * glm::conjugate(real);
        return glm::vec3(q.x, q.y, q.z);
    }
    
    Transform toTransform() const
    {
        Transform t;
        t.orientation = real;
        t.position = translation();
        return t;
    }
    
    friend DualQuat operator+(DualQuat const& p, DualQuat const& q)
    {
        return DualQuat(p.real+q.real, p.dual+q.dual);
    }
    
    friend DualQuat operator*(DualQuat const& p, float s)
    {
        return DualQuat(p.real*s, p.dual*s);
    }
    
    friend DualQuat operator*(float s, DualQuat const& p)
    {
        return DualQuat(p.real*s, p.dual*s);
    }
    
    friend DualQuat lerp(DualQuat const& from , DualQuat const& to, float t)
    {
        const float s = (glm::dot(from.real, to.real) < 0.0f) ? -t : t;
        return (from * (1.0f - t) + to * s);
    }
    
    friend DualQuat operator* (DualQuat const& a, DualQuat const& b)
    {
        return DualQuat(a.real*b.real, (a.real*b.dual) + (a.dual*b.real));
    }
};

/**
 * Interpolate between 2 transform.
 */
Transform mix(Transform const& from, Transform const& to, float t)
{
    return lerp(DualQuat(from), DualQuat(to), t).toTransform();
}
/**
 * Concatenate transforms.
 */
Transform concat(Transform const& a, Transform const& b)
{
    return (DualQuat(a) * DualQuat(b)).toTransform();
}

} // Framework
