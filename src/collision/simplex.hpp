#ifndef _DUMB_FW_COLLISION_SIMPLEX_
#define _DUMB_FW_COLLISION_SIMPLEX_

#include <glm/glm.hpp>

namespace Framework {

/**
 * 2d simplex. It can be either a point, a line segment or a triangle.
 */
class Simplex2d
{
public:
    /** 2d simplex vertex. **/
    struct Vertex
    {
        /** Support point for the first shape. **/
        glm::vec2 pA;
        /** Support point for the second shape. **/
        glm::vec2 pB;
        /** Minkowski difference point. **/
        glm::vec2 p;
    };
    
public:
    /** Constructor. **/
    Simplex2d();
    /** Destructor. **/
    ~Simplex2d();
    /**
     * Add support points and compute the Minkowski difference.
     * @param [in] pA  Support point for the first shape.
     * @param [in] pB  Support point for the second shape.
     */
    void add(const glm::vec2& pA, const glm::vec2& pB);
    /**
     * Remove all points from simplex;
     */
    void clear();
    /**
     * Retrieve last point in simplex.
     * @warning unsafe method.
     */
    const Vertex& last() const;
    /**
     * Get point count.
     */
    int count() const;
    /**
     * Check if the current 2d simplex contains the origin.
     * Both simplex and search direction are updated if the simplex does not 
     * contain the origin.
     * @param [in][out] direction  Search direction.
     * @return true if the simplex contains the origin.
     */
    bool update(glm::vec2& direction);

private:
    /** Number of points. **/
    int _count;
    /** Point stack. **/
    Vertex _points[3];
};

}

#endif // _DUMB_FW_COLLISION_SIMPLEX_
