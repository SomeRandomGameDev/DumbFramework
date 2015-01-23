#ifndef _DUMB_FW_RENDER_SPOT_LIGHT_
#define _DUMB_FW_RENDER_SPOT_LIGHT_

#include <DumbFramework/render/light/light.hpp>

namespace Framework {
namespace Render    {

/**
 * Spot light.
 * The light comes from a single direction and position.
 */
class SpotLight : public Light
{
    public:
        /** Constructor. **/
        SpotLight();
        /** Destructor. **/
        virtual ~SpotLight();
        
    public:
        /** Light position. **/
        glm::vec3 position;
        /** Light direction. **/
        glm::vec3 direction;
        /** Inner cone angle. **/
        float innerConeAngle;
        /** Outer cone angle. **/
        float outerConeAngle;
};

} // Render
} // Framework

#endif /* _DUMB_FW_RENDER_SPOT_LIGHT_ */
