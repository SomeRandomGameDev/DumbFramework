#ifndef _DUMB_FW_BOUNDING_QUAD_
#define _DUMB_FW_BOUNDING_QUAD_

#include <DumbFramework/boundingobjects.hpp>

namespace Framework {

/**
 * Axis aligned bounding quad.
 */
class BoundingQuad
{
public:
    /** Constructor. */
    BoundingQuad();
    /** Constructor. 
     *  @param [in] bmin Bounding quad minimum point.
     *  @param [in] bmax Bounding quad maximum point.
     */
    BoundingQuad(const glm::vec2& bmin, const glm::vec2& bmax);
    /** Constructor.
     *  @param [in] buffer Pointer to the point array.
     *  @param [in] count  Number of points 
     *  @param [in] stride Offset between two consecutive points. (default=0)
     */
    BoundingQuad(const float* buffer, size_t count, size_t stride=0);
    /** Constructor.
     *  @param [in] circle Bounding circle.
     */
    BoundingQuad(const BoundingCircle& circle);
    /** Constructor.
     *  Merge two bounding quads.
     */
    BoundingQuad(const BoundingQuad& q0, const BoundingQuad& q1);
    /** Copy constructor.
     *  @param [in] quad Source bounding quad.
     */
    BoundingQuad(const BoundingQuad& quad);
    /** Copy operator.
     *  @param [in] quad Source bounding quad.
     */
    BoundingQuad& operator= (const BoundingQuad& quad);
    /** Check if the current bounding quad contains the specified bounding quad. */
    ContainmentType::Value contains(const BoundingQuad& quad);
    /** Check if the current bounding quad contains the specified bounding circle. */
    ContainmentType::Value contains(const BoundingCircle& circle);
    /** Check if the current bounding quad contains the specified list of points.
     *  @param [in] buffer Pointer to the point array.
     *  @param [in] count  Number of points 
     *  @param [in] stride Offset between two consecutive points. (default=0)
     */
    ContainmentType::Value contains(const float* buffer, size_t count, size_t stride=0);
    /** Check if the current bounding quad contains the specified point.
     *  @param [in] point Point to be tested.
     */
    ContainmentType::Value contains(const glm::vec2& point);
    /** Check if the current bounding quad intersects the specified ray.
     *  @param [in] ray Ray to be tested.
     */
    bool intersects(const Ray2d& ray);
    /** Tell on which side of the specified line the current bounding quad is.
     *  @param [in] line Line.
     */
    Side classify(const Line2d& line) const;
    /** Apply transformation.
     *  @param [in] m 3*3 transformation matrix.
     */
    void transform(const glm::mat3& m);
    /** Get lowest quad corner. **/
    const glm::vec2& getMin() const;
    /** Get highest quad corner. **/
    const glm::vec2& getMax() const;
    /** Get quad center. **/
    const glm::vec2& getCenter() const;
    /** Get quad extent. **/
    const glm::vec2& getExtent() const;

private:
    /** Update center and extent. **/
    void _update();
	
private:	
    /** Minimum point **/
    glm::vec2 _min;
    /** Maximum point **/
    glm::vec2 _max;
    /** Center **/
    glm::vec2 _center;
    /** Extent **/
    glm::vec2 _extent;
};

}

#endif /* _DUMB_FW_BOUNDING_QUAD_ */
