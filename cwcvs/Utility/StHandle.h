#pragma once

#include <MacTypes.h>
#include <MacMemory.h>

class StHandle {

		Handle			mH;
		
					StHandle		(const	StHandle&	other);
		StHandle&	operator=		(const	StHandle&	other);
		
	public:
	
					StHandle		(Handle				inH = nil);
					~StHandle		(void);
		
		operator	Handle			(void) const {return mH;}
		Size		GetSize			(void) const {return ::GetHandleSize (mH);}
		void		Lock			(void) {::HLock (mH);}
	};
	
