/*
	File:		StForeColor.h

	Contains:	

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>     11/6/98    rmgw    Make default black, not white.
         <2>     9/22/98    rmgw    Convert to CWP3.1.
         <1>     2/22/98    rmgw    first checked in.
*/


#pragma once

#include <QuickDraw.h>

class StForeColor

	{
		
		RGBColor	saveColor;
		
	public:
		
		static	const	RGBColor	sBlackColor;
		
									StForeColor		(const	RGBColor*	inColor = &sBlackColor);
									~StForeColor	(void);
	};
	
	
