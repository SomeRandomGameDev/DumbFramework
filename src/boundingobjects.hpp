#include <sys/types.h>
#include <glm/glm.hpp>
#include "ray.hpp"
#include "containment.hpp"
#include "plane.hpp"

#if defined(_DUMB_FW_BOUNDING_BOX_) or defined(_DUMB_FW_BOUNDING_SPHERE_)

namespace Dumb      {
namespace Framework {

class BoundingBox;
struct BoundingSphere;
class BoundingFrustum;

}}

#else

#include "boundingbox.hpp"
#include "boundingsphere.hpp"
#include "boundingfrustum.hpp"

#endif /* defined(_DUMB_FW_BOUNDING_BOX_) or defined(_DUMB_FW_BOUNDING_SPHERE_) */
