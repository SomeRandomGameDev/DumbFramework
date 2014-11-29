#ifndef _DUMB_FW_INPUT_DELEGATE_
#define _DUMB_FW_INPUT_DELEGATE_

#include <vector>
#include <string>
#include <functional>

#include <glm/glm.hpp>

#include <DumbFramework/config.hpp>

namespace Framework {
namespace Input     {

/**
 * Input event delegate base class.
 * Return type is always void.
 * To add a new listener just do:
 * @code
 * Delegate<void(int, int)> dummy;
 * dummy += [](int a, int b) { std::cout << a << ' ' << b << std::endl; };
 * 
 * // Consider we have a class Foo with a method void bar(int, int)
 * Foo bleep;
 * dummy += std::bind(&Foo::bar, bleep, std::placeholders::_1, std::placeholders::_2);
 * 
 * // Call delegate.
 * dummy(0xcafe, 0xbeef);
 * 
 * @endcode
 */
template <typename... Args>
class Delegate
{
    using Func = std::function<void(Args...)>;
    public:
        /**
         * Add listener to delegate.
         * @param [in] rhs Listener.
         */
        Delegate& operator+= (Func const& rhs)
        {   _listeners.push_back(rhs);
            return *this;
        }
        /**
         * Call listeners.
         */
        void operator() (Args... rhs)
        {   for(auto call : _listeners)
            {
                call(rhs...);
            }
        }
        /**
         * Remove all listeners.
         */
        void clear()
        {   _listeners.clear();
        }
    private:
        /** Listeners. **/
        std::vector<Func> _listeners;
};

} // Input
} // Framework

#endif /* _DUMB_FW_INPUT_DELEGATE_ */

