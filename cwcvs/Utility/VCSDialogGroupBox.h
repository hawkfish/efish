/*
	File:		VCSDialogGroupBox.h

	Contains:	Interface to a simple group box.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>     11/4/98    rmgw    Break out group box item.
*/


#pragma once

#include "VCSDialogItem.h"

class VCSDialogGroupBox : public VCSDialogItem

	{
	
		VCSDialogItem*			mTitle;
		
	public:
	
								VCSDialogGroupBox	(VCSDialog&				inDialog,
													 DialogItemIndex		inIndex,
													 VCSDialogItem*			inTitle = 0);
		virtual					~VCSDialogGroupBox	(void);
		
		//	Events
		virtual	void	 		OnDraw			(void);
	};
	
