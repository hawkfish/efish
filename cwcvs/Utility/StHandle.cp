#include "StHandle.h"

#include <MacMemory.h>

// ---------------------------------------------------------------------------
//		€ StHandle
// ---------------------------------------------------------------------------

StHandle::StHandle (

	Handle	inH)
	
	: mH (inH)
	
	{ // begin StHandle
		
	} // end StHandle

// ---------------------------------------------------------------------------
//		€ ~StHandle
// ---------------------------------------------------------------------------

StHandle::~StHandle (void)
	
	{ // begin ~StHandle
		
		if (mH) ::DisposeHandle (mH);
		mH = nil;
		
	} // end ~StHandle
