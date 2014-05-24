#include <sys/types.h>
#include <glm/glm.hpp>
#include "ray.hpp"
#include "containment.hpp"
#include "plane.hpp"

#if defined(_DUMB_FW_BOUNDING_BOX_)     \
 or defined(_DUMB_FW_BOUNDING_SPHERE_)  \
 or defined(_DUMB_FW_BOUNDING_FRUSTUM_)

namespace Framework {

/* 2D */
class BoundingCircle;

/* 3D */
class BoundingBox;
struct BoundingSphere;
class BoundingFrustum;

}

#else

/* 2D */
#include "boundingcircle.hpp"

/* 3D */
#include "boundingbox.hpp"
#include "boundingsphere.hpp"
#include "boundingfrustum.hpp"

#endif /* defined(_DUMB_FW_BOUNDING_BOX_) or defined(_DUMB_FW_BOUNDING_SPHERE_) or defined(_DUMB_FW_BOUNDING_FRUSTUM_) */
