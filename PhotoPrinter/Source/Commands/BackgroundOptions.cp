/*
	File:		BackgroundOptions.cp

	Contains:	Implementation of the Background Options dialog.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Jun 2000		drd		Created
*/

#include "BackgroundOptions.h"

/*
BackgroundOptionsCommand
*/
BackgroundOptionsCommand::BackgroundOptionsCommand(const CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
}//end BackgroundOptionsCommand

/*
~BackgroundOptionsCommand
*/
BackgroundOptionsCommand::~BackgroundOptionsCommand()
{
} // ~BackgroundOptionsCommand

/*
ExecuteCommand
*/
void		
BackgroundOptionsCommand::ExecuteCommand(void* inCommandData)
{
#pragma unused(inCommandData)
} // ExecuteCommand
									 
/*
FindCommandStatus {OVERRIDE}
*/
void		
BackgroundOptionsCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
	*ioStatus->enabled = true;
} // FindCommandStatus

#pragma mark -

