// ===========================================================================
//	SessionCarbonDebugPrefix.h		©2000 Electric Fish, Inc. All rights reserved.
// ===========================================================================

	// Bring in the project's precompiled header for the given target

#define PP_DEBUG				1

#if __option(profile)
	#include "CarbonDebugProfileHeadersPPC++"
#else
	#include "CarbonDebugHeadersPPC++"		
#endif
