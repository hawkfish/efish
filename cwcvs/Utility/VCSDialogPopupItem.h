#pragma once

#include "VCSDialogControlItem.h"

class VCSDialogPopupItem : public VCSDialogControlItem

	{
		
	public:
	
						VCSDialogPopupItem		(VCSDialog&			inDialog,
												 DialogItemIndex	inIndex);
		virtual			~VCSDialogPopupItem		(void);
		
		//	Access
		MenuHandle		GetMenu (void) const;
		
		//	Operations
		virtual	void	GetDescriptor			(Str255				itemText) const;
		virtual	void	SetDescriptor			(ConstStr255Param	itemText);

	};

