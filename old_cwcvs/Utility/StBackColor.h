/*
	File:		StBackColor.h

	Contains:	

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>     9/22/98    rmgw    Convert to CWP3.1.
         <1>     2/22/98    rmgw    first checked in.
*/


#pragma once

#include <QuickDraw.h>

class StBackColor

	{
		
		RGBColor	saveColor;
		
	public:
		
		static	const	RGBColor	sWhiteColor;
		
									StBackColor		(const	RGBColor*	inColor = &sWhiteColor);
									~StBackColor	(void);
	};
	
	
