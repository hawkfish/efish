/*
	File:		EGWorld.cp

	Contains:	an adapter to LGWorld with greater control over local/temp memory

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		30 Aug 2000		drd		CopyImage locks pixels; IsPurged maintains lock state;
								corrected spelling of SetPurgeable
		25 Aug 2000		dml		Created
*/

#include "EGWorld.h"
#include "StPixelState.h"

/*
EGWorld
*/
EGWorld::EGWorld(const Rect&		inBounds,
					SInt16			inPixelDepth,
					CTabHandle		inCTableH ,
					GDHandle		inGDeviceH ,
					RGBColor*		inBackColor ,
					MemoryStrategyT	inMemoryStrategy,
					bool			inSetPurgable,
					bool			inKeepLocal,
					bool			inNoNewDevice)
{	
	GWorldFlags	flags (0);
	if (inKeepLocal == kKeepLocal)
		flags |= keepLocal;
	if (inNoNewDevice == kNoNewDevice)
		flags |= noNewDevice;
	if (inSetPurgable == kPurge)
		flags |= pixPurge;

	if (inMemoryStrategy != kTryTempMemOnly) {
		try {
			mG = new LGWorld (inBounds, inPixelDepth, flags, inCTableH, inGDeviceH, inBackColor);
					// bounds, pixdepth, flags, ctabH, gdeviceH
			return;
			} // try

		catch (LException e) {
			if (e.GetErrorCode() != memFullErr) throw;
			if (inMemoryStrategy == kNeverTryTempMem)
				throw;
			} // catch
		}//endif try real memory first

		flags |= useTempMem;
		mG = new LGWorld (inBounds, inPixelDepth, flags, inCTableH, inGDeviceH, inBackColor);
}//end ct


/*
~EGWorld
*/
EGWorld::~EGWorld()
{
}//end

/*
CopyImage
	Copies an image from the offscreen GWorld to the specified Port

	NOTE: You will want to make sure that the foreground color is black
	and the background color is white before calling this function.
	CopyBits() can be unreliable if this is not the case. We don't do
	this automatically here because we want this routine to be fast
	(that's why you usually use offscreen drawing).

	But PowerPlant's LGWorld::CopyImage doesn't lock our pixels, which seems like a bug
*/
void
EGWorld::CopyImage(
	GrafPtr			inDestPort,
	const Rect&		inDestRect,
	SInt16			inXferMode,
	RgnHandle		inMaskRegion) const
{
	StLockPixels	locker(::GetGWorldPixMap(this->GetMacGWorld()));
	mG->CopyImage(inDestPort, inDestRect, inXferMode, inMaskRegion);
} // CopyImage

/*
IsPurged
*/
bool		
EGWorld::IsPurged(void)
 {
	// Unfortunately, we can't use ::GetPixelsState, since that only tells us if the pixels
	// *can* be purged.
	Boolean result (::LockPixels(::GetGWorldPixMap(GetMacGWorld())));
	if (!result)
		::UnlockPixels(::GetGWorldPixMap(GetMacGWorld()));

	return result ? false : true;
}//end IsPurged



//FIXME
void
EGWorld::SetBounds( const Rect& inBounds )
{
	mG->SetBounds(inBounds);
}//end SetBounds

/*
SetPurgeable
*/
void		
EGWorld::SetPurgeable(bool inState)
{
	if (inState)
		::AllowPurgePixels(::GetGWorldPixMap(GetMacGWorld()));
	else
		::NoPurgePixels(::GetGWorldPixMap(GetMacGWorld()));
}// SetPurgeable




