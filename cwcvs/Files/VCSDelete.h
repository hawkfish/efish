/*
	File:		VCSDelete.h

	Contains:	Interface to the Delete command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSFileCommand.h"

class VCSDelete : public VCSFileCommand

	{
		
	public:
	
									VCSDelete				(VCSContext&				inContext);
		virtual						~VCSDelete				(void);

		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
	};
