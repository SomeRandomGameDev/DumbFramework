#include <DumbFramework/severity.hpp>

namespace Framework {

/**
 * Convert severity value to string.
 * Severity | String
 * -------- | -------
 * Info     | info
 * Warning  | warning
 * Error    | error
 * 
 * @return Severity as string. 
 */
char const* Severity::toString() const
{
    switch(value)
    {
        case Severity::Info:
            return "info";
        case Severity::Warning:
            return "warning";
        case Severity::Error:
            return "error";
        default:
            return "     ";
    }
}

} // Framework
