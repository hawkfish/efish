/*
	File:		StDisableBroadcaster.h

	Contains:	Interface to a broadcast disabler.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		20 Jul 2001		rmgw	Created. 
*/

#pragma once

#include <LBroadcaster.h>

class StDisableBroadcaster

	{
		//	Illegal
								StDisableBroadcaster	(const	StDisableBroadcaster&	other);
		StDisableBroadcaster&	operator=				(const	StDisableBroadcaster&	other);
	
	protected:
	
		LBroadcaster*			mTarget;
		bool					mWasBroadcasting;
		
	public:
		
			//	Construction/Destruction/Copying
		explicit				StDisableBroadcaster	(LBroadcaster*			inTarget);
		virtual					~StDisableBroadcaster	(void);
			
			//	LBroadcaster Pointer emulation
		operator				LBroadcaster*			(void) const
														{return mTarget;};
			
	};
