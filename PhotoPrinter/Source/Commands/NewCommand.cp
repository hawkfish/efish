/*
	File:		NewCommand.cp

	Contains:	Implementation of the New command (used by the buttons).

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 Jun 2000		drd		Created
*/

#include "NewCommand.h"

#include "MAppleEvent.h"

/*
NewCommand
*/
NewCommand::NewCommand(
	const CommandT	inCommand, 
	PhotoPrintApp*	inApp)
	: PhotoAppCommandAttachment(inCommand, inApp)
{
} // NewCommand

/*
~NewCommand
*/
NewCommand::~NewCommand()
{
} // ~NewCommand

/*
ExecuteCommandNumber {OVERRIDE}
*/
void		
NewCommand::ExecuteCommandNumber(
	CommandT			inCommand,
	void*				/*inCommandData*/)
{
	// Create a "new document" event
	MAppleEvent 		aevt(kAECoreSuite, kAECreateElement);
	DescType			docType = cDocument;
	aevt.PutParamPtr(typeType, &docType, sizeof(DescType), keyAEObjectClass);

	// What kind of template
	docType = inCommand;
	aevt.PutParamPtr(typeType, &docType, sizeof(DescType), keyAERequestedType);

	// And send it! This will result in a window being opened.
	UAppleEventsMgr::SendAppleEvent(aevt);
} // ExecuteCommandNumber										 

/*
FindCommandStatus {OVERRIDE}
	Sets its argument to TRUE if the command should be enabled.
*/
void		
NewCommand::FindCommandStatus(SCommandStatus* ioStatus)
{
	*ioStatus->enabled = true;
} // FindCommandStatus
