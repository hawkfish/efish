/*
	File:		VCSVersion.h

	Contains:	Interface to the Version command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    10/14/98    rmgw    first checked in.
*/


#pragma once

#include "VCSFileCommand.h"

class VCSVersion : public VCSFileCommand

	{
		
	protected:
	
		virtual	void				IterateFile		 		(const CInfoPBRec&		 	cpbPtr,
															 long						dirID,
															 Boolean&					quitFlag);
	public:
	
		static	OSErr				ParseEntriesFile 		(const	FSSpec*				inSpec,
															 Handle*					outDate,
															 Handle*					outVersion);
															 
									VCSVersion				(VCSContext&				inContext);
		virtual						~VCSVersion				(void);

		virtual	CWVCSItemStatus		GetCheckoutState 		(const	FSSpec*				inSpec);
		virtual	CWVCSItemStatus		ProcessRegularFile 		(CWVCSItem&					inItem);
	};
