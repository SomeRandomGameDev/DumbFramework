#include <unittest++/UnitTest++.h>
#include <vector>
#include <glm/gtc/random.hpp>
#include <DumbFramework/transform.hpp>

using namespace Framework;

SUITE(Transform)
{
    TEST(Init)
    {
        glm::fquat rotation = glm::angleAxis(glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3  origin = glm::vec3(0.0f, 2.0f, -1.0f);
        
        Transform t(rotation , origin);
        
        glm::vec3 position = t.position();
        CHECK_CLOSE(origin.x, position.x, 0.0001f);
        CHECK_CLOSE(origin.y, position.y, 0.0001f);
        CHECK_CLOSE(origin.z, position.z, 0.0001f);
    }
    
    TEST(Transform)
    {
        glm::fquat rotation = glm::angleAxis(glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3  origin = glm::vec3(0.0f, 2.0f, -1.0f);
        
        Transform t(rotation , origin);
        
        glm::vec3 expected = origin;
        glm::vec3 position = t.transform(glm::vec3(0.0f));
        CHECK_CLOSE(expected.x, position.x, 0.0001f);
        CHECK_CLOSE(expected.y, position.y, 0.0001f);
        CHECK_CLOSE(expected.z, position.z, 0.0001f);
                
        glm::vec3 p(-5.1f, 8.14f, 111.5f);
        expected = (rotation * p) + origin;
        position = t.transform(p);
        CHECK_CLOSE(expected.x, position.x, 0.0001f);
        CHECK_CLOSE(expected.y, position.y, 0.0001f);
        CHECK_CLOSE(expected.z, position.z, 0.0001f);
    }
}
