#include <UnitTest++/UnitTest++.h>
#include "boundingsphere.hpp"

using namespace Dumb::Framework;

SUITE(BoundingSphere)
{
    TEST(ContainsPoint)
    {
        BoundingSphere sphere(glm::vec3(1.0f), 1.0f);
        ContainmentType::Value res = sphere.contains(glm::vec3(0.5f));
        CHECK_EQUAL(ContainmentType::Contains, res);
    }
}