/*
	File:		VCSUndoCheckout.h

	Contains:	Interface to the Undo Checkout command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>    10/14/98    rmgw    Add recursion.
         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSFileCommand.h"

class VCSUndoCheckout : public VCSFileCommand

	{
		
	public:
	
									VCSUndoCheckout			(VCSContext&				inContext);
		virtual						~VCSUndoCheckout		(void);

		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
	};
