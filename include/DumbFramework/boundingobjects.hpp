#include <sys/types.h>
#include <glm/glm.hpp>
#include <DumbFramework/line2d.hpp>
#include <DumbFramework/ray.hpp>
#include <DumbFramework/containment.hpp>
#include <DumbFramework/plane.hpp>

#if defined(_DUMB_FW_BOUNDING_BOX_)     \
 or defined(_DUMB_FW_BOUNDING_SPHERE_)  \
 or defined(_DUMB_FW_FRUSTUM_) \
 or defined(_DUMB_FW_BOUNDING_CIRCLE_)  \
 or defined(_DUMB_FW_BOUNDING_QUAD_)
 
namespace Framework {

/* 2D */
class BoundingCircle;
class BoundingQuad;

/* 3D */
class BoundingBox;
class BoundingSphere;
class Frustum;

}

#else

/* 2D */
#include <DumbFramework/boundingcircle.hpp>
#include <DumbFramework/boundingquad.hpp>

/* 3D */
#include <DumbFramework/boundingbox.hpp>
#include <DumbFramework/boundingsphere.hpp>
#include <DumbFramework/frustum.hpp>

#endif /*    defined(_DUMB_FW_BOUNDING_BOX_) 
        * or defined(_DUMB_FW_BOUNDING_SPHERE_)
        * or defined(_DUMB_FW_FRUSTUM_)
        * or defined(_DUMB_FW_BOUNDING_CIRCLE_)
        * or defined(_DUMB_FW_BOUNDING_QUAD_)
        */
