/*
	File:		VCSDialogPrep.h

	Contains:	Stack object for VCS dialog activation.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>     10/8/98    rmgw    Convert to v7 API.
         <1>      2/6/98    rmgw    first checked in.
*/


#pragma once

#include "VCSContext.h"

class VCSDialogPrep {
		
		const	VCSContext&		mContext;
	
	public:
		
								VCSDialogPrep	(const	VCSContext&	inContext);
								~VCSDialogPrep	(void);
	};
	
