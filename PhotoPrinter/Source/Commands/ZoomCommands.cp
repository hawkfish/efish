/*
	File:		ZoomCommands.cp

	Contains:	Implementation of the Zoom In and Zoom Out commands.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		06 aug 2000		dml		reduced kMinScreenResolution to zllow overview
		29 Aug 2000		drd		Fixed include; tweaks
		24 Aug 2000		dml		Created
*/

#include "ZoomCommands.h"

#include "PhotoPrintDoc.h"

const SInt16 kMaxScreenResolution = 72 * 16;
const SInt16 kMinScreenResolution = 72 / 16;	

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
	// Zoom in 2x. We might want to do it more like Adobe Acrobat ???
	mDoc->SetResolution(min(kMaxScreenResolution, (SInt16)(mDoc->GetResolution() * 2)));
} // ExecuteCommand
									 
/*
FindCommandStatus {OVERRIDE}
*/
void		
ZoomInCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
	*ioStatus->enabled = (mDoc->GetResolution() < kMaxScreenResolution);
} // FindCommandStatus

#pragma mark -

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
	*ioStatus->enabled = (mDoc->GetResolution() > kMinScreenResolution);
} // FindCommandStatus
