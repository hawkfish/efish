/*
	File:		VCSDatabaseConnect.h

	Contains:	Interface to the Connect command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSCommand.h"

class VCSDatabaseConnect : public VCSCommand

	{
	
	public:
		
									VCSDatabaseConnect		(VCSContext&	inContext);
		virtual						~VCSDatabaseConnect		(void);
			
		virtual	CWVCSCommandStatus	DoRequest 				(void);
	};
		