#include <unittest++/UnitTest++.h>
#include <glm/gtx/io.hpp>
#include "boundingobjects.hpp"

using namespace Framework;

SUITE(BoundingBox)
{
	TEST(MergeCTOR)
	{
		BoundingBox b0(glm::vec3( 0.0f,-0.5f, 0.0f), glm::vec3( 2.0f, 0.25f, 1.5f));
		BoundingBox b1(glm::vec3(-1.7f, 1.1f, 0.2f), glm::vec3(-0.1f, 4.25f, 6.5f));
		
		BoundingBox merged(b0, b1);
		
        CHECK_EQUAL(merged.getMin(), glm::min(b0.getMin(), b1.getMin()));
        CHECK_EQUAL(merged.getMax(), glm::max(b0.getMax(), b1.getMax()));
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
	    
        box = BoundingBox::BoundingBox(glm::vec3(0.5f), glm::vec3(1.0f));
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Disjoints, res);
		
        box = BoundingBox::BoundingBox(glm::vec3(0.5f, 0.5f,-2.0f), glm::vec3(1.5f, 1.5f, 3.0f));
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, res);
		
        box = BoundingBox::BoundingBox(glm::vec3(-1.75f), glm::vec3( 0.25f));
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, res);

        box = BoundingBox::BoundingBox(glm::vec3(1.8f), glm::vec3(4.25f));
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, res);

        box = BoundingBox::BoundingBox(glm::vec3(-10.25f), glm::vec3( 10.75f));
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Contains, res);
        
        box = BoundingBox::BoundingBox(glm::vec3(0.0f, 0.0f,-120.0f), glm::vec3(1.0f, 1.0f, -80.0f));
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Disjoints, res);
	}
	
	TEST(IntersectsRay)
	{
        BoundingBox box(glm::vec3(-3.1f, 1.2f, -2.6f), glm::vec3(4.5f, 6.7f, 5.1f));
        Ray ray;
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
