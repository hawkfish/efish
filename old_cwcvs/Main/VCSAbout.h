/*
	File:		VCSAbout.h

	Contains:	Interface to the About command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSCommand.h"

class VCSAbout : public VCSCommand

	{
	
	public:
		
									VCSAbout		(VCSContext&	inContext);
		virtual						~VCSAbout		(void);
			
		virtual	CWVCSCommandStatus	DoRequest 		(void);
	};
		