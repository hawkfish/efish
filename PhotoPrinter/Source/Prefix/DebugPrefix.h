// ===========================================================================
//	DebugPrefix.h		©2000 Electric Fish, Inc. All rights reserved.
// ===========================================================================

	// Bring in the project's precompiled header for the given target

#define PP_DEBUG				1

#if __POWERPC__
	#include "DebugPrefixHeadersPPC++"		
#else
	#include "DebugPrefixHeaders68K++"
#endif

