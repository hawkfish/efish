/*
	File:		ModelAction.h

	Contains:	Interface to a model swapping action.

	Written by:	Richard Wesley and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		01 Aug 2001		rmgw	Rename ImageCount property to ItemsPerPage.  Bug #265.
		24 jul 2001		rmgw	Move dirty tracking to PhotoPrintAction.
		18 Jul 2001		rmgw	Created.
*/

#pragma once

#include "PhotoPrintAction.h"

#include "Layout.h"

#include "PhotoPrintModel.h"
#include "HORef.h"

class ModelAction : public PhotoPrintAction

	{
	
			//	Illegal
								ModelAction			(const	ModelAction&	other);
		const ModelAction&		operator=			(const	ModelAction&	other);
		
	protected:
		
		typedef	HORef<PhotoPrintModel>			ModelRef;
		typedef	PhotoItemList::difference_type	ModelPosition;
		typedef	std::vector<ModelPosition>		ModelSelection;
		typedef	Layout::LayoutType				LayoutType;
		typedef	UInt32							ImageCount;
							
		LayoutType				mUndoLayoutType;
		ImageCount				mUndoItemsPerPage;
		ModelRef				mUndoModel;
		ModelSelection			mUndoSelection;
		
			//	Construction/Destruction Utilities
		ImageCount				GetCurrentItemsPerPage	(void) const;
		LayoutType				GetCurrentLayoutType	(void) const;
		ModelRef				MakeCurrentModel		(void) const;
		ModelSelection			MakeCurrentSelection	(void) const;

	public:
		
			//	Construction/Destruction		
		explicit				ModelAction			(PhotoPrintDoc*	inDoc,
													 const SInt16	inStringIndex);
		virtual					~ModelAction		(void);
			
			// LAction
		virtual	void			RedoSelf			(void);
		virtual	void			UndoSelf			(void);
	};
