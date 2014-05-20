#include <unittest++/UnitTest++.h>
#include <glm/gtc/matrix_transform.hpp>
#include "boundingobjects.hpp"

using namespace Dumb;

SUITE(BoundingFrustum)
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
        
        BoundingFrustum frustum(camera, projection);
        
        BoundingSphere sphere(eye + frustum.getNear().getNormal() * 4.0f, 0.5f);
        
        ret = frustum.contains(sphere);
        CHECK_EQUAL(ContainmentType::Contains, ret);
        
        sphere.center = eye;
        sphere.radius = 2.0f;
        ret = frustum.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, ret);
        
        sphere.center = eye + frustum.getNear().getNormal() * 2.0f + frustum.getTop().getNormal();
        sphere.radius = 1.0f;
        ret = frustum.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, ret);

        sphere.center = eye + frustum.getFar().getNormal() * 200.0f;
        sphere.radius = 12.0f;
        ret = frustum.contains(sphere);
        CHECK_EQUAL(ContainmentType::Disjoints, ret);

        sphere.center = eye - (frustum.getBottom().getNormal() + frustum.getLeft().getNormal()) * 11.0f;
        ret = frustum.contains(sphere);
        CHECK_EQUAL(ContainmentType::Disjoints, ret);
    }

    TEST(ContainsPointList)
    {
        /// @todo
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

        BoundingFrustum frustum(camera, projection);
        
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
