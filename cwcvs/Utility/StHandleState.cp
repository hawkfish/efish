#include "StHandleState.h"

// ---------------------------------------------------------------------------
//		¥ StHandleState
// ---------------------------------------------------------------------------

StHandleState::StHandleState (

	Handle	inH)
	
	: mH (inH)
	, mState (::HGetState (mH))
	
	{ // begin StHandleState
		
	} // end StHandleState

// ---------------------------------------------------------------------------
//		¥ ~StHandleState
// ---------------------------------------------------------------------------

StHandleState::~StHandleState (void)
	
	{ // begin ~StHandleState
		
		::HSetState (mH, mState);
		
	} // end ~StHandleState
