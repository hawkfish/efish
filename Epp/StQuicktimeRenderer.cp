/*
	File:		StQuicktimeRenderer.cp

	Contains:	Draws offscreen, then rotates (intended to be used as stack-based class).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	31 jul 2000		dml		use QTImporter, not FDecompressImage
	28 jul 2000		dml		tweaks on FDecompressImage (fix print bug?)
	28 Jul 2000		drd		Slight optimization in destructor
	25 Jul 2000		drd		Use white background (since StOffscreenGWorld otherwise uses Appearance)
*/

#include "StQuicktimeRenderer.h"
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
	Copy our PixMap to the current port, using a QTImport component

*/
void
StQuicktimeRenderer::Render()
{
	OSErr e;
	long size;
	PixMapHandle			sourcePixels = ::GetGWorldPixMap(mMacGWorld);
	CodecType				codec (kJPEGCodecType);
	
	// find out how big a compressed version of the pixmap would be 
	e = GetMaxCompressionSize(sourcePixels, &(**sourcePixels).bounds, 32, codecMaxQuality, 
								codec, bestFidelityCodec, &size);
	ThrowIfOSErr_(e);

	MNewHandle desc ((long)1);
	ThrowIfNil_(desc);
	MNewHandle imageHandle ((Size)size);
	// compress the pixmap into a jpeg ptr (actually a locked handle)
	{
	StHandleLocker lock (imageHandle);
	Ptr imageData (*imageHandle);

	e = CompressImage(sourcePixels, &(**sourcePixels).bounds, codecMaxQuality, 
						codec, (ImageDescriptionHandle)(Handle)desc, (Ptr)imageData);
	ThrowIfOSErr_(e);
	}//end locked handle section
	
	// open up a QTImport component for the handle containing the jpeg data
	StQTImportComponent qti (imageHandle, 'JPEG');

	
	// necessary setup for the qti
	e = ::GraphicsImportSetMatrix(qti, mMat);
	ThrowIfOSErr_(e);
	e = ::GraphicsImportSetClip(qti, mClip);
	ThrowIfOSErr_(e);
	e = ::GraphicsImportSetQuality (qti, codecMaxQuality);
	ThrowIfOSErr_(e);
	// better safe, explicitly say where to draw
	e =::GraphicsImportSetGWorld(qti, mSavePort, mSaveDevice);
	ThrowIfOSErr_(e);

	e = ::GraphicsImportDraw(qti);
	ThrowIfOSErr_(e);	
}//end Render

