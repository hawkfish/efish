/*
	File:		EGWorld.cp

	Contains:	an adapter to LGWorld with greater control over local/temp memory

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		25 Aug 2000		dml		Created
*/


#include "EGWorld.h"

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



EGWorld::~EGWorld() {
	}//end


bool		
EGWorld::IsPurged(void)
 {
	Boolean result (::LockPixels(::GetGWorldPixMap(GetMacGWorld())));
	::UnlockPixels(::GetGWorldPixMap(GetMacGWorld()));

	return result ? false : true;
	}//end IsPurged




//FIXME
void
EGWorld::SetBounds( const Rect& inBounds ) {
	mG->SetBounds(inBounds);
}//end SetBounds


void		
EGWorld::SetPurgable(bool inState) {
	if (inState)
		::AllowPurgePixels(::GetGWorldPixMap(GetMacGWorld()));
	else
		::NoPurgePixels(::GetGWorldPixMap(GetMacGWorld()));
	
	}//end SetPurgable




