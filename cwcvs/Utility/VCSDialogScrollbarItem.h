/*
	File:		VCSDialogScrollbarItem.h

	Contains:	Interface to a Dialog box scrollbar item.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>     3/22/99    rmgw    first checked in.
*/


#pragma once

#include "VCSDialogControlItem.h"

class VCSScrollTarget {

	public:
		
		//	Input
		virtual	void			ScrollBy				(short				inDelta) = 0;
		virtual	void			ScrollTo				(short				inLine) = 0;

		//	Output
		virtual	SInt16			GetScrollMin			(void) const = 0;
		virtual	SInt16			GetScrollMax			(void) const = 0;
		virtual	SInt16			GetScrollValue			(void) const = 0;
	};
	
class VCSDialogScrollbarItem : public VCSDialogControlItem

	{
		
		static	pascal void 	ActionProc 				(ControlHandle		theControl,
														 short 				part);
		ControlActionUPP		mActionUPP;
		
		VCSScrollTarget*		mScrollTarget;
		short					mPageLines;
		
	protected:
	
		virtual	void			ScrollBy				(short				inDelta);
		virtual	void			ScrollTo				(short				inLine);
		
		virtual	void			OnAction				(short				inPart);
		
	public:
	
								VCSDialogScrollbarItem	(VCSDialog&			inDialog,
														 DialogItemIndex	inIndex,
														 short				inPageLines = 16);
		virtual					~VCSDialogScrollbarItem	(void);
		
		//	Access
		virtual	void			AlignToTarget			(void);
		virtual	void			SetScrollTarget			(VCSScrollTarget*	inTarget = 0);
		
		//	Events
		virtual	Boolean 		OnClick					(Point				localWhere,
														 EventRecord&		theEvent,
														 DialogItemIndex&	itemHit);
	};

