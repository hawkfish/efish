#include "StQuicktimeRenderer.h"

StQuicktimeRenderer::StQuicktimeRenderer(const Rect&		inBounds,
										SInt16			inPixelDepth,
										GWorldFlags		inFlags,
										MatrixRecord*	pMat,
										RgnHandle		inClip)
	: StOffscreenGWorld (inBounds, inPixelDepth, inFlags)
	, mDest (inBounds)
	, mMat (pMat)
	, mClip (inClip)
{
}//end ct


StQuicktimeRenderer::~StQuicktimeRenderer() 
{
	// Restore current GWorld to the one which was current
	// when the Constructor was called
	::SetGWorld(mSavePort, mSaveDevice);

	// Copy image from the offscreen GWorld to the current GWorld,
	// then destroy the offscreen GWorld
	if (mMacGWorld != nil) {
		StColorState saveColors;
		StColorState::Normalize();
		
		Render();
		::UnlockPixels(::GetGWorldPixMap(mMacGWorld));
		::DisposeGWorld(mMacGWorld);
		mMacGWorld = nil;
		}//end


}//end dt


void
StQuicktimeRenderer::Render() {
	ImageDescriptionHandle	imageDesc;
	::MakeImageDescriptionForPixMap(::GetGWorldPixMap(mMacGWorld),
									 &imageDesc);

	::FDecompressImage(GetPixBaseAddr(::GetGWorldPixMap(GetMacGWorld())),
						imageDesc,
						::GetGWorldPixMap(mSavePort),
						nil,
						mMat,
						ditherCopy,
						mClip,
						nil,
						nil,
						codecMaxQuality,
						0,
						(**imageDesc).dataSize,
						nil,
						nil);

	::DisposeHandle((Handle)imageDesc);

}//end Render





