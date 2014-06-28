#ifndef _DUMB_FW_POLYGON_2D_
#define _DUMB_FW_POLYGON_2D_

#include "boundingobjects.hpp"

namespace Framework {

/**
 * 2D polygon
 */
class Polygon2d
{
public:
    /** Constructor. */
    Polygon2d();
    /** Constructor.
     *  @param [in] buffer Pointer to the point array.
     *  @param [in] count  Number of points 
     *  @param [in] stride Offset between two consecutive points. (default=0)
     */
    Polygon2d(const float* buffer, size_t count, size_t stride=0);
    /** Copy constructor.
     *  @param [in] box Source polygon.
     */
    Polygon2d(const Polygon2d& poly);
    /**
     * Destructor.
     */
    ~Polygon2d();
    /** Copy operator.
     *  @param [in] box Source polygon.
     */
    Polygon2d& operator= (const Polygon2d& poly);
    /** Initialize polygon.
     *  @param [in] buffer Pointer to the point array.
     *  @param [in] count  Number of points 
     *  @param [in] stride Offset between two consecutive points. (default=0)
     */
    void set(const float* buffer, size_t count, size_t stride=0);
    /** Apply transformation.
     *  @param [in] m 3*3 transformation matrix.
     */
    void transform(const glm::mat3& m);
	/**
	 * Find polygon support point.
	 * @param [in] direction Normalized direction vector.
	 */
	glm::vec2 support(const glm::vec2& direction) const;
    /** Get polygon center. **/
    const glm::vec2& getCenter() const;
    /** Get point count. **/
    size_t getCount() const;
    /** Get point (unsafe). **/
    const glm::vec2& getPoint(size_t index) const;
    
private:
    /** Adjust buffer size if needed. **/
    void adjust(size_t count);
    /** Compute center. **/
    void computeCenter();
    
private:	
    /** Points **/
    glm::vec2 *_point;
    /** Point count **/
    size_t _count;
    /** Center **/
    glm::vec2 _center;
};

}

#endif /* _DUMB_FW_POLYGON_2D_ */
