#ifndef _DUMB_FW_RENDER_LIGHT_
#define _DUMB_FW_RENDER_LIGHT_

#include <glm/glm.hpp>

namespace Framework {
namespace Render    {

/**
 * Light type.
 */
struct LightType
{
    enum Value
    {
        POINT_LIGHT,
        SPOT_LIGHT,
        DIRECTIONAL_LIGHT,
        COUNT
    };
    Value value; /**< Light type value. **/
    /**
     * @brief Default constructor.
     * POINT_LIGHT is the default light type.
     */
    inline LightType() : value(POINT_LIGHT) {}
    /**
     * @brief Constructor.
     * Constructs light type from value.
     */
    inline LightType(Value v) : value(v) {}
    inline operator Value() { return value; }
};

/**
 * Point light.
 */
struct PointLight
{
    /** Light color. **/
    glm::vec3 color;
    /** Light position. **/
    glm::vec3 position;
    /** Light radius. **/
    float radius;
};

/**
 * Spot light.
 * The light comes from a single direction and position.
 */
struct SpotLight
{
    /** Light color. **/
    glm::vec3 color;
    /** Light position. **/
    glm::vec3 position;
    /** Light direction. **/
    glm::vec3 direction;
    /** Inner cone angle. **/
    float innerConeAngle;
    /** Outer cone angle. **/
    float outerConeAngle;
};

/**
 * Directional light.
 * The light comes from a single direction. 
 */
struct DirectionalLight
{
    /** Light color. **/
    glm::vec3 color;
    /** Light direction. **/
    glm::vec3 direction;
};

} // Render
} // Framework

#endif /* _DUMB_FW_RENDER_LIGHT_ */