/*
	File:		VCSStatus.h

	Contains:	Interface to the Status command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>    10/22/98    rmgw    Add Regular dialog.
         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSFileCommand.h"

class VCSStatus : public VCSFileCommand

	{
		
	public:
	
									VCSStatus				(VCSContext&				inContext);
		virtual						~VCSStatus				(void);

		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
	};
