/*
	File:		E2ClickAttachment.h

	Contains:	Definition of PowerPlant attachment to handle double-clicks.

	Written by:	David Dunham (with thanks to Richard Wesley)

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Depends on:	PowerPlant

	Change History (most recent first):

		15 Sep 2000		drd		Created
*/

#pragma once

#include <LAttachment.h>
#include <LBroadcaster.h>

class E2ClickAttachment : public LAttachment, public LBroadcaster {
public:
	enum {
		class_ID = FOUR_CHAR_CODE('E2ca')
	};

					E2ClickAttachment(
							LPane*	inPane,
							const MessageT	in2ClickMessage = msg_Nothing,
							const CommandT	in2ClickCommand = cmd_Nothing);
					E2ClickAttachment(LStream*	inStream);
	virtual			~E2ClickAttachment()	{}

protected:
	// LAttachment
	virtual void	ExecuteSelf(
							MessageT		inMessage,
							void*			ioParam);

	// Instance data
	LPane*		mPane;				// The pane we're attached to
	MessageT	m2ClickMessage;		// We broadcast this message
	CommandT	m2ClickCommand;		// We send this command
};
