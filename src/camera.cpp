#include <glm/gtc/matrix_transform.hpp>
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
    : eye(cam.eye)
    , orientation(cam.orientation)
    , fov(cam.fov)
    , near(cam.near)
    , far(cam.far)
{}
/**
 * Copy operator.
 * @param [in] cam    Input camera.
 */
Camera& Camera::operator=(Camera const& cam)
{
    eye = cam.eye;
    orientation = cam.orientation;
    fov = cam.fov;
    near = cam.near;
    far  = cam.far;
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
    this->eye  = eye;
    glm::mat3 m;
    
    glm::vec3 f = glm::normalize(center - eye);
    glm::vec3 l = glm::normalize(glm::cross(up, f));
    glm::vec3 u = glm::cross(f, l);

    m[0] = l; m[1] = u; m[2] = f;

    this->orientation = glm::quat_cast(m);
}
/**
 * Set perspective information.
 * @param [in] fovy    Field of view angle in the y direction.
 * @param [in] near    Distance to the near clipping plane.
 * @param [in] far     Distance to the far clipping plane.
 */
void Camera::perspective(float fovy, float near, float far)
{
    this->fov  = fovy;
    this->near = near;
    this->far  = far;
}
/**
 * Compute view matrix.
 */
glm::mat4 Camera::viewMatrix() const
{
    glm::mat4 result = glm::mat4_cast(glm::inverse(orientation));

    result[3][0] =  (result[0][0]*eye.x + result[1][0]*eye.y + result[2][0]*eye.z);
    result[3][1] = -(result[0][1]*eye.x + result[1][1]*eye.y + result[2][1]*eye.z);
    result[3][2] =  (result[0][2]*eye.x + result[1][2]*eye.y + result[2][2]*eye.z);

    result[0][0] *= -1.0f;
    result[1][0] *= -1.0f;
    result[2][0] *= -1.0f;

    result[0][2] *= -1.0f;
    result[1][2] *= -1.0f;
    result[2][2] *= -1.0f;

    return result;
}
/**
 * Compute projection matrix for a given screen size.
 * @param [in] size    Screen size.
 */
glm::mat4 Camera::projectionMatrix(glm::ivec2 const& size) const
{
    float aspect = size.x / (float)size.y;
    return glm::perspective(fov, aspect, near, far);
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
    tmp  = viewMatrix() * tmp;
    tmp  = projectionMatrix(size) * tmp;
    tmp /= tmp.w;
    return glm::vec3(tmp);
}
/** Forward vector. **/
glm::vec3 Camera::forward() const { return glm::rotate(orientation, glm::vec3(0.0, 0.0, 1.0f)); }
/** Up vector. **/
glm::vec3 Camera::up() const { return glm::rotate(orientation, glm::vec3(0.0, 1.0, 0.0f)); }
/** Right vector. **/
glm::vec3 Camera::right() const { return glm::rotate(orientation, glm::vec3(-1.0, 0.0, 0.0f)); }

} // Framework
