/*
	File:		VCSLabel.h

	Contains:	Interface to the Label command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>    11/25/98    rmgw    Add regular command.
         <2>    10/26/98    rmgw    Implement full Label command.
         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSFileCommand.h"

#include <AEDataModel.h>

class VCSLabel : public VCSFileCommand

	{
		
	public:
	
									VCSLabel				(VCSContext&				inContext);
		virtual						~VCSLabel				(void);

		virtual	CWVCSItemStatus		ProcessRegularFolder 	(CWVCSItem&					inItem);
		virtual	CWVCSItemStatus		ProcessAdvancedFolder 	(CWVCSItem&					inItem);
	};
