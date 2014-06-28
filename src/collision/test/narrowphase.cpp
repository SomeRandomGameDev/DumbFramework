#include <unittest++/UnitTest++.h>
#include <vector>
#include <iostream>
#include <glm/gtc/random.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include "boundingobjects.hpp"
#include "polygon2d.hpp"
#include "collision/narrowphase.hpp"

using namespace Framework;

SUITE(NarrowPhase)
{
    TEST(Circles)
    {
        // This test may looks stupid but it's here to validate our GJK implementation.
        bool ret;
        Simplex2d simplex;
        glm::vec2 direction;
        ContainmentType::Value groundTruth;
        BoundingCircle c0;
        BoundingCircle c1;
        
        c0 = BoundingCircle(glm::vec2( 1.2f, 2.2f), 0.7f);
        c1 = BoundingCircle(glm::vec2(-3.1f, 4.2f), 1.1f);
        groundTruth = c0.contains(c1);
        ret = NarrowPhase::intersects(c0, c1, simplex, direction);
        CHECK_EQUAL(false, ret);
        CHECK((groundTruth == ContainmentType::Disjoints) && (ret == false));
        
        c0 = BoundingCircle(glm::vec2( 1.2f, 2.2f), 0.7f);
        c1 = BoundingCircle(glm::vec2( 0.6f, 1.7f), 0.5f);
        groundTruth = c0.contains(c1);
        ret = NarrowPhase::intersects(c0, c1, simplex, direction);
        CHECK((groundTruth == ContainmentType::Intersects) && (ret == true));
        
        c0 = BoundingCircle(glm::vec2( 0.1f, 1.3f), 2.7f);
        c1 = BoundingCircle(glm::vec2(-0.9f, 1.5f), 0.7f);
        groundTruth = c0.contains(c1);
        ret = NarrowPhase::intersects(c0, c1, simplex, direction);
        CHECK((groundTruth == ContainmentType::Contains) && (ret == true));
        
        c0 = BoundingCircle(glm::vec2( 0.2f,-0.5f), 1.7f);
        c1 = BoundingCircle(glm::vec2(-4.5f, 7.2f), 0.5f);
        groundTruth = c0.contains(c1);
        ret = NarrowPhase::intersects(c0, c1, simplex, direction);
        CHECK_EQUAL(false, ret);
        CHECK((groundTruth == ContainmentType::Disjoints) && (ret == false));
    }
    
    TEST(Quads)
    {
        // This test may looks stupid but it's here to validate our GJK implementation.
        bool ret;
        Simplex2d simplex;
        glm::vec2 direction;
        ContainmentType::Value groundTruth;
        BoundingQuad q0;
        BoundingQuad q1;
        
        q0 = BoundingQuad(glm::vec2( 0.7f, 1.1f), glm::vec2( 2.1f, 2.3f));
        q1 = BoundingQuad(glm::vec2(-1.7f, 1.3f), glm::vec2(-0.2f, 3.1f));
        groundTruth = q0.contains(q1);
        ret = NarrowPhase::intersects(q0, q1, simplex, direction);
        CHECK((groundTruth == ContainmentType::Disjoints) && (ret == false));
        
        q0 = BoundingQuad(glm::vec2( 0.7f, 1.1f), glm::vec2( 2.1f, 2.3f));
        q1 = BoundingQuad(glm::vec2( 1.7f, 1.3f), glm::vec2( 2.9f, 3.1f));
        groundTruth = q0.contains(q1);
        ret = NarrowPhase::intersects(q0, q1, simplex, direction);
        CHECK((groundTruth == ContainmentType::Intersects) && (ret == true));
        
        q0 = BoundingQuad(glm::vec2( 1.1f, 1.3f), glm::vec2( 5.1f, 4.7f));
        q1 = BoundingQuad(glm::vec2( 2.7f, 2.4f), glm::vec2( 3.9f, 3.5f));
        groundTruth = q0.contains(q1);
        ret = NarrowPhase::intersects(q0, q1, simplex, direction);
        CHECK((groundTruth == ContainmentType::Contains) && (ret == true));
    }
    
    void createPolygon(size_t count, float radius, const glm::vec2& displacement, Polygon2d& poly)
    {
        std::vector<float> points;
        float angle = 0.0f;
        float delta = 2.0f * M_PI / (float)count;
        points.resize(count*2);
        for(size_t i=0; i<count; i++, angle+=delta)
        {
            points[2*i + 0] = (radius * cos(angle)) + displacement.x;
            points[2*i + 1] = (radius * sin(angle)) + displacement.y;
        }
        poly.set(&points[0], count, 0);
    }
    
    TEST(Polygon2d)
    {
        bool ret;
        Simplex2d simplex;
        glm::vec2 direction;
        
        Polygon2d p0;
        Polygon2d p1;

        // nonagon
        createPolygon( 9, 3.7f, glm::vec2( 3.5f, 1.4f), p0);
        // dodecagon 
        createPolygon(12, 3.5f, glm::vec2(-0.3f,-0.7f), p1);

        ret = NarrowPhase::intersects(p0, p1, simplex, direction);
        CHECK_EQUAL(true, ret);
        
        // move p1 away from p0
        glm::mat3 m = glm::translate(glm::mat3(), glm::vec2(-11.5f, 21.2f));
        p1.transform(m);
        ret = NarrowPhase::intersects(p0, p1, simplex, direction);
        CHECK_EQUAL(false, ret);
    }
    
    TEST(Hybrid)
    {
        bool ret;
        Simplex2d simplex;
        glm::vec2 direction;
        
        // heptagon
        Polygon2d poly;
        createPolygon( 7, 2.6f, glm::vec2(-3.7f, 2.4f), poly);
        // quad
        BoundingQuad quad(glm::vec2(-2.7f, 0.3f), glm::vec2(-1.5f, 1.1f));
        // circle
        BoundingCircle c0(glm::vec2(0.5f,  3.1f), 1.8f);
        BoundingCircle c1(glm::vec2(9.1f,-17.9f), 4.2f);
        
        ret = NarrowPhase::intersects(quad, poly, simplex, direction);
        CHECK_EQUAL(true, ret);
        
        ret = NarrowPhase::intersects(poly, c0, simplex, direction);
        CHECK_EQUAL(true, ret);

        ret = NarrowPhase::intersects(poly, c1, simplex, direction);
        CHECK_EQUAL(false, ret);
        
        ret = NarrowPhase::intersects(c0, quad, simplex, direction);
        CHECK_EQUAL(false, ret);
    }
}
