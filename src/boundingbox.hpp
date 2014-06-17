#ifndef _DUMB_FW_BOUNDING_BOX_
#define _DUMB_FW_BOUNDING_BOX_

#include "boundingobjects.hpp"

namespace Framework {

/**
 * Axis aligned bounding box.
 */
class BoundingBox
{
public:
    /** Constructor. */
    BoundingBox();
    /** Constructor. 
     *  @param [in] bmin Bounding box minimum point.
     *  @param [in] bmax Bounding box maximum point.
     */
    BoundingBox(const glm::vec3& bmin, const glm::vec3& bmax);
    /** Constructor.
     *  @param [in] buffer Pointer to the point array.
     *  @param [in] count  Number of points 
     *  @param [in] stride Offset between two consecutive points. (default=0)
     */
    BoundingBox(const float* buffer, size_t count, size_t stride=0);
    /** Constructor.
     *  @param [in] sphere Bounding sphere.
     */
    BoundingBox(const BoundingSphere& sphere);
    /** Constructor.
     *  Merge two bounding boxes.
     */
    BoundingBox(const BoundingBox& b0, const BoundingBox& b1);
    /** Copy constructor.
     *  @param [in] box Source bounding box.
     */
    BoundingBox(const BoundingBox& box);
    /** Copy operator.
     *  @param [in] box Source bounding box.
     */
    BoundingBox& operator= (const BoundingBox& box);
    /** Check if the current bounding box contains the specified bounding box. */
    ContainmentType::Value contains(const BoundingBox& box);
    /** Check if the current bounding box contains the specified bounding sphere. */
    ContainmentType::Value contains(const BoundingSphere& sphere);
    /** Check if the current bounding box contains the specified list of points.
     *  @param [in] buffer Pointer to the point array.
     *  @param [in] count  Number of points 
     *  @param [in] stride Offset between two consecutive points. (default=0)
     */
    ContainmentType::Value contains(const float* buffer, size_t count, size_t stride=0);
    /** Check if the current bounding box contains the specified point.
     *  @param [in] point Point to be tested.
     */
    ContainmentType::Value contains(const glm::vec3& point);
    /** Check if the current bounding box intersects the specified ray.
     *  @param [in] ray Ray to be tested.
     */
    bool intersects(const Ray& ray);
    /** Tell on which side of the specified plane the current bounding box is.
     *  @param [in] plane Plane.
     */
    Side classify(const Plane& plane) const;
    /** Apply transformation.
     *  @param [in] m 4*4 transformation matrix.
     */
    void transform(const glm::mat4& m);
    /** Get lowest box corner. **/
    const glm::vec3& getMin() const;
    /** Get highest box corner. **/
    const glm::vec3& getMax() const;
    /** Get box center. **/
    const glm::vec3& getCenter() const;
    /** Get box extent. **/
    const glm::vec3& getExtent() const;

private:
    /** Update center and extent. **/
    void _update();
	
private:	
    /** Minimum point **/
    glm::vec3 _min;
    /** Maximum point **/
    glm::vec3 _max;
    /** Center **/
    glm::vec3 _center;
    /** Extent **/
    glm::vec3 _extent;
};

}

#endif /* _DUMB_FW_BOUNDING_BOX_ */
