/*
	File:		VCSCheckOut.h

	Contains:	Interface to the Checkout command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>    10/26/98    rmgw    Implement Recursive Checkout.
         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSFileCommand.h"

class VCSCheckOut : public VCSFileCommand

	{
		
	public:
	
									VCSCheckOut				(VCSContext&				inContext);
		virtual						~VCSCheckOut			(void);

		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
	};
