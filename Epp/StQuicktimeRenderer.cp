/*
	File:		StQuicktimeRenderer.cp

	Contains:	Draws offscreen, then rotates (intended to be used as stack-based class).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	28 jul 2000		dml		tweaks on FDecompressImage (fix print bug?)
	28 Jul 2000		drd		Slight optimization in destructor
	25 Jul 2000		drd		Use white background (since StOffscreenGWorld otherwise uses Appearance)
*/

#include "StQuicktimeRenderer.h"
#include "MNewPtr.h"
#include "MNewHandle.h"

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
	OSErr e;
	long size;
	PixMapHandle			sourcePixels = ::GetGWorldPixMap(mMacGWorld);
	
	e = GetMaxCompressionSize(sourcePixels, &(**sourcePixels).bounds, 32, codecMaxQuality, 
								'raw ', bestFidelityCodec, &size);
	ThrowIfOSErr_(e);

	MNewPtr imageData (size);
	ThrowIfNil_(imageData);
	MNewHandle desc ((long)1);
	ThrowIfNil_(desc);

	e = CompressImage(sourcePixels, &(**sourcePixels).bounds, codecMaxQuality, 
						'raw ', (ImageDescriptionHandle)(Handle)desc, (Ptr)imageData);
	ThrowIfOSErr_(e);

	e = ::FDecompressImage(imageData,
						(ImageDescriptionHandle)(Handle)desc,
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
	ThrowIfOSErr_(e);

}//end Render
