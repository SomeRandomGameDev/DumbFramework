#ifndef _DUMB_FW_RENDER_POINT_LIGHT_
#define _DUMB_FW_RENDER_POINT_LIGHT_

#include <DumbFramework/render/light/light.hpp>

namespace Framework {
namespace Render    {

/**
 * Point light.
 */
class PointLight : public Light
{
    public:
        /** Constructor. **/
        PointLight();
        /** Destructor. **/
        virtual ~PointLight();
        
    public:
        /** Light position. **/
        glm::vec3 _position;
};

} // Render
} // Framework

#endif /* _DUMB_FW_RENDER_POINT_LIGHT_ */
