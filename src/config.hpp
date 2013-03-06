#ifndef _DUMB_FW_CONFIG_
#define _DUMB_FW_CONFIG_

#include <stdio.h>

#if defined(_MSC_VER)
#include "platform/msvc/config.hpp"
#else
	// nothing atm
#endif

/// @todo Move this somewhere else.
namespace ModuleID
{
	enum
	{
		BASE   = 0,
		RENDER
	};
} // ModuleID

#endif /* _DUMB_FW_CONFIG_ */
