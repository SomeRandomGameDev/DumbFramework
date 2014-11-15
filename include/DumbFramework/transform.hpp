#ifndef _DUMB_FW_TRANSFORM_
#define _DUMB_FW_TRANSFORM_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Framework {

/**
 * 3D transformation.
 */
class Transform
{
    public:
        /** Orientation **/
        glm::fquat orientation;
        /** Position **/
        glm::vec3  position;
        
    public:
        /**
         * Default constructor.
         */
        Transform();
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
};

} // Framework

#endif /* _DUMB_FW_TRANSFORM_ */
