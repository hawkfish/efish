/*
	File:		StBackColor.cp

	Contains:	

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>     2/22/98    rmgw    first checked in.
*/


#include "StBackColor.h"

//	===	Constants ===

const	RGBColor	
StBackColor::sWhiteColor = {0xFFFF, 0xFFFF, 0xFFFF};

// ---------------------------------------------------------------------------
//		¥ StBackColor
// ---------------------------------------------------------------------------

StBackColor::StBackColor (

	const	RGBColor*	inColor)
	
	{ // begin StBackColor
		
		GetBackColor (&saveColor);
		if (inColor) RGBBackColor (inColor);
		
	} // end StBackColor
	
// ---------------------------------------------------------------------------
//		¥ ~StBackColor
// ---------------------------------------------------------------------------

StBackColor::~StBackColor (void)
	
	{ // begin ~StBackColor
		
		RGBBackColor (&saveColor);
		
	} // end ~StBackColor
