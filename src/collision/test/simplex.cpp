#include <unittest++/UnitTest++.h>
#include <vector>
#include <iostream>
#include <glm/gtc/random.hpp>
#include <glm/gtx/io.hpp>
#include "collision/simplex.hpp"

using namespace Framework;

SUITE(Simplex)
{
    TEST(Line)
    {
        bool ret;
        Simplex2d simplex;
        glm::vec2 direction;
        
        simplex.add(glm::vec2(-1.5f, 1.2f));
        simplex.add(glm::vec2( 1.2f, 2.0f));
        direction = glm::vec2( 0.0f, 1.0f);
        
        ret = simplex.update(direction);
        CHECK_EQUAL(false, ret);
       
        simplex.clear();
        simplex.add(glm::vec2( 0.5f, 1.2f));
        simplex.add(glm::vec2( 0.2f,-1.1f));
        ret = simplex.update(direction);
        CHECK_EQUAL(false, ret);
    }
    
    TEST(Triangle)
    {
        bool ret;
        Simplex2d simplex;
        glm::vec2 direction;
        
        simplex.add(glm::vec2(-1.5f, 1.2f));
        simplex.add(glm::vec2( 1.2f, 2.0f));
        simplex.add(glm::vec2( 0.2f,-1.1f));
        direction = glm::vec2( 0.0f, 1.0f);
        
        ret = simplex.update(direction);
        CHECK_EQUAL(true, ret);

        simplex.clear();
        simplex.add(glm::vec2( 1.0f, 2.2f));
        simplex.add(glm::vec2( 0.2f, 1.4f));
        simplex.add(glm::vec2( 1.4f, 1.1f));
        direction = glm::vec2( 1.0f, 0.0f);
        
        ret = simplex.update(direction);
        CHECK_EQUAL(false, ret);
        CHECK_EQUAL(2, simplex.count());
    }
}
