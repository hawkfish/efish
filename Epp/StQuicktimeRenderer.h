/*
	File:		StQuicktimeRenderer.h

	Contains:	Draws offscreen, then rotates (intended to be used as stack-based class).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	01 Aug 2000		drd		Removed unused mDest, added mDepth
*/

#pragma once

#include <UQuicktime.h>
#include "MRect.h"
#include "HORef.h"
#include "StQTImportComponent.h"
#include "UGWorld.h"

class StQuicktimeRenderer : public StOffscreenGWorld {
	protected:
		SInt16			mDepth;
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
