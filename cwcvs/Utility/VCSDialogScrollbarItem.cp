/*
	File:		VCSDialogScrollbarItem.cp

	Contains:	Implementation of a Dialog box scroll bar.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>     3/22/99    rmgw    first checked in.
*/


#include "VCSDialogScrollbarItem.h"

#include <SetUpA4.h>

// ---------------------------------------------------------------------------
//		¥ ActionProc
// ---------------------------------------------------------------------------

pascal void 
VCSDialogScrollbarItem::ActionProc (

	ControlHandle	theControl,
	short 			part)

	{ // begin ActionProc

		EnterCallback();

		VCSDialogScrollbarItem*	that = (VCSDialogScrollbarItem*) (**theControl).contrlRfCon;
		if (that) that->OnAction (part);
		
		ExitCallback();

	} // end ActionProc

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ VCSDialogScrollbarItem
// ---------------------------------------------------------------------------

VCSDialogScrollbarItem::VCSDialogScrollbarItem (

	VCSDialog&			inDialog,
	DialogItemIndex		inIndex,
	short				inPageLines)
	
	: VCSDialogControlItem (inDialog, inIndex)
	, mActionUPP (NewControlActionProc (ActionProc))
	, mPageLines (inPageLines)
	
	{ // begin VCSDialogScrollbarItem
		
		PrepareCallback();

		ControlHandle	theControl = GetControl ();
		(**theControl).contrlRfCon = (long ) this;
		
	} // end VCSDialogScrollbarItem
	
// ---------------------------------------------------------------------------
//		¥ ~VCSDialogScrollbarItem
// ---------------------------------------------------------------------------

VCSDialogScrollbarItem::~VCSDialogScrollbarItem (void)

	{ // begin ~VCSDialogScrollbarItem
		
	} // end ~VCSDialogScrollbarItem

// ---------------------------------------------------------------------------
//		¥ SetScrollTarget
// ---------------------------------------------------------------------------

void 
VCSDialogScrollbarItem::SetScrollTarget (

	VCSScrollTarget*	inTarget)
	
	{ // begin SetScrollTarget
	
		mScrollTarget = inTarget;
		
		AlignToTarget ();
		
	} // end SetScrollTarget
	
// ---------------------------------------------------------------------------
//		¥ AlignToTarget
// ---------------------------------------------------------------------------

void 
VCSDialogScrollbarItem::AlignToTarget (void)

	{ // begin AlignToTarget
	
		if (!mScrollTarget) return;
		
		SetMinValue (mScrollTarget->GetScrollMin ());
		SetMaxValue (mScrollTarget->GetScrollMax ());
		SetValue (mScrollTarget->GetScrollValue ());
		
	} // end AlignToTarget

// ---------------------------------------------------------------------------
//		¥ ScrollBy
// ---------------------------------------------------------------------------

void 
VCSDialogScrollbarItem::ScrollBy (

	short 			inDelta)
	
	{ // begin ScrollBy
		
		if (mScrollTarget) mScrollTarget->ScrollBy (inDelta);
		
	} // end ScrollBy
	
// ---------------------------------------------------------------------------
//		¥ ScrollTo
// ---------------------------------------------------------------------------

void 
VCSDialogScrollbarItem::ScrollTo (

	short 			inLine)
	
	{ // begin ScrollTo
	
		if (mScrollTarget) mScrollTarget->ScrollTo (inLine);

	} // end ScrollTo
	
// ---------------------------------------------------------------------------
//		¥ OnAction
// ---------------------------------------------------------------------------

void 
VCSDialogScrollbarItem::OnAction (

	short 			part)

	{ // begin OnAction

		DialogPtr 		theDialog = GetDialogPtr ();
		ControlHandle	theControl = GetControl ();
		
		short			ctlMax = GetControlMaximum (theControl);
		short			ctlMin = GetControlMinimum (theControl);
		short			ctlValue = GetControlValue (theControl);
		short			lines = 0;
		
		switch (part) {
			case kControlUpButtonPart:
				lines = -1;
				break;

			case kControlDownButtonPart:
				lines = 1;
				break;

			case kControlPageUpPart:
				lines = -mPageLines;
				break;

			case kControlPageDownPart:
				lines = mPageLines;
				break;

			default:
				return;
			} // switch

		if ((ctlValue + lines) > ctlMax)
			lines = ctlMax - ctlValue;

		if ((ctlValue + lines) < ctlMin)
			lines = ctlMin - ctlValue;
			
		if (lines != 0) {
			SetControlValue (theControl, ctlValue + lines);
			ScrollBy (lines);
			} // if
			
	} // end OnAction

// ---------------------------------------------------------------------------
//		¥ OnClick
// ---------------------------------------------------------------------------

Boolean 
VCSDialogScrollbarItem::OnClick (
	
	Point			localWhere,
	EventRecord&	/*theEvent*/,
	short&			itemHit)
	
	{ // begin OnClick
		
		ControlHandle	theControl;
		short			part = ::FindControl (localWhere, GetDialogPtr (), &theControl);
		switch (part) {
			case kControlUpButtonPart:
			case kControlDownButtonPart:
			case kControlPageUpPart:
			case kControlPageDownPart:
				TrackControl (theControl, localWhere, mActionUPP);
				break;
				
			case kControlIndicatorPart:
				TrackControl (theControl, localWhere, nil);
				ScrollTo (GetValue ());
				break;
			} // switch
		
		itemHit = GetIndex ();
		
		return true;
			
	} // end OnClick
