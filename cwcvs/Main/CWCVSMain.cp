#include <ConditionalMacros.h>
#if TARGET_CPU_68K && TARGET_RT_MAC_CFM
	#define EnterCodeResource()
	#define ExitCodeResource()
#else
	#include <A4Stuff.h>
#endif

#include "CWCVS.h"

#include "VCSV7Context.h"

// ---------------------------------------------------------------------------
//		€ CWCVS
// ---------------------------------------------------------------------------

static short 
CWCVS (

	CWPluginContext		inContext)
	
	{ // begin CWCVS
		
		Boolean			isV1 = false;
		CWResult		canV1 = ::CWAllowV1Compatibility (inContext, true, &isV1);
		if (canV1 != cwNoErr) return canV1;
		
		VCSV7Context	pb (inContext);
		
		return CWCVSDispatch (pb);
	
	} // end CWCVS

// ---------------------------------------------------------------------------
//		€ main
// ---------------------------------------------------------------------------

pascal short 
main (

	CWPluginContext	inContext)
	
	{ // begin main
			
		/* set up global world (68K only) */
		EnterCodeResource();
		
		short	result (cwCommandStatusFailed);
		
		try {
			result = ::CWDonePluginRequest (inContext, CWCVS (inContext));
			} // try
			
		catch (...) {
			//	backstop
			} // catch
			
		/* tear down global world (68K only) */
		ExitCodeResource();
		
		/* return result code */
		return result;
	
	} // end main
