#include "StAEDesc.h"

#include <MacMemory.h>

// ---------------------------------------------------------------------------
//		¥ StAEDesc
// ---------------------------------------------------------------------------

StAEDesc::StAEDesc (void)
	
	{ // begin StAEDesc
		
		descriptorType = typeNull;
		dataHandle = nil;
		
	} // end StAEDesc

// ---------------------------------------------------------------------------
//		¥ ~StAEDesc
// ---------------------------------------------------------------------------

StAEDesc::~StAEDesc (void)
	
	{ // begin ~StAEDesc
		
		::AEDisposeDesc (this);
		
	} // end ~StAEDesc
