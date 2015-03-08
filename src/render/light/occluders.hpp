#ifndef _DUMB_FW_LIGHT_OCCLUDERS_
#define _DUMB_FW_LIGHT_OCCLUDERS_

#include <array>
#include <functional>
#include <glm/glm.hpp>
#include <DumbFramework/render/vertexbuffer.hpp>
#include <DumbFramework/render/indexbuffer.hpp>
#include <DumbFramework/render/geometry.hpp>
#include <DumbFramework/render/light.hpp>

namespace Framework {
namespace Render    {
namespace Light     {
    
/**
 * Create occluder meshes.
 * @param [out] buffer Vertex buffer.
 * @param [out] index  Index buffers.
 * @param [out] attr   Vertex buffer attributes.
 * @return true if the occluder was succesfully created.
 */
bool createOccluders(VertexBuffer &buffer, IndexBuffer &index, std::array<Geometry::Attribute, LightType::COUNT> &attr);
/**
 * Occluder draw function.
 * The first parameter is the instance count.
 */
extern const std::function<void(int)> drawOccluders[LightType::COUNT];

} // Light
} // Render
} // Framework

#endif // _DUMB_FW_LIGHT_OCCLUDERS_
