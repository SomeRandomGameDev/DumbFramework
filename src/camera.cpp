#include <DumbFramework/camera.hpp>

namespace Framework {

/**
 * Default constructor.
 */
Camera::Camera()
{}
/**
 * Copy constructor.
 * @param [in] cam    Input camera.
 */
Camera::Camera(Camera const& cam)
    : m_eye(cam.m_eye)
    , m_forward(cam.m_forward)
    , m_up(cam.m_up)
    , m_right(cam.m_right)
    , m_fov(cam.m_fov)
    , m_near(cam.m_near)
    , m_far(cam.m_far)
    , m_view(cam.m_view)
    , m_perspective(cam.m_perspective)
{}
/**
 * Copy operator.
 * @param [in] cam    Input camera.
 */
Camera& Camera::operator=(Camera const& cam)
{
    m_eye = cam.m_eye;
    m_forward = cam.m_forward;
    m_up      = cam.m_up;
    m_right   = cam.m_right;
    m_fov = cam.m_fov;
    m_near = cam.m_near;
    m_far  = cam.m_far;
    m_view = cam.m_view;
    m_perspective = cam.m_perspective;
    return *this;
}
/**
 * Set camera position and make it look at a given position.
 * @param [in] eye       Camera position.
 * @param [in] center    Position where the camera is looking at.
 * @param [in] up        Normalized up vector (default (0,1,0)).
 */
void Camera::lookAt(glm::vec3 const& eye, glm::vec3 const& center, glm::vec3 const& up)
{
    m_eye  = eye;
    m_forward = glm::normalize(center - eye);
    m_right   = glm::normalize(glm::cross(m_forward, up));
    m_up      = glm::cross(m_right, m_forward);
    
    m_view[0][0] = -m_right.x;
    m_view[1][0] = -m_right.y;
    m_view[2][0] = -m_right.z;
    
    m_view[0][1] =  m_up.x;
    m_view[1][1] =  m_up.y;
    m_view[2][1] =  m_up.z;
    
    m_view[0][2] = -m_forward.x;
    m_view[1][2] = -m_forward.y;
    m_view[2][2] = -m_forward.z;
    
    m_view[3][0] = -glm::dot(m_right,   m_eye);
    m_view[3][1] = -glm::dot(m_up,      m_eye);
    m_view[3][2] =  glm::dot(m_forward, m_eye);
}
/**
 * Set perspective information.
 * @param [in] fovy    Field of view angle in the y direction.
 * @param [in] near    Distance to the near clipping plane.
 * @param [in] far     Distance to the far clipping plane.
 */
void Camera::perspective(float fovy, float near, float far)
{
    m_fov  = fovy;
    m_near = near;
    m_far  = far;
    
    m_perspective = glm::mat4(0.0f);
    m_perspective[0][0] = m_perspective[1][1] = 1.0f / tan(m_fov / 2.0f);
    m_perspective[2][2] = -(m_far + m_near) / (m_far - m_near);
    m_perspective[2][3] = -1.0f;
    m_perspective[3][2] = -(2.0f * m_far * m_near) / (m_far - m_near);
}
/**
 * Compute view matrix.
 */
glm::mat4 Camera::viewMatrix() const
{ return m_view; }
/**
 * Compute projection matrix for a given screen size.
 * @param [in] size    Screen size.
 */
glm::mat4 Camera::projectionMatrix(glm::ivec2 const& size) const
{
    float invAspect = static_cast<float>(size.y) / static_cast<float>(size.x);
    glm::mat4 result(m_perspective);
    result[0][0] *= invAspect;
    return result;
}
/**
 * Compute screen space position for a given screen size.
 * @param [in] world   World position.
 * @param [in] size    Screen size.
 * @return Screen position between [-1, 1].
 */
glm::vec3 Camera::screenPosition(glm::vec3 const& world, glm::ivec2 const& size) const
{
    glm::vec4 tmp = glm::vec4(world, 1.0f);
    tmp  = m_view * tmp;
    tmp  = projectionMatrix(size) * tmp;
    tmp /= tmp.w;
    return glm::vec3(tmp);
}
/** Position. **/
glm::vec3 const& Camera::eye() const { return m_eye; }
/** Forward vector. **/
glm::vec3 const& Camera::forward() const { return m_forward; }
/** Up vector. **/
glm::vec3 const& Camera::up() const { return m_up; }
/** Right vector. **/
glm::vec3 const& Camera::right() const { return m_right; }
/** Field of view. **/
float Camera::fov() const { return m_fov; }
/** Near plane distance. **/
float Camera::near() const { return m_near; }
/** Far plane distance. **/
float Camera::far() const { return m_far; }

} // Framework
