/*
	File:		StForeColor.cp

	Contains:	

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>     11/6/98    rmgw    Make default black, not white.
         <1>     2/22/98    rmgw    first checked in.
*/


#include "StForeColor.h"

//	===	Constants ===

const	RGBColor	
StForeColor::sBlackColor = {0x0000, 0x0000, 0x0000};

// ---------------------------------------------------------------------------
//		¥ StForeColor
// ---------------------------------------------------------------------------

StForeColor::StForeColor (

	const	RGBColor*	inColor)
	
	{ // begin StForeColor
		
		GetForeColor (&saveColor);
		if (inColor) RGBForeColor (inColor);
		
	} // end StForeColor
	
// ---------------------------------------------------------------------------
//		¥ ~StForeColor
// ---------------------------------------------------------------------------

StForeColor::~StForeColor (void)
	
	{ // begin ~StForeColor
		
		RGBForeColor (&saveColor);
		
	} // end ~StForeColor
