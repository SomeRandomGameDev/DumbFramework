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
    /** Constructor. **/
    Simplex2d();
    /** Destructor. **/
    ~Simplex2d();
    /**
     * Add a point to simplex.
     */
    void add(const glm::vec2& pt);
    /**
     * Remove all points from simplex;
     */
    void clear();
    /**
     * Retrieve last point in simplex.
     * @warning unsafe method.
     */
    const glm::vec2& last() const;
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
    glm::vec2 _points[3];
};

}

/*
template <typename FirstShape, typename SecondShape, typename VectorType>
VectorType support(const FirstShape& shape0, const SecondShape& shape1, const VectorType& direction)
{
    return shape0.support(direction) - shape1.support(-direction);
}*/

#endif // _DUMB_FW_COLLISION_SIMPLEX_
