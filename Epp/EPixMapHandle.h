#pragma once

#include "MPixMap.h"

#include "HORef.h"
#include "MNewPtr.h"
#include "UState.h"

class EPixMapHandle : public MPixMap

	{
		
		typedef	MDisposeHandle<CTabHandle>	DisposeCTabHandle;
		typedef	HORef<DisposeCTabHandle>	CTabRef;
		
		typedef	MDisposeHandle<PixMapHandle>DisposePixMapHandle;
		typedef	HORef<DisposePixMapHandle>	PixMapRef;
		
		PixMapRef		mPixRef;		
		CTabRef			mColors;
		HORef<MPtr>		mBaseAddr;
		
						EPixMapHandle	(const	EPixMapHandle&	other);
		EPixMapHandle&	operator=		(const	EPixMapHandle&	other);
		
	public:
						
						EPixMapHandle	(const	Rect&	inBounds,
   										 short			inDepth = 8,
    									 CTabHandle		inColors = nil,
    									 SInt16			inHRes = 72,
    									 SInt16			inVRes = 72);
		virtual			~EPixMapHandle	(void);
	};
	
