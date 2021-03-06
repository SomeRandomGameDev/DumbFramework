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
#include <DumbFramework/geometry/frustum.hpp>

namespace Dumb     {
namespace Core     {
namespace Geometry {

/** Constructor. */
Frustum::Frustum()
    : _camera()
    , _projection()
{}
/** Build frustum from camera and projection matrices.
 * @param [in] camera Camera matrix.
 * @param [in] projection Projection matrix.
 */
Frustum::Frustum(glm::mat4 const& camera, glm::mat4 const& projection)
    : _camera(camera)
    , _projection(projection)
{
    float clip[16];
    glm::vec4 plane;
    
    clip[ 0] =  camera[0][0] * projection[0][0];
    clip[ 1] =  camera[0][1] * projection[1][1];
    clip[ 2] =  camera[0][2] * projection[2][2];
    clip[ 3] = -camera[0][2];

    clip[ 4] =  camera[1][0] * projection[0][0];
    clip[ 5] =  camera[1][1] * projection[1][1];
    clip[ 6] =  camera[1][2] * projection[2][2];
    clip[ 7] = -camera[1][2];

    clip[ 8] =  camera[2][0] * projection[0][0];
    clip[ 9] =  camera[2][1] * projection[1][1];
    clip[10] =  camera[2][2] * projection[2][2];
    clip[11] = -camera[2][2];

    clip[12] =  camera[3][0] * projection[0][0];
    clip[13] =  camera[3][1] * projection[1][1];
    clip[14] =  camera[3][2] * projection[2][2] + projection[3][2];
    clip[15] = -camera[3][2];

    // right
    plane.x = clip[ 3] - clip[ 0];
    plane.y = clip[ 7] - clip[ 4];
    plane.z = clip[11] - clip[ 8];
    plane.w = clip[15] - clip[12];
    _planes[FRUSTUM_PLANE_RIGHT] = plane;

    // left
    plane.x = clip[ 3] + clip[ 0];
    plane.y = clip[ 7] + clip[ 4];
    plane.z = clip[11] + clip[ 8];
    plane.w = clip[15] + clip[12];
    _planes[FRUSTUM_PLANE_LEFT] = plane;

    // bottom
    plane.x = clip[ 3] + clip[ 1];
    plane.y = clip[ 7] + clip[ 5];
    plane.z = clip[11] + clip[ 9];
    plane.w = clip[15] + clip[13];
    _planes[FRUSTUM_PLANE_BOTTOM] = plane;

    // top
    plane.x = clip[ 3] - clip[ 1];
    plane.y = clip[ 7] - clip[ 5];
    plane.z = clip[11] - clip[ 9];
    plane.w = clip[15] - clip[13];
    _planes[FRUSTUM_PLANE_TOP] = plane;
    
    // far
    plane.x = clip[ 3] - clip[ 2];
    plane.y = clip[ 7] - clip[ 6];
    plane.z = clip[11] - clip[10];
    plane.w = clip[15] - clip[14];
    _planes[FRUSTUM_PLANE_FAR] = plane;

    // near
    plane.x = clip[ 3] + clip[ 2];
    plane.y = clip[ 7] + clip[ 6];
    plane.z = clip[11] + clip[10];
    plane.w = clip[15] + clip[14];
    _planes[FRUSTUM_PLANE_NEAR] = plane;
}
/** Copy constructor.
 * @param [in] frustum Source bounding frustum.
 */
Frustum::Frustum(Frustum const& frustum)
    : _camera(frustum._camera)
    , _projection(frustum._projection)
{
    for(size_t i=0; i<FRUSTUM_PLANE_COUNT; i++)
    {
        _planes[i] = frustum._planes[i];
    }
}
/** Copy operator.
 * @param [in] frustum Source bounding frustum.
 */
Frustum& Frustum::operator= (Frustum const& frustum)
{
    _camera = frustum._camera;
    _projection = frustum._projection;
    for(size_t i=0; i<FRUSTUM_PLANE_COUNT; i++)
    {
        _planes[i] = frustum._planes[i];
    }
    return *this;
}
/** Check if the current bounding frustum contains the specified bounding box. */
ContainmentType::Value Frustum::contains(BoundingBox const& box)
{
    const glm::vec3& bmin = box.getMin();
    const glm::vec3& bmax = box.getMax();
    
    glm::vec3 neg;
    glm::vec3 pos;

    for(int i=0; i<FRUSTUM_PLANE_COUNT; i++)
    {
        const glm::vec3& pnormal = _planes[i].getNormal();
        neg.x = (pnormal.x > 0) ? bmin.x : bmax.x;
        pos.x = (pnormal.x > 0) ? bmax.x : bmin.x;
        neg.y = (pnormal.y > 0) ? bmin.y : bmax.y;
        pos.y = (pnormal.y > 0) ? bmax.y : bmin.y;
        neg.z = (pnormal.z > 0) ? bmin.z : bmax.z;
        pos.z = (pnormal.z > 0) ? bmax.z : bmin.z;
        if(_planes[i].distance(neg) < 0)
        {
            return ContainmentType::Disjoints;
        }
        if(_planes[i].distance(pos) < 0)
        {
            return ContainmentType::Intersects;
        }
    }

    return ContainmentType::Contains;
}
/** Check if the current bounding frustum contains the specified bounding sphere. */
ContainmentType::Value Frustum::contains(BoundingSphere const& sphere)
{
    Side side;
    bool intersects = false;
    
    for(int i=0; i<FRUSTUM_PLANE_COUNT; i++)
    {
        side = sphere.classify(_planes[i]);
        if(Side::Back == side)
        { return ContainmentType::Disjoints; }
        intersects = intersects || (Side::On == side);
    }
    
    return intersects ? ContainmentType::Intersects : ContainmentType::Contains;
}
/** Check if the current bounding frustum contains the specified bounding frustum. */
ContainmentType::Value Frustum::contains(Frustum const& frustum)
{
    /// @todo
    (void)frustum;
    return ContainmentType::Disjoints;
}
/** Check if the current bounding frustum contains the specified list of points.
 * @param [in] buffer Pointer to the point array.
 * @param [in] count Number of points
 * @param [in] stride Offset between two consecutive points. (default=0)
 */
ContainmentType::Value Frustum::contains(const float* buffer, size_t count, size_t stride)
{
    Side side;
    size_t out = 0;
    stride += 3;
    for(size_t j=0; j<count; j++, buffer+=stride)
    {
        glm::vec3 point(buffer[0], buffer[1], buffer[2]);
        side = Side::Front;
        for(int i=0; (i<FRUSTUM_PLANE_COUNT) && (Side::Back != side); i++)
        {
            side = _planes[i].classify(point);
        }
        out += (Side::Back != side);
    }
    if(out == count)
    { return ContainmentType::Contains; }
    if(out == 0)
    { return ContainmentType::Disjoints; }
    return ContainmentType::Intersects;
}
/** Check if the current bounding box contains the specified point.
 * @param [in] point Point to be tested.
 */
ContainmentType::Value Frustum::contains(glm::vec3 const& point)
{
    Side side;
    bool intersects = false;
    
    for(int i=0; i<FRUSTUM_PLANE_COUNT; i++)
    {
        side = _planes[i].classify(point);
        if(Side::Back == side)
        { return ContainmentType::Disjoints; }
        intersects = intersects || (Side::On == side);
    }
    
    return intersects ? ContainmentType::Intersects : ContainmentType::Contains;
}
/** Check if the current bounding box intersects the specified ray.
 * @param [in] ray Ray to be tested.
 */
bool Frustum::intersects(Ray3 const& ray)
{
    /// @todo
    (void)ray;
    return false;
}
/** Get camera matrix used to build frustum planes. **/
glm::mat4 const& Frustum::getCameraMatrix() const { return _camera; }
/** Get projection matrix used to build frustum planes. **/
glm::mat4 const& Frustum::getProjectionMatrix() const { return _projection; }
/** Get near plane. **/
Plane const& Frustum::getNear() const { return _planes[FRUSTUM_PLANE_NEAR]; }
/** Get far plane. **/
Plane const& Frustum::getFar() const { return _planes[FRUSTUM_PLANE_FAR]; }
/** Get top plane. **/
Plane const& Frustum::getTop() const { return _planes[FRUSTUM_PLANE_TOP]; }
/** Get bottom plane. **/
Plane const& Frustum::getBottom() const { return _planes[FRUSTUM_PLANE_BOTTOM]; }
/** Get left plane. **/
Plane const& Frustum::getLeft() const { return _planes[FRUSTUM_PLANE_LEFT]; }
/** Get right plane. **/
Plane const& Frustum::getRight() const { return _planes[FRUSTUM_PLANE_RIGHT]; }

} // Geometry
} // Core
} // Dumb
