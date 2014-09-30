#include <unittest++/UnitTest++.h>
#include <glm/gtc/random.hpp>
#include <DumbFramework/entity.hpp>
#include <sstream>

using namespace Framework;

SUITE(Entity)
{
    bool deepCheck(Entity *a, Entity *b)
    {
        if(nullptr != a->parent())
        {
            CHECK(a->parent() != b->parent());
        }
        else
        {
            CHECK(nullptr == b->parent());
        }
        CHECK(a != b);
        CHECK_EQUAL(a->name, b->name);
        CHECK_EQUAL(a->childrenCount(), b->childrenCount());
        
        bool ret = ((a != b)
                 && (a->name == b->name)
                 && (a->childrenCount() == b->childrenCount()));
        
        for(size_t i=0; ret && (i<a->childrenCount()); i++)
        {
            ret = deepCheck(a->child(i), b->child(i));
        }
        return ret;
    }
    
    TEST(deepClone)
    {
        Entity e[8];
        
        Entity* root;
        Entity* dup;
        
        for(size_t i=0; i<8; i++)
        {
            std::ostringstream out;
            out << i;
            e[i].name = out.str();
        }
        
        root = &e[0];
        
        e[0].addChild(&e[1]);
        e[0].addChild(&e[2]);
        
        e[1].addChild(&e[3]);
        
        e[3].addChild(&e[4]);
        
        e[2].addChild(&e[5]);
        e[2].addChild(&e[6]);
        
        e[6].addChild(&e[7]);
        
        dup = root->deepClone();
        
        deepCheck(root, dup);
    }
    // [todo] detach, removeChild
}
