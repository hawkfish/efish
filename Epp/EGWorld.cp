/*
	File:		EGWorld.cp

	Contains:	an adapter to LGWorld with greater control over local/temp memory

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		30 jul 2001		dml		Assert SetBounds receives non-empty Rect
		27 Jul 2001		rmgw	Fix backwards IsPurgable logic.
		31 aug 2000		dml		fill in SetBounds.
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
	if (!result) return true;
	
	::UnlockPixels(::GetGWorldPixMap(GetMacGWorld()));

	return false;
}//end IsPurged



//	LGWorld::SetBounds doesnÕt actually change the GWorld.
void
EGWorld::SetBounds( const Rect& inBounds )
{
	// GWorld will return a parameter error if empty rect
	Assert_(!::EmptyRect(&inBounds));
		
	PixMapHandle	pixmap = ::GetGWorldPixMap(GetMacGWorld());

	// Update just the bounds -- weÕre careful to keep the pixel depth and color table the same
	mG->Update(inBounds, (**pixmap).pixelSize, 0/*empty flags*/, (**pixmap).pmTable, nil/*device*/);

	// now set the bounds of the LGWorld
	mG->SetBounds((Rect&)inBounds);
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




