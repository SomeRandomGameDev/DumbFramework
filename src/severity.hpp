#ifndef _DUMB_FW_SEVERITY_
#define _DUMB_FW_SEVERITY_

#include "enum.hpp"

namespace Framework {

BEGIN_ENUM(Severity)
    DEFINE_ENUM(Trace)
    DEFINE_ENUM(Warning)
    DEFINE_ENUM(Error)
END_ENUM(Severity)

} // Framework

#endif /* _DUMB_FW_SEVERITY_ */