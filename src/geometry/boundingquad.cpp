/*
 * Copyright 2015 MooZ
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <DumbFramework/geometry/boundingcircle.hpp>
#include <DumbFramework/geometry/boundingquad.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

/** Constructor. */
BoundingQuad::BoundingQuad()
    : _min(0.0f)
    , _max(0.0f)
    , _center(0.0f)
    , _extent(0.0f)
{}
/** Constructor. 
*  @param [in] bmin Bounding quad minimum point.
*  @param [in] bmax Bounding quad maximum point.
*/
BoundingQuad::BoundingQuad(glm::vec2 const& bmin, glm::vec2 const& bmax)
    : _min(bmin)
    , _max(bmax)
{
    _update();
}
/** Constructor.
*  @param [in] buffer Pointer to the point array.
*  @param [in] count  Number of points 
*  @param [in] stride Offset between two consecutive points. (default=0)
*/
BoundingQuad::BoundingQuad(const float* buffer, size_t count, size_t stride)
{
    off_t offset, inc;
    inc = stride + 2;
    offset = inc;
    
    _min = _max = glm::vec2(buffer[0], buffer[1]);
    for(size_t i=1; i<count; i++, offset+=inc)
    {
        glm::vec2 dummy(buffer[offset], buffer[offset+1]);
        _min = glm::min(_min, dummy);
        _max = glm::max(_max, dummy);
    }
    
    _update();
}
/** Constructor.
*  @param [in] circle Bounding circle.
*/
BoundingQuad::BoundingQuad(BoundingCircle const& circle)
{
    glm::vec2 direction = glm::vec2(circle.getRadius());
    _min = circle.getCenter() - direction;
    _max = circle.getCenter() + direction;
    _update();
}
/** Constructor.
*  Merge two bounding quads.
*/
BoundingQuad::BoundingQuad(BoundingQuad const& q0, BoundingQuad const& q1)
    : _min(glm::min(q0._min, q1._min))
    , _max(glm::max(q0._max, q1._max))
{
    _update();
}
/** Copy constructor.
*  @param [in] box Source bounding quad.
*/
BoundingQuad::BoundingQuad(BoundingQuad const& quad)
    : _min(quad._min)
    , _max(quad._max)
    , _center(quad._center)
    , _extent(quad._extent)
{}
/** Copy operator.
*  @param [in] box Source bounding quad.
*/
BoundingQuad& BoundingQuad::operator= (BoundingQuad const& quad)
{
    _min    = quad._min;
    _max    = quad._max;
    _center = quad._center;
    _extent = quad._extent;
    return *this;
}
/** Check if the current bounding quad contains the specified bounding quad. */
ContainmentType::Value BoundingQuad::contains(BoundingQuad const& quad)
{
    if((quad._max.x < _min.x) ||
       (quad._min.x > _max.x) ||
       (quad._max.y < _min.y) ||
       (quad._min.y > _max.y))
    { return ContainmentType::Disjoints; }  
    if((quad._min.x >= _min.x) &&
       (quad._max.x <= _max.x) &&
       (quad._min.y >= _min.y) &&
       (quad._max.y <= _max.y))
    { return ContainmentType::Contains; }   
    return ContainmentType::Intersects;
}
/** Check if the current bounding quad contains the specified bounding circle. */
ContainmentType::Value BoundingQuad::contains(BoundingCircle const& circle)
{
    glm::vec2 diffMin = circle.getCenter() - _min;
    glm::vec2 diffMax = _max - circle.getCenter();
    
    if((diffMin.x >= circle.getRadius()) &&
       (diffMin.y >= circle.getRadius()) &&
       (diffMax.x >= circle.getRadius()) &&
       (diffMax.y >= circle.getRadius()))
    { return ContainmentType::Contains; }

    float dmin = 0.0f;
    float dmax = 0.0f;
    glm::vec2 sqrMin = diffMin * diffMin;
    glm::vec2 sqrMax = diffMax * diffMax;
    for(int i=0; i<2; i++)
    {
        dmax += glm::max(sqrMin[i], sqrMax[i]);
        if(diffMin[i] < 0.0f)
        {
            dmin += sqrMin[i];
        }
        else if(diffMax[i] < 0.0f)
        {
            dmin += sqrMax[i];
        }
    }
    float r2 = circle.getSquareRadius();
    if((dmin <= r2) && (r2 <= dmax))
    { return ContainmentType::Intersects; }
    return ContainmentType::Disjoints;
}
/** Check if the current bounding quad contains the specified list of points.
*  @param [in] buffer Pointer to the point array.
*  @param [in] count  Number of points 
*  @param [in] stride Offset between two consecutive points. (default=0)
*/
ContainmentType::Value BoundingQuad::contains(const float* buffer, size_t count, size_t stride)
{
    off_t offset = 0, inc = stride + 2;
    size_t inside = 0;
    for(size_t i=0; i<count; i++, offset+=inc)
    {
        glm::vec2 point(buffer[offset], buffer[offset+1]);
        inside += ((point.x >= _min.x) && (point.x <= _max.x) &&
                   (point.y >= _min.y) && (point.y <= _max.y));
    }

    if(inside == 0) { return ContainmentType::Disjoints;  }
    if(inside < count) { return ContainmentType::Intersects; }
    return ContainmentType::Contains;
}
/** Check if the current bounding quad contains the specified point.
*  @param [in] point Point to be tested.
*/
ContainmentType::Value BoundingQuad::contains(glm::vec2 const& point)
{
    glm::vec2 dummy = glm::abs(_center - point);
    if((dummy.x < _extent.x) &&
       (dummy.y < _extent.y))
    { return ContainmentType::Contains; }
    if((dummy.x > _extent.x) &&
       (dummy.y > _extent.y))
    { return ContainmentType::Disjoints; }
    return ContainmentType::Intersects;
}
/** Check if the current bounding quad intersects the specified ray.
*  @param [in] ray Ray to be tested.
*/
bool BoundingQuad::intersects(Ray2 const& ray)
{
    glm::vec2 t0 = (_min - ray.origin) / ray.direction;
    glm::vec2 t1 = (_max - ray.origin) / ray.direction;

    glm::vec2 t2 = glm::min(t0, t1);
    glm::vec2 t3 = glm::max(t0, t1);
    
    float tmin = glm::max(t2.x, t2.y);
    float tmax = glm::min(t3.x, t3.y);

    if((tmax < 0) || (tmin > tmax))
    { return false; }
    return true;
}
/** Tell on which side of the specified line the current bounding box is.
 *  @param [in] line Line.
 */
