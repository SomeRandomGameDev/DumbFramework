#ifndef _DUMB_FW_BOUNDING_CIRCLE_
#define _DUMB_FW_BOUNDING_CIRCLE_

#include "boundingobjects.hpp"

namespace Framework {

/**
 * Bounding circle.
 */
class BoundingCircle
{
public:
    /** Constructor. */
    BoundingCircle();
    /** Constructor. 
     *  @param [in] c  Bounding circle center.
     *  @param [in] r  Bounding sphere radius.
     */
    BoundingCircle(const glm::vec2& c, float r);
    /** Constructor.
     *  @param [in] buffer Pointer to the point array.
     *  @param [in] count  Number of points 
     *  @param [in] stride Offset between two consecutive points. (default=0)
     */
    BoundingCircle(const float* buffer, size_t count, size_t stride=0);
    /** Constructor.
     *  Merge two bounding circles.
     */
    BoundingCircle(const BoundingCircle& c0, const BoundingCircle& c1);
    /** Copy constructor.
     *  @param [in] circle Source bounding circle.
     */
    BoundingCircle(const BoundingCircle& circle);

    /** Copy operator.
     *  @param [in] circle Source bounding circle.
     */
    BoundingCircle& operator= (const BoundingCircle& circle);

    /** Check if the current bounding circle contains the specified bounding circle. */
    ContainmentType::Value contains(const BoundingCircle& circle);
    /** Check if the current bounding circle contains the specified bounding quad. */
    ContainmentType::Value contains(const BoundingQuad& quad);
    /** Check if the current bounding circle contains the specified list of points.
     *  @param [in] buffer Pointer to the point array.
     *  @param [in] count  Number of points 
     *  @param [in] stride Offset between two consecutive points. (default=0)
     */
    ContainmentType::Value contains(const float* buffer, size_t count, size_t stride=0);
    /** Check if the current bounding circle contains the specified point.
     *  @param [in] point Point to be tested.
     */
    ContainmentType::Value contains(const glm::vec2& point);
    /** Check if the current bounding circle intersects the specified ray.
     *  @param [in] ray Ray to be tested.
     */
    bool intersects(const Ray2d& ray);
    /** Tell on which side of the specified line the current bounding circle is.
     *  @param [in] line Line.
     */
    Side classify(const Line2d& line);
    /** Apply transformation.
	 *  @param [in] m 3*3 transformation matrix.
	 */
	void transform(const glm::mat3& m);
    /**
	 * Find bounding circle support point.
	 * @param [in] direction Normalized direction vector.
	 */
	glm::vec2 support(const glm::vec2& direction);
    /** Get circle center. **/
    const glm::vec2& getCenter() const;
    /** Get circle radius. **/
    float getRadius() const;
    /** Get circle squared radius. **/
    float getSquareRadius() const;
    
    /// @todo : contains/classify rectangle, ray
    
private:
    /** Center  */
    glm::vec2 _center;
    /** Radius */
    float _radius;
    /** Square radius */
    float _squareRadius;
};

}

#endif /* _DUMB_FW_BOUNDING_CIRCLE_ */
