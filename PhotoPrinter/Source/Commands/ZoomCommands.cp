/*
	File:		ZoomCommands.cp

	Contains:	Implementation of the Remove Cropping command.

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		24 Aug 2000		dml		Created
*/

#include "ZoomInCommand.h"

#include "PhotoPrintDoc.h"

const SInt16 kMaxScreenResolution = 720;
const SInt16 kMinScreenResolution = 72;

/*
ZoomInCommand
*/
ZoomInCommand::ZoomInCommand(const CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // ZoomInCommand

/*
~ZoomInCommand
*/
ZoomInCommand::~ZoomInCommand()
{
} // ~ZoomInCommand

/*
ExecuteCommand
*/
void		
ZoomInCommand::ExecuteCommand(void* inCommandData)
{
#pragma unused(inCommandData)
	mDoc->SetResolution(mDoc->GetResolution() * 2);

} // ExecuteCommand
									 
/*
FindCommandStatus {OVERRIDE}
*/
void		
ZoomInCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
// !!! We will want to make this command work on multiple selection

	*ioStatus->enabled = (mDoc->GetResolution() < kMaxScreenResolution);
} // FindCommandStatus






/*
ZoomOutCommand
*/
ZoomOutCommand::ZoomOutCommand(const CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // ZoomOutCommand

/*
~ZoomOutCommand
*/
ZoomOutCommand::~ZoomOutCommand()
{
} // ~ZoomOutCommand

/*
ExecuteCommand
*/
void		
ZoomOutCommand::ExecuteCommand(void* inCommandData)
{
#pragma unused(inCommandData)
	mDoc->SetResolution(mDoc->GetResolution() / 2);

} // ExecuteCommand
									 
/*
FindCommandStatus {OVERRIDE}
*/
void		
ZoomOutCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
// !!! We will want to make this command work on multiple selection

	*ioStatus->enabled = (mDoc->GetResolution() > kMinScreenResolution);
} // FindCommandStatus
