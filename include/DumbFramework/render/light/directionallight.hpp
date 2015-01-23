#ifndef _DUMB_FW_RENDER_DIRECTIONAL_LIGHT_
#define _DUMB_FW_RENDER_DIRECTIONAL_LIGHT_

#include <DumbFramework/render/light/light.hpp>

namespace Framework {
namespace Render    {

/**
 * Directional light.
 * The light comes from a single direction. 
 */
class DirectionalLight : public Light
{
    public:
        /** Constructor. **/
        DirectionalLight();
        /** Destructor. **/
        virtual ~DirectionalLight();
        
    public:
        /** Light direction. **/
        glm::vec3 direction;
};

} // Render
} // Framework

#endif /* _DUMB_FW_RENDER_DIRECTIONAL_LIGHT_ */
