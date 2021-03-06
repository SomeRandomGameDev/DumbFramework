#include <UnitTest++/UnitTest++.h>
#include <vector>
#include <glm/gtc/random.hpp>
#include <glm/gtx/io.hpp>                                                                                     
#include <DumbFramework/geometry/boundingcircle.hpp>

using namespace Dumb::Core::Geometry;

SUITE(BoundingCircle)
{
    TEST(ContainsCircle)
    {
        ContainmentType::Value ret;
        BoundingCircle c0(glm::vec2( 4.1f, -1.6f), 2.5f);
        BoundingCircle c1(glm::vec2( 2.7f,  0.8f), 3.1f);
        
        ret = c0.contains(c1);
        CHECK_EQUAL(ContainmentType::Intersects, ret);
        
        ret = c1.contains(c0);
        CHECK_EQUAL(ContainmentType::Intersects, ret);
    
        BoundingCircle c2(glm::vec2( 5.1f, -2.0f), 0.7f);
        
        ret = c0.contains(c2);
        CHECK_EQUAL(ContainmentType::Contains, ret);

        ret = c2.contains(c0);
        CHECK_EQUAL(ContainmentType::Contains, ret);
        
        BoundingCircle c3(glm::vec2(-8.4f, 11.3f), 1.9f);
        
        ret = c3.contains(c2);
        CHECK_EQUAL(ContainmentType::Disjoints, ret);
        
        ret = c0.contains(c3);
        CHECK_EQUAL(ContainmentType::Disjoints, ret);
    }
    
    TEST(ContainsPointList)
    {
        ContainmentType::Value ret;
        std::vector<float> pointList;
        
        size_t count;
        glm::vec2 center;
        
        BoundingCircle c0(glm::vec2( 5.5f, 9.1f), 7.3f);
        center = glm::vec2( 1.4f, 10.1f);
        count = 11;
        for(size_t i=0; i<count; i++)
        {
            glm::vec2 dummy = center + glm::diskRand(2.0f);
            pointList.push_back(dummy.x);
            pointList.push_back(dummy.y);
            pointList.push_back(1.0f);
            pointList.push_back(2.0f);
            pointList.push_back(3.0f);
        }
        ret = c0.contains(&pointList[0], count, 3);
        CHECK_EQUAL(ContainmentType::Contains, ret);

        pointList.clear();
        BoundingCircle c1(glm::vec2( 0.0f,-1.1f), 8.5f);
        center = glm::vec2(-9.4f,-1.1f);
        count = 9;
        for(size_t i=0; i<count; i++)
        {
            glm::vec2 dummy = center + glm::diskRand(5.0f);
            pointList.push_back(dummy.x);
            pointList.push_back(dummy.y);
        }
        ret = c1.contains(&pointList[0], count, 0);
        CHECK_EQUAL(ContainmentType::Intersects, ret);
        
        pointList.clear();
        BoundingCircle c2(glm::vec2(-5.3f,-3.1f), 2.5f);
        center = glm::vec2( 7.4f,-8.1f);
        count = 25;
        for(size_t i=0; i<count; i++)
        {
            glm::vec2 dummy = center + glm::diskRand(4.0f);
            pointList.push_back(dummy.x);
            pointList.push_back(dummy.y);
        }
        ret = c2.contains(&pointList[0], count, 0);
        CHECK_EQUAL(ContainmentType::Disjoints, ret);
    }
    
    TEST(ContainsPoint)
    {
        ContainmentType::Value ret;
        glm::vec2 dummy;
        BoundingCircle c0(glm::vec2( 5.5f, 9.1f), 7.3f);
        
        dummy = glm::vec2( 7.6f, 3.4f);
        ret = c0.contains(dummy);
        CHECK_EQUAL(ContainmentType::Contains, ret);
                
        dummy = glm::vec2(-11.6f, 21.0f);
        ret = c0.contains(dummy);
        CHECK_EQUAL(ContainmentType::Disjoints, ret);
    }
    
    TEST(ClassifyLine)
    {
        BoundingCircle c0;
        Line2 l;
        Side ret;
        
        c0 = BoundingCircle(glm::vec2( 5.5f, 9.1f), 7.3f);
        
        l  = Line2(glm::vec2(-1.0f, 2.0f), glm::vec2( 0.5f,-2.2f));
        ret = c0.classify(l);
        CHECK_EQUAL(Side::Front, ret);

        l  = Line2(glm::vec2( 0.5f,-2.2f), glm::vec2(-1.0f, 2.0f));
        ret = c0.classify(l);
        CHECK_EQUAL(Side::Back, ret);

        l  = Line2(glm::vec2(0.5f,8.5f), glm::vec2(5.5f, 9.1f));
        ret = c0.classify(l);
        CHECK_EQUAL(Side::On, ret);
    }
    
    TEST(Intersects)
    {
        BoundingCircle c0;
        Ray2 ray;
        bool ret;
        
        c0 = BoundingCircle(glm::vec2( 5.5f, 9.1f), 7.3f);
        
        ray = Ray2(glm::vec2(4.0f, 7.6f), glm::vec2(-1.0f,-1.0f));
        ret = c0.intersects(ray);
        CHECK_EQUAL(true, ret);

        ray = Ray2(glm::vec2(-5.5f,-9.2f), glm::vec2( 1.0f, 1.0f));
        ret = c0.intersects(ray);
        CHECK_EQUAL(true, ret);

        ray = Ray2(glm::vec2(-5.5f,-9.2f), glm::vec2( 0.0f, 1.0f));
        ret = c0.intersects(ray);
        CHECK_EQUAL(false, ret);
    }
}
