/*
	File:		VCSDatabaseVariables.h

	Contains:	Interface to the Variables command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSCommand.h"

class VCSDatabaseVariables : public VCSCommand

	{
	
	public:
		
									VCSDatabaseVariables	(VCSContext&	inContext);
		virtual						~VCSDatabaseVariables	(void);
			
		virtual	CWVCSCommandStatus	DoRequest 				(void);
	};
		