#ifndef _DUMB_FW_CAMERA_
#define _DUMB_FW_CAMERA_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>

namespace Framework {

/**
 * Camera.
 */
class Camera
{
    public:
        /**
         * Default constructor.
         */
        Camera();
        /**
         * Copy constructor.
         * @param [in] cam    Input camera.
         */
        Camera(Camera const& cam);
        /**
         * Copy operator.
         * @param [in] cam    Input camera.
         */
        Camera& operator=(Camera const& cam);
        /**
         * Set camera position and make it look at a given position.
         * @param [in] eye       Camera position.
         * @param [in] center    Position where the camera is looking at.
         * @param [in] up        Normalized up vector (default (0,1,0)).
         */
        void lookAt(glm::vec3 const& eye, glm::vec3 const& center, glm::vec3 const& up = glm::vec3(0.0f, 1.0f, 0.0f));
        /**
         * Set perspective information.
         * @param [in] fovy    Field of view angle in the y direction.
         * @param [in] near    Distance to the near clipping plane.
         * @param [in] far     Distance to the far clipping plane.
         */
        void perspective(float fovy, float near, float far);
        /**
         * Compute view matrix.
         */
        glm::mat4 viewMatrix() const;
        /**
         * Compute projection matrix for a given screen size.
         * @param [in] size    Screen size.
         */
        glm::mat4 projectionMatrix(glm::ivec2 const& size) const;
        /**
         * Compute screen space position for a given screen size.
         * @param [in] world   World position.
         * @param [in] size    Screen size.
         * @return Screen position between [-1, 1].
         */
        glm::vec3 screenPosition(glm::vec3 const& world, glm::ivec2 const& size) const;
        
        // [todo] move/rotate ...
        
        /** Position. **/
        glm::vec3 const& eye() const;
        /** Forward vector. **/
        glm::vec3 const& forward() const;
        /** Up vector. **/
        glm::vec3 const& up() const;
        /** Right vector. **/
        glm::vec3 const& right() const;
        /** Field of view. **/
        float fov() const;
        /** Near plane distance. **/
        float near() const;
        /** Far plane distance. **/
        float far() const;
        
    protected:
        /** Position. **/
        glm::vec3 m_eye;
        /** Forward vector. **/
        glm::vec3 m_forward;
        /** Up vector. **/
        glm::vec3 m_up;
        /** Right vector. **/
        glm::vec3 m_right;
        /** Field of view. **/
        float m_fov;
        /** Near plane distance. **/
        float m_near;
        /** Far plane distance. **/
        float m_far;
        /** Cached view matrix. **/
        glm::mat4 m_view;
        /** Cached perspective matrix. **/
        glm::mat4 m_perspective;
};

} // Framework

#endif /* _DUMB_FW_CAMERA_ */

