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
	
	
StPixelState::~StPixelState() {
	::SetPixelsState(mPMH, mFlags);
	}//end dt
	
	

StLockPixels::StLockPixels(GWorldPtr inGW, bool inThrowIfLockFails) 
	: StPixelState(inGW)
{
	TryLockIfNeeded(inThrowIfLockFails);
	}//end

StLockPixels::StLockPixels(PixMapHandle inPMH, bool inThrowIfLockFails) 
	: StPixelState(inPMH)
{
	TryLockIfNeeded(inThrowIfLockFails);
	}//end ct
	
StLockPixels::~StLockPixels() {
	}//end dt
	
void
StLockPixels::TryLockIfNeeded(bool  inThrowIfFail) {
	// if we are already locked, just return
	if (mFlags && (1 << pixelsLockedBit))
		return;
		
	bool result (::LockPixels(mPMH));
	if (inThrowIfFail && !result)
		Throw_(-1);

	}//end TryLockIfNeeded