#include <unittest++/UnitTest++.h>
#include <vector>
#include <iostream>
#include <glm/gtc/random.hpp>
#include <glm/gtx/io.hpp>
#include "boundingobjects.hpp"
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
}
