#include <UnitTest++/UnitTest++.h>
#include <vector>
#include <glm/gtc/random.hpp>
#include <glm/gtx/io.hpp>
#include <DumbFramework/geometry/boundingbox.hpp>
#include <DumbFramework/geometry/boundingsphere.hpp>

using namespace Dumb::Core::Geometry;

namespace glm    {
namespace detail {
    // [todo] remove this.
    std::ostream& operator<< (std::ostream& out, const glm::vec3& v)
    {
        return (out << v.x << ',' << v.y << ',' << v.z);
    }
}
}

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
        ContainmentType::Value ret;
        std::vector<float> pointList;
        size_t count;
        float radius;
        
        BoundingBox b0, b1;
        
        b0 = BoundingBox(glm::vec3(-1.0f,-2.0f,-1.0f), glm::vec3( 1.1f, 0.2f, 1.2f));
        
        b1 = BoundingBox(glm::vec3(-0.7f,-1.1f,-0.5f), glm::vec3( 1.0f, 0.0f, 0.1f));
        radius = glm::length(b1.getExtent()) / 2.0f;
        count = 11;
        for(size_t i=0; i<count; i++)
        {
            glm::vec3 dummy = b1.getCenter() + glm::ballRand(radius);
            pointList.push_back(dummy.x);
            pointList.push_back(dummy.y);
            pointList.push_back(dummy.z);
            pointList.push_back(1.0f);
            pointList.push_back(2.0f);
            pointList.push_back(3.0f);
        }
        ret = b0.contains(&pointList[0], count, 3);
        CHECK_EQUAL(ContainmentType::Contains, ret);

        pointList.clear();
        b1 = BoundingBox(glm::vec3(-2.0f,-3.0f,-2.0f), glm::vec3(-0.5f,-0.5f,-0.2f));
        radius = glm::length(b1.getExtent()) / 2.0f;
        count = 20;
        for(size_t i=0; i<count; i++)
        {
            glm::vec3 dummy = b1.getCenter() + glm::ballRand(radius);
            pointList.push_back(dummy.x);
            pointList.push_back(dummy.y);
            pointList.push_back(dummy.z);
        }
        ret = b0.contains(&pointList[0], count, 0);
        CHECK_EQUAL(ContainmentType::Intersects, ret);
        
        pointList.clear();
        b1 = BoundingBox(glm::vec3( 7.0f, 3.1f, 3.4f), glm::vec3( 9.9f, 4.4f, 4.7f));
        radius = glm::length(b1.getExtent()) / 2.0f;
        count = 25;
        for(size_t i=0; i<count; i++)
        {
            glm::vec3 dummy = b1.getCenter() + glm::ballRand(radius);
            pointList.push_back(dummy.x);
            pointList.push_back(dummy.y);
            pointList.push_back(dummy.z);
            pointList.push_back(0.0f);
        }
        ret = b0.contains(&pointList[0], count, 1);
        CHECK_EQUAL(ContainmentType::Disjoints, ret);
    }
    
    TEST(ContainsSphere)
    {
        BoundingSphere sphere(glm::vec3(1.0f, 1.0f, 1.0f), 2.0f);
        BoundingBox box;
        ContainmentType::Value res;
        
        box = BoundingBox(glm::vec3(0.5f), glm::vec3(1.0f));
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Disjoints, res);
        
        box = BoundingBox(glm::vec3(0.5f, 0.5f,-2.0f), glm::vec3(1.5f, 1.5f, 3.0f));
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, res);
        
        box = BoundingBox(glm::vec3(-1.75f), glm::vec3( 0.25f));
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, res);

        box = BoundingBox(glm::vec3(1.8f), glm::vec3(4.25f));
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Intersects, res);

        box = BoundingBox(glm::vec3(-10.25f), glm::vec3( 10.75f));
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Contains, res);
        
        box = BoundingBox(glm::vec3(0.0f, 0.0f,-120.0f), glm::vec3(1.0f, 1.0f, -80.0f));
        res = box.contains(sphere);
        CHECK_EQUAL(ContainmentType::Disjoints, res);
    }
    
    TEST(IntersectsRay)
    {
        BoundingBox box(glm::vec3(-3.1f, 1.2f, -2.6f), glm::vec3(4.5f, 6.7f, 5.1f));
        Ray3 ray;
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
