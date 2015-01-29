#ifndef _DUMB_FW_RENDER_LIGHT_
#define _DUMB_FW_RENDER_LIGHT_

#include <glm/glm.hpp>
#include <DumbFramework/config.hpp>
#include <DumbFramework/render/uniformbuffer.hpp>

namespace Framework {
namespace Render    {

/**
 * Base light.
 */
class Light
{
    public:
        /** Constructor. **/
        Light();
        /** Destructor. **/
        virtual ~Light();
        
    public:
        /** Light position. **/
        glm::vec3 position;
        /** Light radius. **/
        float radius;
        /** Light color. **/
        glm::vec3 color;
        /** Light intensity. **/
        float intensity;
};

} // Render
} // Framework

#endif /* _DUMB_FW_RENDER_LIGHT_ */
