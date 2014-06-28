#include <unittest++/UnitTest++.h>
#include <vector>
#include <iostream>
#include <glm/gtc/random.hpp>
#include <glm/gtx/io.hpp>
#include "polygon2d.hpp"

using namespace Framework;

SUITE(Polygon2d)
{
    TEST(Quad)
    {
        static const float points[] = 
        {
            -4.5f,-1.3f, 0.0f,
            -4.5f, 1.3f, 0.0f,
             4.5f, 1.3f, 0.0f,
             4.5f,-1.3f, 0.0f
        };
        const float tolerance = 0.00001f;
        size_t count = 4;
        Polygon2d poly(points, count, 1);
        CHECK_EQUAL(count, poly.getCount());
        for(size_t i=0; i<poly.getCount(); i++)
        {
            CHECK_CLOSE(points[3*i + 0], poly.getPoint(i).x, tolerance);
            CHECK_CLOSE(points[3*i + 1], poly.getPoint(i).y, tolerance);
        }
        glm::vec2 center = poly.getCenter();
        CHECK_CLOSE(0.0f, center.x, tolerance);
        CHECK_CLOSE(0.0f, center.y, tolerance);
    }

    TEST(Dodecagon)
    {
        const float tolerance = 0.00001f;
        size_t count = 12;
        float *points = new float[count*2];
        float radius = 2.1f;
        float angle  = 0.0f;
        float delta  = 2.0f*M_PI / (float)count;
        glm::vec2 displacement(6.4f, -3.1f);
        for(size_t i=0; i<count; i++)
        {
            points[2*i + 0] = (radius * cos(angle)) + displacement.x;
            points[2*i + 1] = (radius * sin(angle)) + displacement.y;
            angle += delta;
        }
        
        Polygon2d poly(points, count, 0);

        CHECK_EQUAL(count, poly.getCount());
        for(size_t i=0; i<poly.getCount(); i++)
        {
            CHECK_CLOSE(points[2*i + 0], poly.getPoint(i).x, tolerance);
            CHECK_CLOSE(points[2*i + 1], poly.getPoint(i).y, tolerance);
        }
        glm::vec2 center = poly.getCenter();
        CHECK_CLOSE(displacement.x, center.x, tolerance);
        CHECK_CLOSE(displacement.y, center.y, tolerance);
        
        delete [] points;
    }
}
