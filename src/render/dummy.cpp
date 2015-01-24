#include <DumbFramework/log.hpp>
#include <DumbFramework/render/dummy.hpp>

namespace Framework {
namespace Render    {

static const int g_cubeVertexCount   = 26;
static const int g_cubeTriangleCount = 12;

static const GLfloat g_cubeVertices[] =
{
//   Point                  UV               Normal
     1.0f,  1.0f,  1.0f,    0.75f, 0.75f,    0.0f,  1.0f,  0.0f, 0.f,0.f,0.f,0.f,
     1.0f,  1.0f, -1.0f,    0.50f, 0.75f,    0.0f,  1.0f,  0.0f, 0.f,0.f,0.f,0.f,
    -1.0f,  1.0f, -1.0f,    0.50f, 1.00f,    0.0f,  1.0f,  0.0f, 0.f,0.f,0.f,0.f,
    -1.0f,  1.0f,  1.0f,    0.75f, 1.00f,    0.0f,  1.0f,  0.0f, 0.f,0.f,0.f,0.f,
    -1.0f, -1.0f, -1.0f,    0.25f, 0.00f,   -1.0f,  0.0f, -0.0f, 0.f,0.f,0.f,0.f,
    -1.0f, -1.0f,  1.0f,    0.25f, 0.25f,   -1.0f,  0.0f, -0.0f, 0.f,0.f,0.f,0.f,
    -1.0f,  1.0f,  1.0f,    0.50f, 0.25f,   -1.0f,  0.0f, -0.0f, 0.f,0.f,0.f,0.f,
    -1.0f,  1.0f, -1.0f,    0.50f, 0.00f,   -1.0f,  0.0f, -0.0f, 0.f,0.f,0.f,0.f,
     1.0f, -1.0f, -1.0f,    0.25f, 0.75f,   -0.0f, -1.0f, -0.0f, 0.f,0.f,0.f,0.f,
     1.0f, -1.0f,  1.0f,    0.00f, 0.75f,   -0.0f, -1.0f, -0.0f, 0.f,0.f,0.f,0.f,
    -1.0f, -1.0f, -1.0f,    0.25f, 1.00f,   -0.0f, -1.0f, -0.0f, 0.f,0.f,0.f,0.f,
    -1.0f, -1.0f,  1.0f,    0.00f, 1.00f,   -0.0f, -1.0f,  0.0f, 0.f,0.f,0.f,0.f,
     1.0f,  1.0f, -1.0f,    0.50f, 0.75f,    1.0f,  0.0f, -0.0f, 0.f,0.f,0.f,0.f,
     1.0f,  1.0f,  1.0f,    0.50f, 0.50f,    1.0f,  0.0f, -0.0f, 0.f,0.f,0.f,0.f,
     1.0f, -1.0f, -1.0f,    0.25f, 0.75f,    1.0f,  0.0f, -0.0f, 0.f,0.f,0.f,0.f,
     1.0f,  1.0f,  1.0f,    0.50f, 0.50f,    1.0f, -0.0f,  0.0f, 0.f,0.f,0.f,0.f,
     1.0f, -1.0f,  1.0f,    0.25f, 0.50f,    1.0f, -0.0f,  0.0f, 0.f,0.f,0.f,0.f,
     1.0f, -1.0f, -1.0f,    0.25f, 0.75f,    1.0f, -0.0f,  0.0f, 0.f,0.f,0.f,0.f,
     1.0f,  1.0f,  1.0f,    0.50f, 0.50f,   -0.0f, -0.0f,  1.0f, 0.f,0.f,0.f,0.f,
    -1.0f,  1.0f,  1.0f,    0.50f, 0.25f,   -0.0f, -0.0f,  1.0f, 0.f,0.f,0.f,0.f,
     1.0f, -1.0f,  1.0f,    0.25f, 0.50f,   -0.0f, -0.0f,  1.0f, 0.f,0.f,0.f,0.f,
    -1.0f, -1.0f,  1.0f,    0.25f, 0.25f,    0.0f, -0.0f,  1.0f, 0.f,0.f,0.f,0.f,
     1.0f,  1.0f, -1.0f,    0.50f, 0.75f,    0.0f,  0.0f, -1.0f, 0.f,0.f,0.f,0.f,
     1.0f, -1.0f, -1.0f,    0.25f, 0.75f,    0.0f,  0.0f, -1.0f, 0.f,0.f,0.f,0.f,
    -1.0f, -1.0f, -1.0f,    0.25f, 1.00f,    0.0f,  0.0f, -1.0f, 0.f,0.f,0.f,0.f,
    -1.0f,  1.0f, -1.0f,    0.50f, 1.00f,    0.0f, -0.0f, -1.0f, 0.f,0.f,0.f,0.f
};

static const GLuint g_cubeFaces[] =
{
     0,  1,  2,
     0,  2,  3,
     4,  5,  6,
     4,  6,  7,
     8,  9, 10,
     9, 11, 10,
    12, 13, 14,
    15, 16, 17,
    18, 19, 20,
    19, 21, 20,
    22, 23, 24,
    22, 24, 25
};

/**
 * Create an unit cube mesh.
 * @return false if an error occured.
 */
bool createCube(Mesh& mesh)
{
    bool ret;
    uint32_t attrMask;
    attrMask = Render::Mesh::HasPosition | Render::Mesh::HasTexCoord | Render::Mesh::HasNormal | Render::Mesh::HasTangent;
    ret = mesh.create(g_cubeVertexCount, g_cubeTriangleCount, attrMask, (void*)g_cubeVertices, (void*)g_cubeFaces);
    return ret;
}

} // Render
} // Framework
