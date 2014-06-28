#include <cstring>
#include "polygon2d.hpp"

namespace Framework {

/** Constructor. */
Polygon2d::Polygon2d()
    : _point(NULL)
    , _count(0)
    , _center(0.0f)
{}
/** Constructor.
 *  @param [in] buffer Pointer to the point array.
 *  @param [in] count  Number of points 
 *  @param [in] stride Offset between two consecutive points. (default=0)
 */
Polygon2d::Polygon2d(const float* buffer, size_t count, size_t stride)
{
    _count = count;
    _point = new glm::vec2[_count];
    for(size_t i=0; i<_count; i++, buffer+=stride)
    {
        _point[i].x = *buffer++;
        _point[i].y = *buffer++;
    }
    computeCenter();
}
/** Copy constructor.
 *  @param [in] box Source polygon.
 */
Polygon2d::Polygon2d(const Polygon2d& poly)
{
    _count = poly._count;
    if(_count)
    {
        _point = new glm::vec2[_count];
        memcpy(_point, poly._point, _count*sizeof(glm::vec2));
        _center = poly._center;
    }
    else
    {
        _point  = NULL;
        _center = glm::vec2(0.0f);
    }
}
/**
 * Destructor.
 */
Polygon2d::~Polygon2d()
{
    if(_point)
    {
        delete [] _point;
        _point = NULL;
    }
    _count = 0;
}
/** Adjust buffer size if needed. **/
void Polygon2d::adjust(size_t count)
{
    if(_count != count)
    {
        _count = count;
        if(_point)
        {
            delete [] _point;
            _point = NULL;
        }
        if(_count)
        {
            _point = new glm::vec2[_count];
        }
    }
}
/** Initialize polygon.
 *  @param [in] buffer Pointer to the point array.
 *  @param [in] count  Number of points 
 *  @param [in] stride Offset between two consecutive points. (default=0)
 */
void Polygon2d::set(const float* buffer, size_t count, size_t stride)
{
    adjust(count);
    for(size_t i=0; i<_count; i++, buffer+=stride)
    {
        _point[i].x = *buffer++;
        _point[i].y = *buffer++;
    }
    computeCenter();
}
/** Copy operator.
 *  @param [in] box Source polygon.
 */
Polygon2d& Polygon2d::operator= (const Polygon2d& poly)
{
    adjust(poly._count);
    if(_count)
    {
        memcpy(_point, poly._point, _count*sizeof(glm::vec2));
    }
    _center = poly._center;
    return *this;
}
    
/** Apply transformation.
 *  @param [in] m 3*3 transformation matrix.
 */
void Polygon2d::transform(const glm::mat3& m)
{
    for(size_t i=0; i<_count; i++)
    {
        glm::vec3 pt = m * glm::vec3(_point[i], 1.0f);
        _point[i] = glm::vec2(pt.x, pt.y);
    }
}
/**
 * Find polygon support point.
 * @param [in] direction Normalized direction vector.
 */
glm::vec2 Polygon2d::support(const glm::vec2& direction) const
{
    struct
    {
        size_t index;
        float  value;
    } candidate;
    float current;
    
    candidate.index = 0;
    candidate.value = -std::numeric_limits<double>::infinity();
    for(size_t i=0; i<_count; i++)
    {
        current = glm::dot(direction, _point[i]);
        if(current > candidate.value)
        {
            candidate.value = current;
            candidate.index = i;
        }
    }
    return _point[candidate.index];
}
/** Get polygon center. **/
const glm::vec2& Polygon2d::getCenter() const
{ return _center; }
/** Get point count. **/
size_t Polygon2d::getCount() const
{ return _count; }
/** Get point (unsafe). **/
const glm::vec2& Polygon2d::getPoint(size_t index) const
{
    return _point[index];
}
/** Compute center. **/
void Polygon2d::computeCenter()
{
    float area, da;
    area = 0.0f;
    _center.x = 0.0f;
    _center.y = 0.0f;
    for(size_t i=0, j=_count-1; i<_count; i++)
    {
        da = (_point[j].x * _point[i].y) - (_point[j].y * _point[i].x);
        _center += da * (_point[j] + _point[i]);
        area += da;
        j = i;
    }
    _center /= (3.0f * area);
}

}
