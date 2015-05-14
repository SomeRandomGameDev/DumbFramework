#include <thread>
#include <chrono>
#include <functional>

#include <DumbFramework/render/tileculling.hpp>

namespace Framework {
namespace Render    {

TileCulling::TileCulling()
    : _tileCount(0)
    , _indices()
    , _futures()
{}

TileCulling::~TileCulling()
{}

void TileCulling::run(Camera const& cam, glm::ivec2 const& tileCount)//std::vector<PointLight> const& pointLight, std::vector<SpotLight> const& spotLight)
{
    _tileCount = tileCount;
    _indices.resize(_tileCount.x * _tileCount.y);

    size_t size  = _tileCount.x * _tileCount.y;
    size_t count = size / 4;

    // [todo] 2 pass, horizontal/ vertical / count per thread

    for(size_t i=0; i<4; i++)
    {
        _futures.push_back( std::async(std::bind(&TileCulling::callback, this, std::placeholders::_1), i) );
    }
}

void TileCulling::callback(size_t id)
{
    // [todo] 2 pass
    std::this_thread::sleep_for( std::chrono::microseconds(50) );
}

void TileCulling::wait()
{
    for(auto&& future : _futures)
    {
        future.wait();
    }
}

} // Render
} // Framework

