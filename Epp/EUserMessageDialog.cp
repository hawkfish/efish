/*
	File:		EUserMessageDialog.cp

	Contains:	Implementation of the user message display dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		16 Jul 2001		rmgw	Create user message system. 
*/

#include "EUserMessageDialog.h"

#include "MPString.h"
#include "MRect.h"

//	==	Constants === 

const	PaneIDT		pane_Next				=	'next';
const	PaneIDT		pane_Back				=	'back';
const	PaneIDT		pane_Icon				=	'icon';
const	PaneIDT		pane_Message			=	'mesg';
const	PaneIDT		pane_Details			=	'dtls';
const	PaneIDT		pane_DetailsScroller	=	'dscr';

const	MessageT	msg_Next				= 	msg_OK;
const	MessageT	msg_Back				= 	-10001;

enum {
	si_Next = 1,
	si_Done,
	si_OK,
	si_Fnord
	};
	
// ---------------------------------------------------------------------------
//	¥ EUserMessageDialog											  [public]
// ---------------------------------------------------------------------------

EUserMessageDialog::EUserMessageDialog (

	ResIDT					inDialogResID,
	LCommander*				inSuper,
	const	MessageList&	inMessages,
	short					inEventMask)
	
	: EDialog (inDialogResID, inSuper, inEventMask)
	
	, mMessages (inMessages)
	, mCurrentPosition (mMessages.end ())
	
	{ // begin EUserMessage
		
		UReanimator::LinkListenerToBroadcasters (this, GetDialog (), inDialogResID);
		
		if (mMessages.size () <= 1) HidePaneByID (msg_Next);
		
		GetCurrentMessage (mDefaults);
		
	} // end EUserMessage

// ---------------------------------------------------------------------------
//	¥ ~EUserMessageDialog											  [public]
// ---------------------------------------------------------------------------

EUserMessageDialog::~EUserMessageDialog (void)

	{ // begin ~EUserMessageDialog
	
	} // end ~EUserMessageDialog
	
#pragma mark -

// ---------------------------------------------------------------------------
//	¥ DetailsAreVisible											 	 [public]
// ---------------------------------------------------------------------------
//	Returns true if the details pane is visible

bool
EUserMessageDialog::DetailsAreVisible (void) const
	
	{ // begin DetailsAreVisible
		
		return (FindPaneByID (pane_DetailsScroller)->GetVisibleState () != triState_Off);
		
	} // end DetailsAreVisible

// ---------------------------------------------------------------------------
//	¥ ShowDetails												 	 [public]
// ---------------------------------------------------------------------------
//	If the details pane is not visible, grows the window to accomodate it and
//	shows it.

void
EUserMessageDialog::ShowDetails (void)
	
	{ // begin ShowDetails
		
		if (DetailsAreVisible ()) return;
		
		//	Where is the window?
		LWindow*	w (GetDialog ());
		MRect		wBounds;
		w->GetGlobalBounds (wBounds);
		
		//	Where is the scroller?
		LPane*		scroller = FindPaneByID (pane_DetailsScroller);
		MRect		sFrame;
		scroller->CalcLocalFrameRect (sFrame);
		
		//	Grow the dialog window
		wBounds.bottom += sFrame.Height ();
		w->ResizeWindowTo (wBounds.Width (), wBounds.Height ());
		
		//	Show the details
		scroller->Show ();
		
	} // end ShowDetails

// ---------------------------------------------------------------------------
//	¥ HideDetails												 	 [public]
// ---------------------------------------------------------------------------
//	If the details pane is visible, hide it and shrink the window to cover
//	the hole.

void
EUserMessageDialog::HideDetails (void)
	
	{ // begin HideDetails
		
		if (!DetailsAreVisible ()) return;
		
		//	Where is the window?
		LWindow*	w (GetDialog ());
		MRect		wBounds;
		w->GetGlobalBounds (wBounds);
		
		//	Where is the scroller?
		LPane*		scroller = FindPaneByID (pane_DetailsScroller);
		MRect		sFrame;
		scroller->CalcLocalFrameRect (sFrame);
		
		//	Hide the details
		scroller->Hide ();
		
		//	Shrink the dialog window
		wBounds.bottom -= sFrame.Height ();
		w->ResizeWindowTo (wBounds.Width (), wBounds.Height ());

	} // end HideDetails

// ---------------------------------------------------------------------------
//	¥ GetCurrentMessage												 [public]
// ---------------------------------------------------------------------------
//	Retrieve the current contents of the window as a message object.

void
EUserMessageDialog::GetCurrentMessage (

	EUserMessage&	outMessage) const
	
	{ // begin GetCurrentMessage
		
		typedef	EUserMessage::TextRef	TextRef;
		Str255			s;
		
		//	Icon ID
		LControl*		iconPane = dynamic_cast<LControl*> (FindPaneByID (pane_Icon));
		Assert_(iconPane);
		
		SInt16			iconID;
		iconPane->GetDataTag (0, kControlIconResourceIDTag, sizeof (iconID), &iconID);
		outMessage.SetIconID (iconID);
		
		//	Title
		TextRef			title;
		GetDialog ()->GetDescriptor (s);
		if (s[0]) title = new MNewHandle (s + 1, s[0]);
		outMessage.SetTitle (title);
		
		//	Message
		TextRef			msg;
		FindPaneByID (pane_Message)->GetDescriptor (s);
		if (s[0]) msg = new MNewHandle (s + 1, s[0]);
		outMessage.SetMessage (msg);
		
		//	Details
		TextRef			details;
		LTextEditView*	detailsPane = dynamic_cast<LTextEditView*> (FindPaneByID (pane_Details));
		Assert_(detailsPane);
		
		MHandle			detailsPaneText (detailsPane->GetTextHandle ());
		if (detailsPaneText.GetSize ()) {
			details = new MNewHandle (Size (0));
			(*details) += detailsPaneText;
			} // if
		outMessage.SetDetails (details);
			
	} // end GetCurrentMessage

