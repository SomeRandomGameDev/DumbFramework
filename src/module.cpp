#include <DumbFramework/module.hpp>

namespace Framework {

/**
 * Convert module id to string.
 * Module   | String
 * -------- | -------
 * Base     | Base
 * Render   | Render
 * App      | App
 * 
 * @return Module id as string. 
 */
char const* Module::toString() const
{
    switch(value)
    {
        case Module::Base:
            return "Base";
        case Module::Render:
            return "Render";
        case Module::App:
            return "App";
        default:
            return "Unknown";
    }
}

} // Framework
