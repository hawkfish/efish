/*
	File:		VCSRename.h

	Contains:	Interface to the Rename command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSFileCommand.h"

class VCSRename : public VCSFileCommand

	{
		
	public:
	
									VCSRename				(VCSContext&				inContext);
		virtual						~VCSRename				(void);

		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
	};
