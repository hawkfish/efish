/*
	File:		StQuicktimeRenderer.cp

	Contains:	Draws offscreen, then rotates (intended to be used as stack-based class).

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	10 sep 2001		dml		make a tile based Render
	20 Aug 2001		drd		Fixed possible leak in Render
	11 sep 2000		dml		change matrix handling for right-side of recursion in Render
	01 Aug 2000		drd		Back to FDecompressImage, and Render now uses recursion for
							wide images (workaround for what must be an Apple bug)
	31 jul 2000		dml		use QTImporter, not FDecompressImage
	28 jul 2000		dml		tweaks on FDecompressImage (fix print bug?)
	28 Jul 2000		drd		Slight optimization in destructor
	25 Jul 2000		drd		Use white background (since StOffscreenGWorld otherwise uses Appearance)
*/

#include "StQuicktimeRenderer.h"
#include "MRect.h"
#include "EGWorld.h"
#include "StPixelState.h"
#include "MNewRegion.h"

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
	, mDepth (inPixelDepth)
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
		try {
			// Note that pixels were locked in StOffscreenGWorld constructor. Also,
			// QuickTime doesn't colorize, so we don't need to deal with colors.
			this->RenderTiles();
			::UnlockPixels(::GetGWorldPixMap(mMacGWorld));
			::DisposeGWorld(mMacGWorld);
			mMacGWorld = nil;	// Prevents inherited destructor from doing anything
		} catch (...) {
			// Swallow the exception -- this will allow the inherited destructor to run
		}
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
	if ((mBounds.right - mBounds.left) > 2048) {
		// There seems to be a bug (probably in StdPix) with images that are too wide
		SInt16					center = (mBounds.left + mBounds.right) / 2;
		Rect					bounds = mBounds;
		bounds.right = center;
		{
			::SetGWorld(mSavePort, mSaveDevice);
			StQuicktimeRenderer		left(bounds, mDepth, useTempMem, mMat, mClip);
			::CopyBits((BitMap *) *::GetPortPixMap(mMacGWorld),
						(BitMap *) *::GetPortPixMap(left.mMacGWorld),
						&bounds, &bounds, srcCopy, nil);
		}
		bounds = mBounds;
		bounds.left = center;
		{
			// Adjust the matrix (which is the only way to get the eventual FDecompressImage
			// to draw in the right place)
			MatrixRecord	mat;
			::SetIdentityMatrix(&mat);
			::TranslateMatrix(&mat, ::FixRatio(bounds.left - mBounds.left, 1), 0);
			::ConcatMatrix(mMat, &mat);

			::SetGWorld(mSavePort, mSaveDevice);
			StQuicktimeRenderer		right(bounds, mDepth, useTempMem, &mat, mClip);
			::CopyBits((BitMap *) *::GetPortPixMap(mMacGWorld),
						(BitMap *) *::GetPortPixMap(right.mMacGWorld),
						&bounds, &bounds, srcCopy, nil);
		}
	} else {
		ImageDescriptionHandle	sourceDesc;
		PixMapHandle			sourcePixels = ::GetGWorldPixMap(mMacGWorld);
		OSErr					err = ::MakeImageDescriptionForPixMap(sourcePixels, &sourceDesc);
		ThrowIfOSErr_(err);

		err = ::FDecompressImage(::GetPixBaseAddr(sourcePixels),
							sourceDesc,
							::GetPortPixMap(mSavePort),
							nil,					// Decompress entire source
							mMat,
							srcCopy,
							mClip,					// mask
							nil,					// matte
							nil,					// matteRect
							codecMaxQuality,		// accuracy
							bestFidelityCodec,		// codec
							0,						// dataSize not needed with no dataProc
							nil,					// dataProc
							nil);					// progressProc
		::DisposeHandle((Handle)sourceDesc);
		ThrowIfOSErr_(err);
	}
}//end Render

SInt16 maxTileWidth (2048);
SInt16 maxTileHeight (2048);

void
StQuicktimeRenderer::RenderTiles() {
	MRect bounds (mBounds);
	SInt16 numRows (bounds.Height() / maxTileHeight);
	if (numRows * maxTileHeight < bounds.Height())
		++numRows;
	SInt16 numCols (bounds.Width() / maxTileWidth);
	if (numCols * maxTileWidth < bounds.Width())
		++numCols;

	if (bounds.Height() > maxTileHeight) 
		bounds.SetHeight(maxTileHeight);
	if (bounds.Width() > maxTileWidth)
		bounds.SetWidth(maxTileWidth);

	StColorPenState ().Normalize();
	
	// loop for row
	for (int row = 0; row < numRows; ++row) {
		// loop for col
		for (int col = 0; col < numCols; ++col) {

			// figure out swath
			MRect swath (bounds);
			swath.Offset(col * maxTileWidth, row * maxTileHeight);
			swath *= mBounds; //clip to our bounds in case partial row or tile at end			

			MRect normalizedSwath (swath); // for src clipping, topleft is at 0,0
			normalizedSwath.Offset(-mBounds.left, -mBounds.top);

			// render slave
			ImageDescriptionHandle	sourceDesc;
			PixMapHandle			sourcePixels = ::GetGWorldPixMap(this->GetMacGWorld());
			OSErr					err = ::MakeImageDescriptionForPixMap(sourcePixels, &sourceDesc);
			ThrowIfOSErr_(err);

			err = ::FDecompressImage(::GetPixBaseAddr(sourcePixels),
								sourceDesc,
								::GetPortPixMap(mSavePort),
								&normalizedSwath,		// Decompress just this swath
								mMat,
								srcCopy,
								mClip,					// mask
								nil,					// matte
								nil,					// matteRect
								codecMaxQuality,		// accuracy
								bestFidelityCodec,		// codec
								0,						// dataSize not needed with no dataProc
								nil,					// dataProc
								nil);					// progressProc
			::DisposeHandle((Handle)sourceDesc);
			ThrowIfOSErr_(err);
			}//end for tiles
		}//end for rows
	}//end RenderTiles
	