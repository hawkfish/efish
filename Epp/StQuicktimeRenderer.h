/*
	File:		StQuicktimeRenderer.h

	Contains:	Draws offscreen, then rotates (intended to be used as stack-based class).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	10 sep 2001		dml		add RenderTiles
	01 Aug 2000		drd		Took out all sorts of extraneous includes
	01 Aug 2000		drd		Removed unused mDest, added mDepth
*/

#pragma once

#include "UGWorld.h"

class StQuicktimeRenderer : public StOffscreenGWorld {
protected:
	SInt16			mDepth;
	MatrixRecord*	mMat;
	RgnHandle		mClip;
	
	void	Render();
	void	RenderTiles();
	
public:
			 	StQuicktimeRenderer(const Rect&		inBounds,
									SInt16			inPixelDepth = 0,
									GWorldFlags		inFlags = useTempMem,
									MatrixRecord*	pMat = 0,
									RgnHandle		inClip = nil);

	virtual 	~StQuicktimeRenderer();
};//end StQuicktimeRenderer
