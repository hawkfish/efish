#pragma once
/* StPixelState.h
*  Copyright (c) 2000 Electric Fish, Inc
*/

#include <QDOffscreen.h>

class StPixelState {
	protected:
		PixMapHandle	mPMH;
		GWorldFlags		mFlags;
		
	public:
				StPixelState(GWorldPtr inGW);
				StPixelState(PixMapHandle inPMH);
		virtual ~StPixelState();
		
	}; // end StPixelState
	
	

class StLockPixels : public StPixelState {
	protected:
			void	TryLockIfNeeded(bool inThrowIfFail);
	public:
				StLockPixels(GWorldPtr inGW, bool inThrowIfLockFails = true);
				StLockPixels(PixMapHandle inPMH, bool inThrowIfLockFails = true);
		virtual ~StLockPixels();
	};//end StLockPixels