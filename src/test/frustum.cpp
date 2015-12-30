#include <UnitTest++/UnitTest++.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <DumbFramework/geometry/boundingsphere.hpp>
#include <DumbFramework/geometry/boundingbox.hpp>
#include <DumbFramework/geometry/frustum.hpp>

using namespace Dumb::Core::Geometry;

SUITE(Frustum)
{
    TEST(ContainsSphere)
    {
        glm::mat4 camera;
        glm::mat4 projection;
        
        glm::vec3 eye   ( 1.0f, 3.0f,-3.0f);
        glm::vec3 target( 1.0f, 4.0f,-1.0f);
        glm::vec3 up    ( 0.0f, 1.0f, 0.0f);
        
        camera     = glm::lookAt(eye, target, up);
        projection = glm::perspective(60.0f, 4.0f/3.0f, 1.0f, 10.0f);
        
        ContainmentType::Value ret;
        
        Frustum frustum(camera, projection);
        
        BoundingSphere sphere(eye + frustum.getNear().getNormal() * 4.0f, 0.5f);
        
        ret = frustum.contains(sphere);
        CHECK_EQUAL(ContainmentType::Contains, ret);
        
        glm::vec3 center = eye;
        sphere = BoundingSphere(center, 2.0f);
        ret = frustum.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, ret);
        
        center = eye + frustum.getNear().getNormal() * 2.0f + frustum.getTop().getNormal();
        sphere = BoundingSphere(center, 1.0f);
        ret = frustum.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, ret);

        center = eye + frustum.getFar().getNormal() * 200.0f;
        sphere = BoundingSphere(center, 12.0f);
        ret = frustum.contains(sphere);
        CHECK_EQUAL(ContainmentType::Disjoints, ret);

        center = eye - (frustum.getBottom().getNormal() + frustum.getLeft().getNormal()) * 11.0f;
        sphere = BoundingSphere(center, 12.0f);
        ret = frustum.contains(sphere);
        CHECK_EQUAL(ContainmentType::Disjoints, ret);
    }

    TEST(ContainsPointList)
    {
        glm::mat4 camera;
        glm::mat4 projection;
        
        glm::vec3 eye   ( 1.1f, 7.8f, 11.2f);
        glm::vec3 target( 5.7f, 5.1f, 8.0f);
        glm::vec3 up    ( 0.0f, 1.0f, 0.0f);
        
        camera     = glm::lookAt(eye, target, up);
        projection = glm::perspective(70.0f, 16.0f/9.0f, 2.0f, 100.0f);

        Frustum frustum(camera, projection);
        ContainmentType::Value ret;
        
        glm::vec3 center = eye + frustum.getNear().getNormal() * 4.0f;
        
        std::vector<float> pointList;
        size_t count = 11;
        for(size_t i=0; i<count; i++)
        {
            glm::vec3 dummy = center + glm::ballRand(2.0f);
            pointList.push_back(dummy.x);
            pointList.push_back(dummy.y);
            pointList.push_back(dummy.z);
            pointList.push_back(0.0f);
        }
        ret = frustum.contains(&pointList[0], count, 1);
        CHECK_EQUAL(ContainmentType::Contains, ret);
        
        pointList.clear();
        count = 22;
        center = eye;
        for(size_t i=0; i<count; i++)
        {
            glm::vec3 dummy = center + glm::ballRand(31.0f);
            pointList.push_back(dummy.x);
            pointList.push_back(dummy.y);
            pointList.push_back(dummy.z);
        }
        ret = frustum.contains(&pointList[0], count, 0);
        CHECK_EQUAL(ContainmentType::Intersects, ret);
        
        pointList.clear();
        count = 9;
        center = eye + frustum.getFar().getNormal() * 4.0f + frustum.getTop().getNormal() * 7.0f;
        for(size_t i=0; i<count; i++)
        {
            glm::vec3 dummy = center + glm::ballRand(3.3f);
            pointList.push_back(dummy.x);
            pointList.push_back(dummy.y);
            pointList.push_back(dummy.z);
        }
        ret = frustum.contains(&pointList[0], count, 0);
        CHECK_EQUAL(ContainmentType::Disjoints, ret);
    }

    TEST(ContainsPoint)
    {
        glm::mat4 camera;
        glm::mat4 projection;
        
        glm::vec3 eye   (-2.2f,-3.0f, 4.0f);
        glm::vec3 target(-2.0f, 1.1f, 5.0f);
        glm::vec3 up    ( 0.0f, 1.0f, 0.0f);
        
        camera     = glm::lookAt(eye, target, up);
        projection = glm::perspective(80.0f, 16.0f/9.0f, 4.0f, 100.0f);

        Frustum frustum(camera, projection);
        
        ContainmentType::Value ret;

        ret = frustum.contains(eye + 11.0f * frustum.getNear().getNormal());
        CHECK_EQUAL(ContainmentType::Contains, ret);

        ret = frustum.contains(eye + 34.0f * frustum.getTop().getNormal());
        CHECK_EQUAL(ContainmentType::Disjoints, ret);

        ret = frustum.contains(eye + 4.0f * frustum.getNear().getNormal());
        CHECK_EQUAL(ContainmentType::Intersects, ret);
    }

    TEST(ContainsBox)
    {
        /// @todo
    }

    TEST(IntersectsRay)
    {
        /// @todo
    }
}
