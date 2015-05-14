#ifndef _DUMB_FW_RENDER_TILE_CULLING_
#define _DUMB_FW_RENDER_TILE_CULLING_

#include <future>
#include <vector>

#include <DumbFramework/camera.hpp>
#include <DumbFramework/render/light.hpp>

namespace Framework {
namespace Render    {

class TileCulling
{
public:
    TileCulling();
    ~TileCulling();
    
    void run(Camera const& cam, glm::ivec2 const& tileCount);//(std::vector<PointLight> const& pointLight, std::vector<SpotLight> const& spotLight);
    
    void wait();

private:
    void callback(size_t id);

private:
    glm::ivec2 _tileCount;
    
    struct IndexArray
    {
        uint16_t size;
        uint16_t data[4095];
    };
    std::vector<IndexArray> _indices;
    
    std::vector<std::future<void>> _futures;
};

} // Render
} // Framework

#endif // _DUMB_FW_RENDER_TILE_CULLING_
