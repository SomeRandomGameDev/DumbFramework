#include "occluders.hpp"

namespace Framework {
namespace Render    {
namespace Light     {

static const size_t g_icospherePointCount = 12;
static const size_t g_icosphereTriangleCount = 20;

static const size_t g_cubePointCount = 8;
static const size_t g_cubeTriangleCount = 12;

static const size_t g_pyramidPointCount = 5;
static const size_t g_pyramidTriangleCount = 6;

static const size_t g_quadPointCount = 4;

static const float g_points[] = 
{
#if 0
 // icosphere
 0.000000f,  0.000000f, -1.500000f,
 1.085400f, -0.788580f, -0.670823f,
-0.414577f, -1.275960f, -0.670823f,
 1.085400f,  0.788580f, -0.670823f,
-1.341637f,  0.000000f, -0.670823f,
-0.414577f,  1.275960f, -0.670823f,
 1.341637f,  0.000000f,  0.670823f,
 0.414577f, -1.275960f,  0.670823f,
-1.085400f, -0.788580f,  0.670823f,
-1.085400f,  0.788580f,  0.670823f,
 0.414577f,  1.275960f,  0.670823f,
 0.000000f,  0.000000f,  1.500000f,
#else
 // cube
-1.000000f, -1.000000f,  1.000000f,
-1.000000f,  1.000000f,  1.000000f,
-1.000000f,  1.000000f, -1.000000f,
 1.000000f,  1.000000f,  1.000000f,
 1.000000f,  1.000000f, -1.000000f,
 1.000000f, -1.000000f,  1.000000f,
 1.000000f, -1.000000f, -1.000000f,
-1.000000f, -1.000000f, -1.000000f,
#endif // 0
 // pyramid
 0.000000f,  0.000000f, 0.000000f,
 0.500000f,  0.500000f, 1.000000f,
-0.500000f,  0.500000f, 1.000000f,
-0.500000f, -0.500000f, 1.000000f,
 0.500000f, -0.500000f, 1.000000f,
 // quad
-1.0f,-1.0f, 0.0f,
-1.0f, 1.0f, 0.0f,
 1.0f,-1.0f, 0.0f,
 1.0f, 1.0f, 0.0f
};

static const uint8_t g_triangles[] =
{
#if 0
 // icosphere
 0,  1,  2,
 1,  0,  3,
 0,  2,  4,
 0,  4,  5,
 0,  5,  3,
 1,  3,  6,
 2,  1,  7,
 4,  2,  8,
 5,  4,  9,
 3,  5, 10,
 1,  6,  7,
 2,  7,  8,
 4,  8,  9,
 5,  9, 10,
 3, 10,  6,
 7,  6, 11,
 8,  7, 11,
 9,  8, 11,
10,  9, 11,
 6, 10, 11,
#else
 // cube
 0, 1, 2,
 1, 3, 4,
 3, 5, 6,
 0, 7, 5,
 7, 2, 4,
 5, 3, 1,
 7, 0, 2,
 2, 1, 4,
 4, 3, 6,
 5, 7, 6,
 6, 7, 4,
 0, 5, 1,
#endif // 0
 // pyramid
12, 13, 14,
12, 14, 15,
12, 15, 16,
12, 16, 13,
13, 14, 16,
15, 16, 13
};

/**
 * Create occluder meshes.
 * @param [out] buffer Vertex buffer.
 * @param [out] index  Index buffers.
 * @param [out] attr   Vertex buffer attributes.
 * @return true if the occluder was succesfully created.
 */
bool createOccluders(VertexBuffer &buffer, IndexBuffer &index, std::array<Geometry::Attribute, LightType::COUNT> &attr)
{
    bool ret;

    attr[LightType::POINT_LIGHT      ] = Geometry::Attribute(Geometry::ComponentType::FLOAT, 3, false, sizeof(float[3]), 0, 0);
    attr[LightType::SPOT_LIGHT       ] = Geometry::Attribute(Geometry::ComponentType::FLOAT, 3, false, sizeof(float[3]), g_cubePointCount*sizeof(float[3]), 0);
    attr[LightType::DIRECTIONAL_LIGHT] = Geometry::Attribute(Geometry::ComponentType::FLOAT, 3, false, sizeof(float[3]), (g_cubePointCount+g_pyramidPointCount)*sizeof(float[3]), 0);

    size_t pointSize = (g_cubePointCount + g_pyramidPointCount + g_quadPointCount) * sizeof(float[3]);
    ret = buffer.create(pointSize, (void*)g_points, BufferObject::Access::Frequency::STATIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create vertex buffer.");
        return false;
    }
    
    size_t indexCount = g_cubeTriangleCount + g_pyramidTriangleCount;
    ret = index.create(indexCount * sizeof(uint8_t[3]), (void*)g_triangles, BufferObject::Access::Frequency::STATIC, BufferObject::Access::Type::DRAW);
    if(false == ret)
    {
        Log_Error(Module::Render, "Failed to create index buffer.");
        return false;
    }

    return true;
}

} // Light
} // Render
} // Framework
