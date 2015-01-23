#ifndef _DUMB_FW_DUMMY_MESH_
#define _DUMB_FW_DUMMY_MESH_

#include <DumbFramework/render/mesh.hpp>

namespace Framework {
namespace Render    {

/**
 * Create an unit cube mesh.
 * @return false if an error occured.
 */
bool createCube(Mesh& mesh);

} // Render
} // Framework

#endif /* _DUMB_FW_DUMMY_MESH_ */
