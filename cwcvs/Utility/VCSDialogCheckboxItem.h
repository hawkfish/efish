/*
	File:		VCSDialogCheckboxItem.h

	Contains:	Dialog item for checkboxes.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>      2/9/98    rmgw    first checked in.
*/


#pragma once

#include "VCSDialogControlItem.h"

class VCSDialogCheckboxItem : public VCSDialogControlItem

	{
		
	public:
	
						VCSDialogCheckboxItem	(VCSDialog&			inDialog,
												 DialogItemIndex	inIndex);
		virtual			~VCSDialogCheckboxItem	(void);
		
		//	Events
		virtual	Boolean OnClick					(Point				localWhere,
												 EventRecord&		theEvent,
												 DialogItemIndex&	itemHit);
	};

