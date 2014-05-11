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
        /// @todo
	}
	
	TEST(ContainsRay)
	{
        /// @todo
	}
    
    TEST(ContainsBox)
    {
        /// @todo
    }
}
