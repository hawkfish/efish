/*
	File:		VCSDialogControlItem.h

	Contains:	Dialog item for controls.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>     3/22/99    rmgw    Add Min/Max access.
         <1>      2/9/98    rmgw    first checked in.
*/


#pragma once

#include "VCSDialogItem.h"

class VCSDialogControlItem : public VCSDialogItem

	{
	
	public:
	
						VCSDialogControlItem	(VCSDialog&			inDialog,
												 DialogItemIndex	inIndex);
		virtual			~VCSDialogControlItem	(void);


		ControlHandle	GetControl 				(void) const;
		
		//	Operations
		virtual	void	SetEnable				(Boolean				itemEnable = true);
										 
		virtual	short	GetMinValue				(void) const;
		virtual	void	SetMinValue				(SInt16					itemMin = 0);

		virtual	short	GetMaxValue				(void) const;
		virtual	void	SetMaxValue				(SInt16					itemMax = 0);

		virtual	short	GetValue				(void) const;
		virtual	void	SetValue				(SInt16					itemValue = 0);
		
		virtual	void	GetDescriptor			(Str255					itemText) const;
		virtual	void	SetDescriptor			(ConstStr255Param		itemText);
	};
	
