/*
	File:		StQuicktimeRenderer.cp

	Contains:	Draws offscreen, then rotates (intended to be used as stack-based class).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	25 Jul 2000		drd		Use white background (since StOffscreenGWorld otherwise uses Appearance)
*/

#include "StQuicktimeRenderer.h"

static	RGBColor	gWhite	= { 65535, 65535, 65535 };

/*
StQuicktimeRenderer
*/
StQuicktimeRenderer::StQuicktimeRenderer(const Rect&	inBounds,
										SInt16			inPixelDepth,
										GWorldFlags		inFlags,
										MatrixRecord*	pMat,
										RgnHandle		inClip)
	: StOffscreenGWorld (inBounds, inPixelDepth, inFlags, nil, nil, &gWhite)
	, mDest (inBounds)
	, mMat (pMat)
	, mClip (inClip)
{
}//end ct

/*
~StQuicktimeRenderer
*/
StQuicktimeRenderer::~StQuicktimeRenderer() 
{
	// Restore current GWorld to the one which was current
	// when the Constructor was called
	::SetGWorld(mSavePort, mSaveDevice);

	// Copy image from the offscreen GWorld to the current GWorld,
	// then destroy the offscreen GWorld
	if (mMacGWorld != nil) {
		StColorState	saveColors;
		StColorState::Normalize();
		
		this->Render();
		::UnlockPixels(::GetGWorldPixMap(mMacGWorld));
		::DisposeGWorld(mMacGWorld);
		mMacGWorld = nil;	// Prevents inherited destructor from doing anything
	}//end
}//end dt

/*
Render
*/
void
StQuicktimeRenderer::Render()
{
	ImageDescriptionHandle	imageDesc;
	OSErr	err = ::MakeImageDescriptionForPixMap(::GetGWorldPixMap(mMacGWorld),
									 &imageDesc);

	if (err != noErr) {
		err++;	// fool optimzer
	}

	err = ::FDecompressImage(::GetPixBaseAddr(::GetGWorldPixMap(GetMacGWorld())),
						imageDesc,
						::GetGWorldPixMap(mSavePort),
						nil,
						mMat,
						ditherCopy,
						mClip,					// mask
						nil,					// matte
						nil,					// matteRect
						codecMaxQuality,		// accuracy
						0,						// codec
						(**imageDesc).dataSize,
						nil,					// dataProc
						nil);					// progressProc
	if (err != noErr) {
		err++;	// fool optimzer
	}

	::DisposeHandle((Handle)imageDesc);
}//end Render
