#include <DumbFramework/render/light.hpp>

namespace Framework {
namespace Render    {

const unsigned int LightType::maxCount[LightType::COUNT] =
{
    MAX_POINT_LIGHTS,
    MAX_SPOT_LIGHTS,
    MAX_DIRECTIONAL_LIGHTS
};

const unsigned int LightType::elementCount[LightType::COUNT] = 
{
    8,
    8,
    12,
};

} // Render
} // Framework
