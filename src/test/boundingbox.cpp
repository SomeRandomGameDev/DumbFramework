#include <unittest++/UnitTest++.h>
#include <glm/gtx/io.hpp>
#include "boundingobjects.hpp"

using namespace Dumb::Framework;

SUITE(BoundingBox)
{
	TEST(MergeCTOR)
	{
		BoundingBox b0(glm::vec3( 0.0f,-0.5f, 0.0f), glm::vec3( 2.0f, 0.25f, 1.5f));
		BoundingBox b1(glm::vec3(-1.7f, 1.1f, 0.2f), glm::vec3(-0.1f, 4.25f, 6.5f));
		
		BoundingBox merged(b0, b1);
		
        CHECK_EQUAL(merged.min, glm::min(b0.min, b1.min));
        CHECK_EQUAL(merged.max, glm::max(b0.max, b1.max));
	}

    TEST(ContainsPoint)
    {
        BoundingBox box(glm::vec3(-1.25f), glm::vec3(0.5f));
        ContainmentType::Value res;
		
		res = box.contains(glm::vec3(-0.75f,-0.5f, 0.125f));
        CHECK_EQUAL(ContainmentType::Contains, res);
        
		res = box.contains(glm::vec3(-1.25f,-0.625f,0.125f));
        CHECK_EQUAL(ContainmentType::Intersects, res);

		res = box.contains(glm::vec3(1.5f));
        CHECK_EQUAL(ContainmentType::Disjoints, res);
    }

    TEST(ContainsPointList)
    {
        /// @todo
    }
    
	TEST(ContainsSphere)
    {
        BoundingSphere sphere(glm::vec3(1.0f, 1.0f, 1.0f), 2.0f);
        BoundingBox box;
    	ContainmentType::Value res;
	    
        box.min = glm::vec3(0.5f);
        box.max = glm::vec3(1.0f);
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Disjoints, res);
		
        box.min = glm::vec3(0.5f, 0.5f,-2.0f);
        box.max = glm::vec3(1.5f, 1.5f, 3.0f);
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, res);
		
        box.min = glm::vec3(-1.75f);
        box.max = glm::vec3( 0.25f);
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, res);

        box.min = glm::vec3(1.8f);
        box.max = glm::vec3(4.25f);
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, res);

        box.min = glm::vec3(-10.25f);
        box.max = glm::vec3( 10.75f);
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Contains, res);
        
        box.min = glm::vec3(0.0f, 0.0f,-120.0f);
        box.max = glm::vec3(1.0f, 1.0f, -80.0f);
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Disjoints, res);
	}
	
	TEST(IntersectsRay)
	{
        Dumb::Framework::BoundingBox box(glm::vec3(-3.1f, 1.2f, -2.6f), glm::vec3(4.5f, 6.7f, 5.1f));
        Dumb::Framework::Ray ray;
        bool res;
        
        ray.origin    = glm::vec3(-1.4f, 3.2f, 1.1f);
        ray.direction = glm::normalize(glm::vec3(-1.0f, 1.0f, 0.5f));
        res = box.intersects(ray);
        CHECK_EQUAL(true, res);
        
        ray.origin    = glm::vec3(6.25f,-2.7f,-8.8f);
        ray.direction = glm::normalize(glm::vec3(-1.0f, 0.0f,-1.0f));
        res = box.intersects(ray);
        CHECK_EQUAL(false, res);

        ray.direction = glm::normalize(glm::vec3(-1.0f,-1.0f, 1.0f));
        res = box.intersects(ray);
        CHECK_EQUAL(false, res);

        ray.origin = glm::vec3(5.7f, 8.95f, 6.25f);
        ray.direction = glm::normalize(glm::vec3(-1.0f,-1.0f,-1.0f));
        res = box.intersects(ray);
        CHECK_EQUAL(true, res);
	}
    
    TEST(ContainsBox)
    {
        /// @todo
    }
}
