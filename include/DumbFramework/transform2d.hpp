#ifndef _DUMB_FW_TRANSFORM_2D_
#define _DUMB_FW_TRANSFORM_2D_

#include <glm/glm.hpp>

namespace Framework {

/**
 * 2D transformation.
 */
class Transform2D
{
    public:

        /**
         * Retrieve up vector.
         */
        inline glm::vec2 up() const;
        /**
         * Retrieve right vector.
         */
        inline glm::vec2 right() const;
        /**
         * Return local transform matrix.
         */
        inline glm::mat3 local() const;
        /**
         * Compute transform matrix.
         */
        inline void update();
        
    public:
        /** Position. **/
        glm::vec2 position;
        /** Rotation angle. **/
        float angle;
        /** Scale. **/
        glm::vec2 scale;
        
    private:
        /** Transformation matrix. **/
        glm::mat3 _local;
};

} // Framework

#include "transform2d.inl"

#endif /* _DUMB_FW_TRANSFORM_2D_ */
