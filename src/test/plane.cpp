#include <unittest++/UnitTest++.h>
#include <glm/gtc/random.hpp>
#include "plane.hpp"

using namespace Dumb::Framework;

SUITE(Plane)
{
    TEST(Classify)
    {
        glm::vec3 p0(glm::sphericalRand(10.0f));
        glm::vec3 p1(glm::sphericalRand(10.0f));
        glm::vec3 p2(glm::sphericalRand(10.0f));
        
        Dumb::Framework::Plane plane(p0, p1, p2);
        Dumb::Framework::Plane::Side side;
        
        side = plane.classify(p0);
        CHECK_EQUAL(Dumb::Framework::Plane::On, side);

        side = plane.classify(p1);
        CHECK_EQUAL(Dumb::Framework::Plane::On, side);

        side = plane.classify(p2);
        CHECK_EQUAL(Dumb::Framework::Plane::On, side);

        glm::vec3 p3 = (p0 + (p1+p2)/2.0f) / 2.0f;
        side = plane.classify(p3);
        CHECK_EQUAL(Dumb::Framework::Plane::On, side);
        
        side = plane.classify(p3 + plane.getNormal()*5.0f);
        CHECK_EQUAL(Dumb::Framework::Plane::Front, side);
        
        side = plane.classify((p1+p2)/2.0f - plane.getNormal()*2.0f);
        CHECK_EQUAL(Dumb::Framework::Plane::Back, side);
    }
}
