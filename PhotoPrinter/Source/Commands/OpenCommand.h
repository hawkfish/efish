/*
	File:		OpenCommand.h

	Contains:	Definition of the Open menu command and its Nav Services dialog

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		06 Jul 2001		drd		91 Added OpenDialog to handle filtering etc.
*/

#pragma once

#include "PhotoAppCommandAttachment.h"
#include "MNavGetFile.h"

class OpenCommand : public PhotoAppCommandAttachment {
public:
						OpenCommand(CommandT inCommand, PhotoPrintApp* inApp);
	virtual				~OpenCommand(void)		{}
	
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
	virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
												 void*				inCommandData);
};//end class OpenCommand

class OpenDialog : public MNavGetFile {
public:
						OpenDialog();
	virtual				~OpenDialog()			{}

				void	DoCarbonGetFile (NavReplyRecord* outReply, NavTypeListHandle inTypes = nil);

	static	pascal	Boolean		FileObjectFilter(AEDesc *inItem, void *inInfo, void *inUserData, NavFilterModes inMode);
	static	pascal	void		HandleEvent(NavEventCallbackMessage inMessage, NavCBRecPtr inCallback, void* inUserData);

protected:
				void	SetActionState(const NavActionState inState);
		virtual	void	OpenSelection();
		virtual	void	SelectEntry(AEDescList* inSelection);

	NavActionState				mCurState;

	static	NavEventUPP			gEventUPP;
	static	NavObjectFilterUPP	gFilterUPP;
};
