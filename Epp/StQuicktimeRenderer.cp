/*
	File:		StQuicktimeRenderer.cp

	Contains:	Draws offscreen, then rotates (intended to be used as stack-based class).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	28 Jul 2000		drd		Slight optimization in destructor
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
		// Note that pixels were locked in StOffscreenGWorld constructor. Also,
		// QuickTime doesn't colorize, so we don't need to deal with colors.
		this->Render();
		::UnlockPixels(::GetGWorldPixMap(mMacGWorld));
		::DisposeGWorld(mMacGWorld);
		mMacGWorld = nil;	// Prevents inherited destructor from doing anything
	}//end
}//end dt

/*
Render
	Copy (via FDecompressImage) our PixMap to the current port
	See <http://developer.apple.com/quicktime/icefloe/dispatch008.html>
*/
void
StQuicktimeRenderer::Render()
{
	ImageDescriptionHandle	sourceDesc;
	PixMapHandle			sourcePixels = ::GetGWorldPixMap(mMacGWorld);
	OSErr					err = ::MakeImageDescriptionForPixMap(sourcePixels, &sourceDesc);

	if (err != noErr) {
		err++;	// fool optimizer
	}

	err = ::FDecompressImage(::GetPixBaseAddr(sourcePixels),
						sourceDesc,
						::GetPortPixMap(mSavePort),
						nil,					// Decompress entire source
						mMat,
						ditherCopy,
						mClip,					// mask
						nil,					// matte
						nil,					// matteRect
						codecMaxQuality,		// accuracy
						bestFidelityCodec,		// codec
						0,						// dataSize not needed with no dataProc
						nil,					// dataProc
						nil);					// progressProc
	if (err != noErr) {
		err++;	// fool optimzer
	}

	::DisposeHandle((Handle)sourceDesc);
}//end Render
