#ifndef _DUMB_FW_COLLISION_NARROW_PHASE_
#define _DUMB_FW_COLLISION_NARROW_PHASE_

#include "simplex.hpp"

namespace Framework {
    
    class NarrowPhase
    {
        public:
            template <typename FirstShape, typename SecondShape>
            static bool intersects(const FirstShape& a, const SecondShape& b, Simplex2d& simplex, glm::vec2& direction);
    };
}

namespace Framework
{

template <typename FirstShape2d, typename SecondShape2d>
bool NarrowPhase::intersects(const FirstShape2d& a, const SecondShape2d& b, Simplex2d& simplex, glm::vec2& direction)
{
    direction = glm::vec2(1.0f, 0.0f);
    simplex.clear();
    
    glm::vec2 dummy = a.support(direction) - b.support(-direction);
    simplex.add(dummy);
    if(glm::dot(dummy, direction) <= 0)
    {
        return false;
    }
    
    direction = -direction;
    for(int i=0; i<128; i++)
    {
        dummy = a.support(direction) - b.support(-direction);
        simplex.add(dummy);
        
        if(glm::dot(dummy, direction) <= 0)
        {
            return false;
        }
        else if(simplex.update(direction))
        {
            return true;
        }
    }
    return false;
}

}

#endif // _DUMB_FW_COLLISION_NARROW_PHASE_
