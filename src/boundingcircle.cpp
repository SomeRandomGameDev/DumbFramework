#include <DumbFramework/boundingobjects.hpp>

namespace Framework {
    
/** Constructor. */
BoundingCircle::BoundingCircle()
    : _center(0.0f)
    , _radius(0.0f)
    , _squareRadius(0.0f)
{}
/** Constructor. 
 *  @param [in] c  Bounding circle center.
 *  @param [in] r  Bounding sphere radius.
 */
BoundingCircle::BoundingCircle(const glm::vec2& c, float r)
    : _center(c)
    , _radius(r)
    , _squareRadius(r*r)
{}
/** Constructor.
 *  @param [in] buffer Pointer to the point array.
 *  @param [in] count  Number of points 
 *  @param [in] stride Offset between two consecutive points. (default=0)
 */
BoundingCircle::BoundingCircle(const float* buffer, size_t count, size_t stride)
{
    glm::vec2 pmin;
    glm::vec2 pmax;
    size_t inc = stride + 2;
    pmin = pmax = glm::vec2(buffer[0], buffer[1]);
    for(size_t i=1; i<count; i++, buffer+=inc)
    {
        glm::vec2 dummy(buffer[0], buffer[1]);
        pmin = glm::min(pmin, dummy);
        pmax = glm::max(pmax, dummy);
    }
    _center = (pmax + pmin) / 2.0f;
    _squareRadius = glm::dot(pmax-pmin, pmax-pmin) / 4.0f;
    _radius = sqrt(_squareRadius);
}
/** Constructor.
 *  Merge two bounding circles.
 */
BoundingCircle::BoundingCircle(const BoundingCircle& c0, const BoundingCircle& c1)
{
    float length = glm::distance(c0._center, c1._center);
    float dR     = c0._radius - c1._radius;
    _radius = (c0._radius + c1._radius + length) / 2.0f;
    _squareRadius = _radius * _radius;
    _center = (c0._center * (length - dR) + c1._center * (length + dR)) / (2.0f * length);
}
/** Copy constructor.
 *  @param [in] circle Source bounding circle.
 */
BoundingCircle::BoundingCircle(const BoundingCircle& circle)
    : _center(circle._center)
    , _radius(circle._radius)
    , _squareRadius(circle._squareRadius)
{}

/** Copy operator.
 *  @param [in] circle Source bounding circle.
 */
BoundingCircle& BoundingCircle::operator= (const BoundingCircle& circle)
{
    _center = circle._center;
    _radius = circle._radius;
    _squareRadius = circle._squareRadius;
    return *this;
}

/** Check if the current bounding circle contains the specified bounding circle. */
ContainmentType::Value BoundingCircle::contains(const BoundingCircle& circle)
{
    glm::vec2 delta = _center - circle._center;
    
    float squareDistance = glm::dot(delta, delta);
    float r0r1 = 2.0f * _radius * circle._radius;
    float sqdr0 = _squareRadius + circle._squareRadius + r0r1;
    float sqdr1 = _squareRadius + circle._squareRadius - r0r1;

    if(squareDistance >  sqdr0) { return ContainmentType::Disjoints;  }
    if(squareDistance <= sqdr1) { return ContainmentType::Contains; }
    return ContainmentType::Intersects;
}
/** Check if the current bounding circle contains the specified list of points.
 *  @param [in] buffer Pointer to the point array.
 *  @param [in] count  Number of points 
 *  @param [in] stride Offset between two consecutive points. (default=0)
 */
ContainmentType::Value BoundingCircle::contains(const float* buffer, size_t count, size_t stride)
{
    off_t offset = 0, inc = stride + 2;
    size_t inside = 0;
    for(size_t i=0; i<count; i++, offset+=inc)
    {
        glm::vec2 point(buffer[offset] - _center.x, buffer[offset+1] - _center.y);
        float squareDistance = glm::dot(point, point);
        inside += (_squareRadius < squareDistance) ? 0 : 1;
    }

    if(inside == 0) { return ContainmentType::Disjoints;  }
    if(inside < count) { return ContainmentType::Intersects; }
    return ContainmentType::Contains;
}
/** Check if the current bounding circle contains the specified point.
 *  @param [in] point Point to be tested.
 */
ContainmentType::Value BoundingCircle::contains(const glm::vec2& point)
{
    glm::vec2 delta = point - _center;
    float squareDistance = glm::dot(delta, delta);
    if(squareDistance < _squareRadius) { return ContainmentType::Contains;  }
    if(squareDistance > _squareRadius) { return ContainmentType::Disjoints; }
    return ContainmentType::Intersects;
}
/** Check if the current bounding circle intersects the specified ray.
 *  @param [in] ray Ray to be tested.
 */
bool BoundingCircle::intersects(const Ray2d& ray)
{
    float epsilon = std::numeric_limits<float>::epsilon();
    float distance = glm::distance(_center, ray.origin);
    glm::vec2 diff = _center - ray.origin;
    float t0 = glm::dot(diff, ray.direction);
    float d2 = glm::dot(diff, diff) - (t0 * t0);
    if(d2 > _squareRadius) { return false; }
    float t1 = sqrt(_squareRadius - d2);
    distance = (t0 > (t1 + epsilon)) ? (t0 - t1) : (t0 + t1);
    return (distance > epsilon);
}
/** Tell on which side of the specified line the current bounding circle is.
 *  @param [in] line Line.
 */
Side BoundingCircle::classify(const Line2d& line)
{
    float d = line.distance(_center);
	if(d <= -_radius)
	{ return Side::Back; }
	if(d >= _radius)
	{ return Side::Front; }
	return Side::On;
}
/** Apply transformation.
 *  @param [in] m 3*3 transformation matrix.
 */
void BoundingCircle::transform(const glm::mat3& m)
{
	// Transform center.
	glm::vec3 pt = m * glm::vec3(_center, 1.0f);
	_center = glm::vec2(pt.x, pt.y);
	
	// Find the biggest scale.
	glm::vec2 sx(m[0].x, m[1].x);
	glm::vec2 sy(m[0].y, m[1].y);
	float scale = glm::max(glm::dot(sx,sx), glm::dot(sy,sy));
	_squareRadius = _squareRadius * scale;
	_radius = _radius * sqrt(scale);
}

/** Get circle center. **/
const glm::vec2& BoundingCircle::getCenter() const
{ return _center; }
/** Get circle radius. **/
float BoundingCircle::getRadius() const
{ return _radius; }
/** Get circle squared radius. **/
float BoundingCircle::getSquareRadius() const
{ return _squareRadius; }

}