// ---------------------------------------------------------------------------
//	¥ SetCurrentMessage												 [public]
// ---------------------------------------------------------------------------
//	Set the current state of the window as a message object.

void
EUserMessageDialog::SetCurrentMessage (

	const	EUserMessage&	inMessage)
	
	{ // begin SetCurrentMessage
		
		Str255			s;
		
		//	Icon ID
		LControl*		iconPane = dynamic_cast<LControl*> (FindPaneByID (pane_Icon));
		Assert_(iconPane);
		
		SInt16			iconID = inMessage.GetIconID ();
		iconPane->SetDataTag (0, kControlIconResourceIDTag, sizeof (iconID), &iconID);
		
		//	Title
		TextRef			title = inMessage.GetTitle ();
		if (title) 
			BlockMoveData (**title, s + 1, s[0] = title->GetSize ());
		else s[0] = 0;
		GetDialog ()->SetDescriptor (s);
			
		//	Message
		TextRef			msg = inMessage.GetMessage ();
		if (msg) 
			BlockMoveData (**msg, s + 1, s[0] = msg->GetSize ());
		else s[0] = 0;
		FindPaneByID (pane_Message)->SetDescriptor (s);
		
		//	Details
		LTextEditView*	detailsPane = dynamic_cast<LTextEditView*> (FindPaneByID (pane_Details));
		Assert_(detailsPane);
			
		TextRef			details = inMessage.GetDetails ();
		if (details) {
			detailsPane->SetTextHandle (*details);
			ShowDetails ();
			} // if
			
		else {
			HideDetails ();
			detailsPane->SetTextPtr (s, 0);
			} // else 
			
	} // end SetCurrentMessage

// ---------------------------------------------------------------------------
//	¥ SetCurrentPosition												 [public]
// ---------------------------------------------------------------------------
//	Change to the specified position.  If it is the end, we return true; otherwise
//	false.  Anything not specified by the current position will be replaced by
//	the defaults.

bool
EUserMessageDialog::SetCurrentPosition (

	MessagePosition	inPosition)
	
	{ // begin SetCurrentPosition
		
		if (mCurrentPosition == inPosition) return (mCurrentPosition == mMessages.end ());
		
		mCurrentPosition = inPosition;
		
		if (mCurrentPosition == mMessages.end ()) return true;
		
		EUserMessage	newContent (mDefaults);
		newContent.SetIconID (inPosition->GetIconID ());
		if (inPosition->GetTitle ()) newContent.SetTitle (inPosition->GetTitle ());
		if (inPosition->GetMessage ()) newContent.SetMessage (inPosition->GetMessage ());
		if (inPosition->GetDetails ()) newContent.SetDetails (inPosition->GetDetails ());
		
		SetCurrentMessage (newContent);
		
		//	Set the back button state
		if (mCurrentPosition != mMessages.begin ())
			EnablePaneByID (pane_Back);
		else DisablePaneByID (pane_Back);
		
		//	Set the next button title
		MessagePosition	nextPosition (mCurrentPosition + 1);
		MPString 		buttonText (GetDialog ()->GetPaneID (), 
										(mMessages.size () > 1) 
											? ((nextPosition != mMessages.end ()) 
												? si_Next 
												: si_Done)
											: si_OK);
		FindPaneByID (pane_Next)->SetDescriptor (buttonText);
		
		return false;
		
	} // end SetCurrentPosition

// ---------------------------------------------------------------------------
//	¥ OnNext												 [public]
// ---------------------------------------------------------------------------
//	Move to the next position.  Returns true if we are now done

bool
EUserMessageDialog::OnNext (void)
	
	{ // begin OnNext
		
		Assert_(mCurrentPosition != mMessages.end ());

		return SetCurrentPosition (mCurrentPosition + 1);
		
	} // end OnNext

// ---------------------------------------------------------------------------
//	¥ OnBack												 [public]
// ---------------------------------------------------------------------------
//	Move to the previous position.  Returns true if we are now done

bool
EUserMessageDialog::OnBack (void)
	
	{ // begin OnBack
		
		Assert_(mCurrentPosition != mMessages.begin ());
		
		return SetCurrentPosition (mCurrentPosition - 1);
		
	} // end OnBack

// ---------------------------------------------------------------------------
//	¥ Run												 [public]
// ---------------------------------------------------------------------------
//	Runs the dialog.

void
EUserMessageDialog::Run (void)
	
	{ // begin Run
		
		bool	done = SetCurrentPosition (mMessages.begin ());
		
		while (!done) {
			GetDialog ()->Show ();
			MessageT	msg = DoDialog ();
			switch (msg) {
				case msg_Next:
					done = OnNext ();
					break;
					
				case msg_Back:
					done = OnBack ();
					break;
				} // switch
			} // for

	} // end Run

