#ifndef _DUMB_FW_RENDER_LIGHT_
#define _DUMB_FW_RENDER_LIGHT_

#include <glm/glm.hpp>

namespace Framework {
namespace Render    {

#define MAX_POINT_LIGHTS 128
#define MAX_SPOT_LIGHTS 128
#define MAX_DIRECTIONAL_LIGHTS 8

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
    
    static const unsigned int maxCount[LightType::COUNT];
    static const unsigned int elementCount[LightType::COUNT];
    
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
    /** Light radius. **/
    float radius;
    /** Light position. **/
    glm::vec3 position;
    // [todo] add padding float?
};

/**
 * Spot light.
 * The light comes from a single direction and position.
 */
struct SpotLight
{
    /** Light color. **/
    glm::vec3 color;
    /** Light radius. **/
    float radius;
    /** Light position. **/
    glm::vec3 position;
    /** Inner cone angle. **/
    float innerConeAngle;
    /** Light direction. **/
    glm::vec3 direction;
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
