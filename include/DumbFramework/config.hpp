#ifndef _DUMB_FW_CONFIG_
#define _DUMB_FW_CONFIG_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined(_MSC_VER)
#include <DumbFramework/platform/msvc/config.hpp>
#else
#include <DumbFramework/platform/unix/config.hpp>
#endif

#endif /* _DUMB_FW_CONFIG_ */
