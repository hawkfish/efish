/*
	File:		E2ClickAttachment.cp

	Contains:	PowerPlant attachment to handle double-clicks.

	Written by:	David Dunham (with thanks to Richard Wesley)

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Depends on:	PowerPlant

	Change History (most recent first):

		15 Sep 2000		drd		Created

	Notes:
		If you create your own E2ClickAttachment, you should call its
		AddListener method. If you used Constructor and the E2ClickAttachment
		is read in from a stream, it will call AddListener on all enclosing
		LListeners (hopefully most of them will ignore its message).
*/

#include "E2ClickAttachment.h"

/*
E2ClickAttachment
*/
E2ClickAttachment::E2ClickAttachment(
	LPane*			inPane,
	const MessageT	in2ClickMessage,
	const CommandT	in2ClickCommand)
	: LAttachment(msg_Click)				// We only want clicks
	, mPane(inPane)
	, m2ClickMessage(in2ClickMessage)
	, m2ClickCommand(in2ClickCommand)
{
	mPane->AddAttachment(this);
} // E2ClickAttachment

/*
E2ClickAttachment
*/
E2ClickAttachment::E2ClickAttachment(LStream* inStream)
	: LAttachment(inStream)
{
	// Assume we are attached to a pane
	mPane = dynamic_cast<LPane*>(this->GetOwnerHost());

	// Get the Constructor data
	*inStream >> m2ClickMessage;
	*inStream >> m2ClickCommand;

	// If we're going to broadcast a message, we need to add ourselves to some
	// listener (Constructor can't put us in a RidL resource to be added automatically
	// since we're not a pane). It's hard to know who is the best person to listen,
	// so we broadcast to anyone who can hear us.
	if (m2ClickMessage != msg_Nothing) {
		LPane*	thePane = mPane;
		while (thePane != nil) {
			LListener*	listener = dynamic_cast<LListener*>(thePane);
			if (listener != nil) {
				// We found someone who can hear our message
				this->AddListener(listener);
			}
			// Keep searching up the tree
			thePane = thePane->GetSuperView();
		}
	}
} // E2ClickAttachment

/*
ExecuteSelf {OVERRIDE}
*/
void
E2ClickAttachment::ExecuteSelf(
	MessageT		inMessage,
	void*			ioParam)
{
#pragma unused(inMessage, ioParam)

	// Be sure we are actually attached to a pane, and that user 2clicked that pane
	if (mPane != nil && mPane->GetClickCount() == 2) {
		// If we have a message, broadcast it
		if (m2ClickMessage != msg_Nothing)
			this->BroadcastMessage(m2ClickMessage, nil);
		
		if ((m2ClickCommand != cmd_Nothing) && LCommander::GetTarget() != nil) {
			LCommander::SetUpdateCommandStatus(true);
			LCommander::GetTarget()->ProcessCommand(m2ClickCommand, nil);
		}
	}
} // ExecuteSelf
