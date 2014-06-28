#include "simplex.hpp"

namespace Framework {

/**
 * Add support points and compute the Minkowski difference.
 * @param [in] pA  Support point for the first shape.
 * @param [in] pB  Support point for the second shape.
 */
void Simplex2d::add(const glm::vec2& pA, const glm::vec2& pB)
{
    if(_count > 3) { return; }
    _points[_count].pA = pA;
    _points[_count].pB = pB;
    _points[_count].p  = pA - pB;
    ++_count;
}
/**
 * Remove all points from simplex;
 */
void Simplex2d::clear()
{
    _count = 0;
}
/**
 * Get point count.
 */
int Simplex2d::count() const
{ 
    return _count;
}
/**
 * Retrieve last point in simplex.
 * @warning unsafe method.
 */
const Simplex2d::Vertex& Simplex2d::last() const
{
    return _points[_count-1];
}
/**
 * Compute triple product cross( cross(a, b), c ) for 2d vectors.
 * It's an optimized version of the 3d version where z = 0.
 * @param [in] a First vector.
 * @param [in] b Second vector.
 * @param [in] c Third vector.
 */
static glm::vec2 triple(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
    float d = a.x*b.y - a.y*b.x;
    return glm::vec2(-d*c.y, d*c.x);
}
/** Constructor. **/
Simplex2d::Simplex2d()
    : _count(0)
{}
/** Destructor. **/
Simplex2d::~Simplex2d()
{}
/**
 * Check if the current 2d simplex contains the origin.
 * Both simplex and search direction are updated if the simplex does not 
 * contain the origin.
 * @param [in][out] direction  Search direction.
 * @return true if the simplex contains the origin.
 */
bool Simplex2d::update(glm::vec2& direction)
{
    if(2 == _count)
    {
        // Line segment.
        glm::vec2 a  = _points[1].p;
        glm::vec2 b  = _points[0].p;
        glm::vec2 ab = b - a;
        glm::vec2 ao = -a;
        // Compute normal.
        direction = triple(ab, ao, ab);
        // Check for degenerate case where the origin is on ab.
        if(glm::dot(direction, direction) <= std::numeric_limits<float>::epsilon())
        {
            direction = glm::vec2(ab.y, -ab.x);
        }
    }
    else if(3 == _count)
    {
        // Triangle.
        glm::vec2 a  = _points[2].p;
        glm::vec2 b  = _points[1].p;
        glm::vec2 c  = _points[0].p;
        glm::vec2 ab = b - a;
        glm::vec2 ac = c - a;
        glm::vec2 ao = -a;
        // Edge normals.
        glm::vec2 abp = triple(ac, ab, ab);
        glm::vec2 acp = triple(ab, ac, ac);
        // Check if the origin is beyond ac
        if(glm::dot(ao, acp) >= 0)
        {
            _count = 2;
            _points[1].p = _points[2].p;
            direction = acp;
        }
        // Check if the origin is beyond ab
        else if(glm::dot(ao, abp) >= 0)
        {
            _count = 2;
            _points[0].p = _points[1].p;
            _points[1].p = _points[2].p;
            direction = abp;
        }
        // The triangle contains the origin.
        else
        {
            return true;
        }
    }
    return false;
}

}