Side BoundingQuad::classify(Line2 const& line) const
{
    float radius   = glm::dot(glm::abs(line.getNormal()), _extent);
    float distance = line.distance(_center);
    
    if(distance < -radius) { return Side::Back;  }
    if(distance >  radius) { return Side::Front; }
    return Side::On;
}
/** Apply transformation.
*  @param [in] m 3*3 transformation matrix.
*/
void BoundingQuad::transform(glm::mat3 const& m)
{
    glm::vec2 dummy[2] =
    { 
        glm::vec2(m * glm::vec3(_min,1.0f)),
        glm::vec2(m * glm::vec3(_max,1.0f))
    };
    _min = glm::min(dummy[0], dummy[1]);
    _max = glm::max(dummy[0], dummy[1]);
    _update();
}
/** Get lowest quad corner. **/
glm::vec2 const& BoundingQuad::getMin() const { return _min; }
/** Get highest quad corner. **/
glm::vec2 const& BoundingQuad::getMax() const { return _max; }
/** Get quad center. **/
glm::vec2 const& BoundingQuad::getCenter() const { return _center; }
/** Get quad extent. **/
glm::vec2 const& BoundingQuad::getExtent() const { return _extent; }
/** Update center and extent. **/
void BoundingQuad::_update()
{
    _center = (_min + _max) / 2.0f;
    _extent = glm::abs(_max - _center); 
}

} // Geometry
} // Core
} // Dumb
