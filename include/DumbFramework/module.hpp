#ifndef _DUMB_FW_MODULE_
#define _DUMB_FW_MODULE_

#include <DumbFramework/enum.hpp>

namespace Framework {

BEGIN_ENUM(Module)
    DEFINE_ENUM(Base)
    DEFINE_ENUM(Render)
    DEFINE_ENUM(App)
END_ENUM(Module)

} // Framework

#endif /* _DUMB_FW_MODULE_ */
