/*
	File:		VCSGet.h

	Contains:	Interface to the Get command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSFileCommand.h"

class VCSGet : public VCSFileCommand

	{
		
	public:
	
									VCSGet					(VCSContext&				inContext);
		virtual						~VCSGet					(void);

		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
		virtual	CWVCSItemStatus		ProcessRegularFolder 	(CWVCSItem&					inItem);
	};
