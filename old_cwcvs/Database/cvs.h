#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#define AUTH_CLIENT_SUPPORT
#define xmalloc NewPtr
#define error(x,y,z) return 0
#include <string.h>

#include <MacMemory.h>

#if defined(AUTH_CLIENT_SUPPORT) || defined(AUTH_SERVER_SUPPORT)
char *scramble (char *str);
char *descramble (char *str);
#endif /* AUTH_CLIENT_SUPPORT || AUTH_SERVER_SUPPORT */

#ifdef __cplusplus
}
#endif

