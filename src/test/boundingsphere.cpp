#include <unittest++/UnitTest++.h>
#include "boundingobjects.hpp"

using namespace Dumb::Framework;

SUITE(BoundingSphere)
{
	TEST(MergeCTOR)
	{
		BoundingSphere s0(glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);
		BoundingSphere s1(glm::vec3(0.0f,-1.0f, 0.0f), 0.5f);
		
		BoundingSphere merged(s0, s1);
		
		ContainmentType::Value res;

		res = merged.contains(s0);
		CHECK_EQUAL(ContainmentType::Contains, res);

		res = merged.contains(s1);
		CHECK_EQUAL(ContainmentType::Contains, res);
	}

    TEST(ContainsPoint)
    {
        BoundingSphere sphere(glm::vec3(1.0f), 1.0f);
        ContainmentType::Value res;
		
		res = sphere.contains(glm::vec3(0.5f));
        CHECK_EQUAL(ContainmentType::Contains, res);

		res = sphere.contains(glm::vec3(1.0f, 0.0, 1.0f));
        CHECK_EQUAL(ContainmentType::Intersects, res);

		res = sphere.contains(glm::vec3(10.0f));
		CHECK_EQUAL(ContainmentType::Disjoints, res);
    }

	TEST(ContainsSphere)
    {
        BoundingSphere big(glm::vec3(0.0f, 2.0f, 0.0f), 4.0f);
		
		ContainmentType::Value res;
        BoundingSphere small;
		
		small = BoundingSphere(glm::vec3(0.5f, 0.5f, 0.5f), 0.25f);
		res = big.contains(small);
		CHECK_EQUAL(ContainmentType::Contains, res);

		res = small.contains(big);
		CHECK_EQUAL(ContainmentType::Contains, res);

		small = BoundingSphere(glm::vec3(-10.0f, 4.0f, 0.0f), 1.4f);
		res = big.contains(small);
		CHECK_EQUAL(ContainmentType::Disjoints, res);

		small = BoundingSphere(glm::vec3(0.0f, 5.0f, -5.0f), 2.125f);
		res = big.contains(small);
		CHECK_EQUAL(ContainmentType::Intersects, res);
	}
	
	TEST(ContainsRay)
	{
		BoundingSphere sphere(glm::vec3(1.0f, 0.0f, 0.0f), 2.0f);
		Ray ray(glm::vec3(0.5f), glm::vec3(-1.0f));
		
		ContainmentType::Value res;
		
		res = sphere.contains(ray);
		CHECK_EQUAL(ContainmentType::Contains, res);
		
		ray.origin = glm::vec3(4.0f);
		res = sphere.contains(ray);
		CHECK_EQUAL(ContainmentType::Intersects, res);

		ray.origin = glm::vec3(-6.0f);
		res = sphere.contains(ray);
		CHECK_EQUAL(ContainmentType::Disjoints, res);
	}
    
    TEST(ContainsBox)
    {
        BoundingSphere sphere(glm::vec3(1.0f, 1.0f, 1.0f), 2.0f);
        BoundingBox box;
    	ContainmentType::Value res;
	    
        box.min = glm::vec3(-0.5f);
        box.max = glm::vec3( 0.5f);
        res = sphere.contains(box);
        CHECK_EQUAL(ContainmentType::Contains, res);
		
        box.min = glm::vec3(-1.75f);
        box.max = glm::vec3( 0.25f);
        res = sphere.contains(box);
        CHECK_EQUAL(ContainmentType::Intersects, res);

        box.min = glm::vec3(1.8f);
        box.max = glm::vec3(3.25f);
        res = sphere.contains(box);
        CHECK_EQUAL(ContainmentType::Intersects, res);

        box.min = glm::vec3(-2.25f);
        box.max = glm::vec3( 3.75f);
        res = sphere.contains(box);
        CHECK_EQUAL(ContainmentType::Disjoints, res);
        
        box.min = glm::vec3(5.125f);
        box.max = glm::vec3(5.75f);
        res = sphere.contains(box);
        CHECK_EQUAL(ContainmentType::Disjoints, res);
	 }
}
