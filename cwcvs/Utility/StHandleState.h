#pragma once

#include <MacMemory.h>

class StHandleState {

		Handle		mH;
		SignedByte	mState;
	
	public:
		
					StHandleState		(Handle		inH);
					~StHandleState		(void);
				
	};
