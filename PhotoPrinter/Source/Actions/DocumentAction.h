/*
	File:		DocumentAction.h

	Contains:	Interface to a document swapping action.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		24 jul 2001		rmgw	Move dirty tracking to PhotoPrintAction.
		20 Jul 2001		rmgw	Created.
*/

#pragma once

#include "PhotoPrintAction.h"

#include "PhotoItemProperties.h"
#include "PrintProperties.h"

class DocumentAction : public PhotoPrintAction

	{
	
			//	Illegal
								DocumentAction		(const	DocumentAction&	other);
		const DocumentAction&	operator=			(const	DocumentAction&	other);
		
	protected:
			
			//	Undo state
		SizeLimitT				mUndoMin;
		SizeLimitT				mUndoMax;
		PrintProperties			mUndoPrintProps;
		
			//	Construction/Destruction Utilities
		SizeLimitT				GetCurrentMin			(void) const;
		SizeLimitT				GetCurrentMax			(void) const;
		const PrintProperties&	GetCurrentPrintProps	(void) const;

	public:
		
			//	Construction/Destruction		
								DocumentAction		(PhotoPrintDoc*	inDoc,
													 const SInt16	inStringIndex);
		virtual					~DocumentAction		(void);
			
			// LAction
		virtual	void			RedoSelf			(void);
		virtual	void			UndoSelf			(void);
	};
