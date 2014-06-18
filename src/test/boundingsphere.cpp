#include <unittest++/UnitTest++.h>
#include <vector>
#include <glm/gtc/random.hpp>
#include "boundingobjects.hpp"

using namespace Framework;

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

    TEST(ContainsPointList)
    {
        BoundingSphere s0, s1;
        ContainmentType::Value res;
        std::vector<float> pointList;
        size_t count;
        
        s0 = BoundingSphere(glm::vec3(0.0f, 2.0f, 0.0f), 4.0f);
		s1 = BoundingSphere(glm::vec3(0.5f, 0.5f, 0.5f), 0.25f);
	    
        count = 11;
        for(size_t i=0; i<count; i++)
        {
            glm::vec3 dummy = s1.getCenter() + glm::ballRand(s1.getRadius());
            pointList.push_back(dummy.x);
            pointList.push_back(dummy.y);
            pointList.push_back(dummy.z);
            pointList.push_back(0.0f);
            pointList.push_back(0.0f);
        }
        res = s0.contains(&pointList[0], count, 2);
        CHECK_EQUAL(ContainmentType::Contains, res);
     
        pointList.clear();
        s1 = BoundingSphere(glm::vec3(-10.0f, 4.0f, 0.0f), 1.4f);
		count = 45;
        for(size_t i=0; i<count; i++)
        {
            glm::vec3 dummy = s1.getCenter() + glm::ballRand(s1.getRadius());
            pointList.push_back(dummy.x);
            pointList.push_back(dummy.y);
            pointList.push_back(dummy.z);
        }
        res = s0.contains(&pointList[0], count, 0);
        CHECK_EQUAL(ContainmentType::Disjoints, res);

        pointList.clear();
		s1 = BoundingSphere(glm::vec3(0.0f, 5.0f, -5.0f), 3.125f);
		count = 17;
        for(size_t i=0; i<count; i++)
        {
            glm::vec3 dummy = s1.getCenter() + glm::ballRand(s1.getRadius());
            pointList.push_back(dummy.x);
            pointList.push_back(dummy.y);
            pointList.push_back(dummy.z);
            pointList.push_back(1.0f);
            pointList.push_back(2.0f);
            pointList.push_back(3.0f);
            pointList.push_back(4.0f);
        }
        res = s0.contains(&pointList[0], count, 4);
        CHECK_EQUAL(ContainmentType::Intersects, res);
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
	
	TEST(IntersectsRay)
	{
		BoundingSphere sphere(glm::vec3(1.0f, 0.0f, 0.0f), 2.0f);
		Ray ray(glm::vec3(0.5f), glm::vec3(-1.0f));
		
        bool res;
        
		res = sphere.intersects(ray);
		CHECK_EQUAL(true, res);
		
		ray.origin = glm::vec3(4.0f);
		res = sphere.intersects(ray);
		CHECK_EQUAL(true, res);

		ray.origin = glm::vec3(-6.0f);
		res = sphere.intersects(ray);
		CHECK_EQUAL(false, res);
	}
    
    TEST(ContainsBox)
    {
        BoundingSphere sphere(glm::vec3(1.0f, 1.0f, 1.0f), 2.0f);
        BoundingBox box;
    	ContainmentType::Value res;
	    
        box = BoundingBox::BoundingBox(glm::vec3(0.5f), glm::vec3(1.0f));
        res = sphere.contains(box);
        CHECK_EQUAL(ContainmentType::Contains, res);
		
        box = BoundingBox::BoundingBox(glm::vec3(0.9f,-8.0f, 0.9f), glm::vec3(1.1f, 8.0f, 1.1f));
        res = sphere.contains(box);
        CHECK_EQUAL(ContainmentType::Intersects, res);
		
        box = BoundingBox::BoundingBox(glm::vec3(-1.75f), glm::vec3( 0.25f));
        res = sphere.contains(box);
        CHECK_EQUAL(ContainmentType::Intersects, res);

        box = BoundingBox::BoundingBox(glm::vec3(1.8f), glm::vec3(4.25f));
        res = sphere.contains(box);
        CHECK_EQUAL(ContainmentType::Intersects, res);

        box = BoundingBox::BoundingBox(glm::vec3(-10.25f), glm::vec3( 10.75f));
        res = sphere.contains(box);
        CHECK_EQUAL(ContainmentType::Disjoints, res);
        
        box = BoundingBox::BoundingBox(glm::vec3(0.0f, 0.0f,-120.0f), glm::vec3(1.0f, 1.0f, -80.0f));
        res = sphere.contains(box);
        CHECK_EQUAL(ContainmentType::Disjoints, res);
	 }
}
