#pragma once

#include <Files.h>

class StFile {

		short		mRefNum;
	
	public:
		
		enum {kInvalidFile = -1};
		
					StFile		(short	inRefNum = kInvalidFile);
					~StFile		(void);
				
		operator	short		(void) const {return mRefNum;}
		
		OSErr		Write		(long&			count,
								 const	void*	buffer);
	};

//	=== Inlines ===

inline OSErr
StFile::Write (long& count, const void* buffer)
	{return ::FSWrite (mRefNum, &count, buffer);}