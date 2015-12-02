#include <UnitTest++/UnitTest++.h>
#include <glm/gtc/random.hpp>
#include <DumbFramework/geometry/plane.hpp>

using namespace Dumb::Core::Geometry;

SUITE(Plane)
{
    TEST(Classify)
    {
        glm::vec3 p0(glm::sphericalRand(10.0f));
        glm::vec3 p1(glm::sphericalRand(10.0f));
        glm::vec3 p2(glm::sphericalRand(10.0f));
        
        Plane plane(p0, p1, p2);
        Side side;
        
        side = plane.classify(p0);
        CHECK_EQUAL(Side::On, side);

        side = plane.classify(p1);
        CHECK_EQUAL(Side::On, side);

        side = plane.classify(p2);
        CHECK_EQUAL(Side::On, side);

        glm::vec3 p3 = (p0 + (p1+p2)/2.0f) / 2.0f;
        side = plane.classify(p3);
        CHECK_EQUAL(Side::On, side);

        side = plane.classify(p3 + plane.getNormal()*5.0f);
        CHECK_EQUAL(Side::Front, side);
        
        side = plane.classify((p1+p2)/2.0f - plane.getNormal()*2.0f);
        CHECK_EQUAL(Side::Back, side);
    }
    
    TEST(Intersects)
    {
        Ray3  ray;
        float distance;
        bool  ret;

        glm::vec3 p0(glm::sphericalRand(32.0f));
        glm::vec3 p1(glm::sphericalRand(32.0f));
        glm::vec3 p2(glm::sphericalRand(32.0f));
        
        Plane plane(p0, p1, p2);
        plane.normalize();
        
        ray.origin    = (p0+p1) / 2.0f + plane.getNormal() * 5.0f;
        ray.direction = plane.getNormal();
        ret = plane.intersects(ray, distance);
        CHECK_EQUAL(false, ret);
    
        ray.direction = -plane.getNormal();
        ret = plane.intersects(ray, distance);
        CHECK_EQUAL(true, ret);
        CHECK_CLOSE(5.0f, distance, 0.0001f);
        
        glm::vec3 target = p0 + glm::normalize(p1-p0) * 7.1f;
        ray.origin   += (p2-p1) * 4.5f;
        ray.direction = glm::normalize(target - ray.origin);
        ret = plane.intersects(ray, distance);
        CHECK_EQUAL(true, ret);
        CHECK_CLOSE(glm::distance(ray.origin, target), distance, 0.001f);
        
        ray.direction *= -1.0f;
        ret = plane.intersects(ray, distance);
        CHECK_EQUAL(false, ret);
        
        ray.origin    = p0 + (p1-p0)*9.0f - plane.getNormal()*21.0f;
        ray.direction = plane.getNormal();
        ret = plane.intersects(ray, distance);
        CHECK_EQUAL(true, ret);
        CHECK_CLOSE(21.0f, distance, 0.0001f);
        
        ray.origin    = p0 + (p1-p0)*9.0f - plane.getNormal()*21.0f;
        ray.direction = glm::normalize(p2-p1);
        ret = plane.intersects(ray, distance);
        CHECK_EQUAL(false, ret);
        CHECK_EQUAL(-std::numeric_limits<float>::max(), distance);
    }
    
    TEST(ClosesPoint)
    {
        glm::vec3 p0(glm::sphericalRand(17.0f));
        glm::vec3 p1(glm::sphericalRand(17.0f));
        glm::vec3 p2(glm::sphericalRand(17.0f));
        
        Plane plane(p0, p1, p2);
        plane.normalize();
        
        float distance = 34.0f;
        
        glm::vec3 dir      = glm::cross(plane.getNormal(), glm::normalize(p1-p0));
        glm::vec3 dummy    = p2 - (dir + plane.getNormal()) * distance;
        glm::vec3 nearest  = plane.closestPoint(dummy);
      
        glm::vec3 expected = dummy + plane.getNormal() * distance;
                
        CHECK_CLOSE(expected.x, nearest.x, 0.0001f);
        CHECK_CLOSE(expected.y, nearest.y, 0.0001f);
        CHECK_CLOSE(expected.z, nearest.z, 0.0001f);
    }

}
