#include <sys/types.h>
#include <glm/glm.hpp>
#include "ray.hpp"
#include "containment.hpp"

#if defined(_DUMB_FW_BOUNDING_BOX_) or defined(_DUMB_FW_BOUNDING_SPHERE_)

namespace Dumb      {
namespace Framework {

struct BoundingBox;
struct BoundingSphere;

}}

#else

#include "boundingbox.hpp"
#include "boundingsphere.hpp"

#endif /* defined(_DUMB_FW_BOUNDING_BOX_) or defined(_DUMB_FW_BOUNDING_SPHERE_) */
