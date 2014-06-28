#ifndef _DUMB_FW_COLLISION_NARROW_PHASE_
#define _DUMB_FW_COLLISION_NARROW_PHASE_

#include "simplex.hpp"

namespace Framework {
    /** 
     * Narrow phase collision detection helper. 
     * It is used to compute shape pair collision.
     */
    class NarrowPhase
    {
        public:
            /** Maximum GJK iteration count. **/
            static unsigned int MAX_ITERATION;
            /**
             * Check if two shape intersects using GJK (Gilbert–Johnson–Keerthi) algorithm.
             * @param [in] a  First shape.
             * @param [in] b  Second shape.
             * @param [out] simplex  Voronoi simplex. Used by EPA (Expanding Polytope Algorithm) to determine penetration vector and depth.
             * @param [in,out] direction  Search direction.
             * @return true if the two shapes collide.
             */
            template <typename FirstShape, typename SecondShape>
            static bool intersects(const FirstShape& a, const SecondShape& b, Simplex2d& simplex, glm::vec2& direction);
    };
}

namespace Framework
{

/**
 * Check if two shape intersects using GJK (Gilbert–Johnson–Keerthi) algorithm.
 * @param [in] a  First shape.
 * @param [in] b  Second shape.
 * @param [out] simplex  Voronoi simplex. Used by EPA (Expanding Polytope Algorithm) to determine penetration vector and depth.
 * @param [in,out] direction  Search direction.
 * @return true if the two shapes collide.
 */
template <typename FirstShape2d, typename SecondShape2d>
bool NarrowPhase::intersects(const FirstShape2d& a, const SecondShape2d& b, Simplex2d& simplex, glm::vec2& direction)
{
    direction = b.getCenter() - a.getCenter();
    if(glm::dot(direction, direction) <= std::numeric_limits<float>::epsilon())
    {
        direction = glm::vec2(1.0f, 0.0f);
    }    

    // Clean simplex.
    simplex.clear();
    
    // Compute Minkowski difference point and add it to the simplex.
    simplex.add(a.support(direction), b.support(-direction));
    // Check if the new point is past the origin along the search direction vector.
    if(glm::dot(simplex.last().p, direction) <= 0)
    {
        return false;
    }
    // Negate search direction in order to make it point towards the origin.
    direction = -direction;
    for(int i=0; i<NarrowPhase::MAX_ITERATION; i++)
    {
        // Compute Minkowski difference point and add it to the simplex.
        simplex.add(a.support(direction), b.support(-direction));
    
        // Check if the new point is past the origin along the search direction vector.
        if(glm::dot(simplex.last().p, direction) <= 0)
        {
            return false;
        }
        // Check if the simplex contains the origin.
        // Update simplex and search direction if that's not the case.
        else if(simplex.update(direction))
        {
            return true;
        }
    }
    return false;
}

}

#endif // _DUMB_FW_COLLISION_NARROW_PHASE_
