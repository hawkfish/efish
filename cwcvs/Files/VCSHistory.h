/*
	File:		VCSHistory.h

	Contains:	Interface to the History command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>    10/22/98    rmgw    One dialog for regular.
         <2>    10/21/98    rmgw    Add Advanced Properties dialogs.
         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSFileCommand.h"

#include <AEDataModel.h>

class VCSHistory : public VCSFileCommand

	{
		
	public:
	
									VCSHistory				(VCSContext&				inContext);
		virtual						~VCSHistory				(void);

		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
		virtual	CWVCSItemStatus		ProcessRegularFolder 	(CWVCSItem&					inItem);
	};
