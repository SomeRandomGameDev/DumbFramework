#ifndef _DUMB_FW_TRANSFORM_
#define _DUMB_FW_TRANSFORM_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>

namespace Framework {

/**
 * 3D transformation.
 */
class Transform
{
    public:
        /**
         * Default constructor.
         */
        Transform();
        /**
         * Constructor.
         * @param [in] orientation Orientation.
         * @param [in] position    Position.
         */
        Transform(glm::fquat const& orientation, glm::vec3 const& position);
        /**
         * Copy constructor.
         */
        Transform(Transform const& t);
        /**
         * Copy operator.
         */
        Transform& operator=(Transform const& t);
        /**
         * Retrieve up vector.
         */
        glm::vec3 up() const;
        /**
         * Retrieve right vector.
         */
        glm::vec3 right() const;
        /**
         * Retrieve forward vector.
         */
        glm::vec3 forward() const;
        /**
         * Retrieve euler angles.
         */
        glm::vec3 eulerAngles() const;
        /**
         * Return local transform matrix.
         */
        glm::mat4 local() const;
        /**
         * Interpolate between 2 transform.
         */
        friend Transform mix(Transform const& from, Transform const& to, float t);
        /**
         * Concatenate transforms.
         */
        friend Transform concat(Transform const& from, Transform const& to);
        /** Get orientation. **/
        glm::fquat orientation() const;
        /** Get position. **/
        glm::vec3  position() const;
        /** 
         * Set orientation.
         * @param [in] q Orientation.
         */
        void orientation(glm::fquat const& q);
        /** 
         * Set position. 
         * @param [in] p Position.
         */
        void position(glm::vec3 const& p);
        /**
         * Directly transform a point.
         * @param [in] p Input position.
         */
         glm::vec3 transform(glm::vec3 const& p) const;
         
    private:
        /** Transformations are represented internally as a dual quaternion. **/
        glm::fdualquat _q;
};

} // Framework

#endif /* _DUMB_FW_TRANSFORM_ */
