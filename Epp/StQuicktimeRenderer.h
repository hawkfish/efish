#pragma once

#include <UQuicktime.h>
#include "MRect.h"
#include "HORef.h"
#include "StQTImportComponent.h"
#include "UGWorld.h"

class StQuicktimeRenderer : public StOffscreenGWorld {
	protected:
		const MRect&	mDest;
		MatrixRecord*	mMat;
		RgnHandle		mClip;
		
		void	Render();
		
	public:
				 	StQuicktimeRenderer(const Rect&		inBounds,
										SInt16			inPixelDepth = 0,
										GWorldFlags		inFlags = useTempMem,
										MatrixRecord*	pMat = 0,
										RgnHandle		inClip = nil);

		virtual 	~StQuicktimeRenderer();
	};//end StQuicktimeRenderer