/*
	File:		VCSDialogTextItem.h

	Contains:	Dialog item for text.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>      2/9/98    rmgw    first checked in.
*/


#pragma once

#include "VCSDialogItem.h"

class VCSDialogTextItem : public VCSDialogItem

	{
	
	public:
	
						VCSDialogTextItem		(VCSDialog&			inDialog,
												 DialogItemIndex	inIndex);
		virtual			~VCSDialogTextItem		(void);


		Handle			GetText		 			(void) const;
		
		//	Operations
		virtual	short	GetValue				(void) const;
		virtual	void	SetValue				(DialogItemIndex		itemValue = 0);
		
		virtual	void	GetDescriptor			(Str255					itemText) const;
		virtual	void	SetDescriptor			(ConstStr255Param		itemText);
	};
	
