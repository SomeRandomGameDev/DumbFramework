#ifndef _DUMB_FW_CONFIG_MSVC_
#define _DUMB_FW_CONFIG_MSVC_

#pragma warning(disable : 4996)

#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#define snprintf _snprintf

#endif /* _DUMB_FW_CONFIG_MSVC_ */