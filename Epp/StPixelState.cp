/*
	File:		StPixelState.cp

	Contains:	Stack-based classes which restore the state of a GWorld's pixels

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	22 Sep 2000		drd		Use bitwise AND, rather than boolean, in TryLockIfNeeded
*/

#include "StPixelState.h"

StPixelState::StPixelState(GWorldPtr inGW) 
{
	mPMH = ::GetGWorldPixMap(inGW);
	mFlags = ::GetPixelsState(mPMH);
}//end gwrld ct

StPixelState::StPixelState(PixMapHandle inPMH)
	: mPMH (inPMH)
	, mFlags (::GetPixelsState(mPMH))
{
}//end ct
	
/*
~StPixelState
	Destructor restores state
*/
StPixelState::~StPixelState()
{
	::SetPixelsState(mPMH, mFlags);
}//end dt
	
#pragma mark -

StLockPixels::StLockPixels(GWorldPtr inGW, bool inThrowIfLockFails) 
	: StPixelState(inGW)
{
	this->TryLockIfNeeded(inThrowIfLockFails);
}//end

StLockPixels::StLockPixels(PixMapHandle inPMH, bool inThrowIfLockFails) 
	: StPixelState(inPMH)
{
	this->TryLockIfNeeded(inThrowIfLockFails);
}//end ct

/*
~StLockPixels
	Inherited destructor already restores state
*/
StLockPixels::~StLockPixels()
{
}//end dt

/*
TryLockIfNeeded
*/
void
StLockPixels::TryLockIfNeeded(bool  inThrowIfFail) {
	// if we are already locked, just return
	if (mFlags & (1 << pixelsLockedBit))
		return;
		
	bool result = ::LockPixels(mPMH);
	if (inThrowIfFail && !result)
		Throw_(-1);		// !!! this is pretty lame error handling
}//end TryLockIfNeeded
