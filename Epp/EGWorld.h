/*
	File:		EGWorld.h

	Contains:	an adapter to LGWorld with greater control over local/temp memory

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
	
		// though the constructor sets this initially, one can change it
		void		SetPurgable(bool inState);
		// really returns whatever ::LockPixels returns, which is supposed to mean . . . 
		bool		IsPurged(void);

//----- LGWorld Adapter

	GWorldPtr	GetMacGWorld() const	{ return mG->GetMacGWorld(); }

	Boolean		BeginDrawing() {return mG->BeginDrawing();};

	void		EndDrawing() {return mG->EndDrawing();};

	virtual void	CopyImage(
						GrafPtr			inDestPort,
						const Rect&		inDestRect,
						SInt16			inXferMode = srcCopy,
						RgnHandle		inMaskRegion = nil) const {mG->CopyImage(inDestPort, inDestRect, inXferMode, inMaskRegion);};

	GWorldFlags	Update(	const Rect&		inBounds,
						SInt16			inPixelDepth = 0,
						GWorldFlags		inFlags = clipPix,
						CTabHandle		inCTableH = nil,
						GDHandle		inGDeviceH = nil) {return mG->Update(inBounds, inPixelDepth, inFlags, inCTableH, inGDeviceH);};

	void		SetBounds( const Rect& inBounds ); // actual override

	void		GetBounds( Rect& outBounds ) const {mG->GetBounds(outBounds);};

	};//end class EGWorld