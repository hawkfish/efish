#include "StFile.h"

#include <Files.h>

// ---------------------------------------------------------------------------
//		¥ StFile
// ---------------------------------------------------------------------------

StFile::StFile (

	short	inRefNum)
	
	: mRefNum (inRefNum)
	
	{ // begin StFile
		
	} // end StFile

// ---------------------------------------------------------------------------
//		¥ ~StFile
// ---------------------------------------------------------------------------

StFile::~StFile (void)
	
	{ // begin ~StFile
		
		if (kInvalidFile != mRefNum) ::FSClose (mRefNum);
		mRefNum = kInvalidFile;
		
	} // end ~StFile
