/*
	File:		EGWorld.h

	Contains:	an adapter to LGWorld with greater control over local/temp memory

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		04 nov 2001		dml		add CopyPartialImage
		30 Aug 2000		drd		CopyImage no longer inlined (so we can work around PP bug);
								corrected spelling of SetPurgeable
		29 Aug 2000		drd		Don't return anything from a void inline
		25 Aug 2000		dml		Created
*/

#pragma once

#include <UGWorld.h>
#include "HORef.h"

enum {
	kPurge = true,
	kNoPurge = false,
	kKeepLocal = true,
	kNoKeepLocal = false,
	kNoNewDevice = true, // careful, these appear backwards
	kNewDevice = false
	}; // handy in constructing

class EGWorld  {
	public:
	
		// there are three different strategies for attempted memory allocation:
		// kTryLocalMemFirst is the default.  if that fails, temp memory is tried second
		// kTryTempMemOnly skips the local memory attempt
		// kNeverTryTempMem performs only the first attempt
		typedef enum {
			kTryLocalMemFirst = 0,
			kTryTempMemOnly,
			kNeverTryTempMem,
			kFnordMemBlem } MemoryStrategyT;
			
		
	protected:
		HORef<LGWorld>	mG; // pronounced in the gangsta style as "emm-Gee!"
	
	public:
	
					EGWorld(const Rect&		inBounds,
							SInt16			inPixelDepth = 0,
							CTabHandle		inCTableH = nil,
							GDHandle		inGDeviceH = nil,
							RGBColor*		inBackColor = nil,
							MemoryStrategyT	inMemoryStrategy = kTryLocalMemFirst,
							bool			inSetPurgable = false,
							bool			inKeepLocal	= false,
							bool			inNoNewDevice = false);

		virtual		~EGWorld();
	
		// really returns whatever ::LockPixels returns, which is supposed to mean . . . 
		bool		IsPurged(void);
		// though the constructor sets this initially, one can change it
		void		SetPurgeable(bool inState);

//----- LGWorld Adapter

	GWorldPtr	GetMacGWorld() const	{ return mG->GetMacGWorld(); }

	Boolean		BeginDrawing()			{ return mG->BeginDrawing(); }

	void		EndDrawing()			{ mG->EndDrawing(); }

	virtual void	CopyImage(
						GrafPtr			inDestPort,
						const Rect&		inDestRect,
						SInt16			inXferMode = srcCopy,
						RgnHandle		inMaskRegion = nil) const;

	virtual void	CopyPartialImage(
						const Rect&		inSrcRect,
						GrafPtr			inDestPort,
						const Rect&		inDestRect,
						SInt16			inXferMode = srcCopy,
						RgnHandle		inMaskRegion = nil) const;


	GWorldFlags	Update(	const Rect&		inBounds,
						SInt16			inPixelDepth = 0,
						GWorldFlags		inFlags = clipPix,
						CTabHandle		inCTableH = nil,
						GDHandle		inGDeviceH = nil) {return mG->Update(inBounds, inPixelDepth, inFlags, inCTableH, inGDeviceH);};

	void		SetBounds( const Rect& inBounds ); // actual override

	void		GetBounds( Rect& outBounds ) const {mG->GetBounds(outBounds);};
};//end class EGWorld
