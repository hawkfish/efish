/*
	File:		NewCommand.cp

	Contains:	Implementation of the New command (used by the buttons).

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		03 Aug 2001		rmgw	Use 'make new document with properties {}' syntax.
		02 Aug 2001		rmgw	New AppleEvent suite.  Bug #273.
		23 Jun 2000		drd		Created
*/

#include "NewCommand.h"

#include "Layout.h"
#include "PhotoPrintDoc.h"

//	Toolbox++
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
	CommandT			/*inCommand*/,
	void*				/*inCommandData*/)
{
	// make new document
	MAppleEvent 		createEvent (kAECoreSuite, kAECreateElement);
		DescType			docType = cDocument;
		createEvent.PutParamPtr(typeType, &docType, sizeof(DescType), keyAEObjectClass);
		
		//	with properties {layoutType, layoutCount, file}
		MAERecord		props;
			StAEDescriptor		layDesc;

			// What kind of template
			Layout::LayoutType	layType = Layout::kGrid;
			layDesc << layType;
			props.PutKeyDesc(layDesc, PhotoPrintDoc::pLayoutType);
		
			// How many items per page
			UInt32				layCount = 0;
			layDesc << layCount;
			props.PutKeyDesc(layDesc, PhotoPrintDoc::pLayoutCount);

		createEvent.PutParamDesc (props, keyAEPropData);

	// And send it! This will result in a window being opened.
	UAppleEventsMgr::SendAppleEvent(createEvent);
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
