/*
	File:		ZoomCommands.cp

	Contains:	Implementation of the Zoom In and Zoom Out commands.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):



		15 Aug 2001		rmgw	Fix CalcFitResolution's GetRevealedRect view.
		01 Aug 2001		rmgw	Use Refresh_No when zooming.  Bug #264.
		26 Jul 2001		drd		232 233 234 Only zoom up to 800% and down to 12.5%
		28 Jun 2001		rmgw	Zoom on center point.  Bug #102.
		07 dec 2000		dml		onscreen, show entire page including unprintable area
		05 Oct 2000		drd		Use std:: with min, max
		06 aug 2000		dml		reduced kMinScreenResolution to zllow overview
		29 Aug 2000		drd		Fixed include; tweaks
		24 Aug 2000		dml		Created
*/

#include "ZoomCommands.h"

#include "PhotoPrintDoc.h"
#include "PhotoPrinter.h"

const SInt16 kMaxScreenResolution = 72 * 8;			// Max at 800%
const SInt16 kMinScreenResolution = 72 / 8;			// Min at 12.5%

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
	double hCenter;
	double vCenter;
	mDoc->GetDisplayCenter (hCenter, vCenter);
	
	mDoc->SetResolution(std::min(kMaxScreenResolution, (SInt16)(mDoc->GetResolution() * 2)));
	
	mDoc->SetDisplayCenter (hCenter, vCenter, Refresh_No);
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
//
// ZoomOutCommand
//

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

	double hCenter;
	double vCenter;
	mDoc->GetDisplayCenter (hCenter, vCenter);

	mDoc->SetResolution(mDoc->GetResolution() / 2);

	mDoc->SetDisplayCenter (hCenter, vCenter, Refresh_No);
} // ExecuteCommand
									 
/*
FindCommandStatus {OVERRIDE}
*/
void		
ZoomOutCommand::FindCommandStatus		(SCommandStatus*	ioStatus)
{
	*ioStatus->enabled = (mDoc->GetResolution() > kMinScreenResolution);
} // FindCommandStatus

#pragma mark -
//
// FitInWindowCommand
//
FitInWindowCommand::FitInWindowCommand(const CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // ZoomInCommand

/*
FitInWindowCommand
*/
FitInWindowCommand::~FitInWindowCommand()
{
} // ~ZoomInCommand


SInt16 
FitInWindowCommand::CalcFitResolution() {
	MRect	paperBounds;
	PhotoPrinter::CalculatePaperRect(mDoc->GetPrintRec(), &mDoc->GetPrintProperties(), 
										 paperBounds, mDoc->GetResolution());
	
	double docWidth (paperBounds.Width());
	double docHeight (paperBounds.Height());

	SInt16 res (mDoc->GetResolution());
	MRect revealed;
	mDoc->GetView()->GetRevealedRect(revealed);

	double scalar = std::max(docWidth / (double)revealed.Width(), docHeight / (double)revealed.Height());
	double intermediate (res);
	intermediate /= scalar;

	return (SInt16) intermediate;	
}//end CalcFitResolution 

void
FitInWindowCommand::FindCommandStatus (SCommandStatus* ioStatus)
{
	*(ioStatus->enabled) = true;
	*(ioStatus->usesMark) = true;
	*(ioStatus->mark) = mDoc->GetResolution() == CalcFitResolution() ? checkMark : 0;
}//end FindCommandStatus

void
FitInWindowCommand::ExecuteCommand (void* inCommandData)
{
#pragma unused(inCommandData)
	mDoc->SetResolution(CalcFitResolution());
	
	//	Fits, so make sure we are at the top left.
	mDoc->GetView()->GetSuperView()->ScrollImageTo (0, 0, Refresh_Yes);
}//end ExecuteCommand	

#pragma mark -
//
// ViewFullSizeCommand
//
ViewFullSizeCommand::ViewFullSizeCommand(const CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // ZoomInCommand

/*
ViewFullSizeCommand
*/
ViewFullSizeCommand::~ViewFullSizeCommand()
{
} // ~ZoomInCommand

void
ViewFullSizeCommand::FindCommandStatus (SCommandStatus* ioStatus)
{
	*(ioStatus->enabled) = true;
	*(ioStatus->usesMark) = true;
	*(ioStatus->mark) = mDoc->GetResolution() == 72 ? checkMark : 0;
}//end FindCommandStatus

void
ViewFullSizeCommand::ExecuteCommand (void* inCommandData)
{
#pragma unused(inCommandData)
	double hCenter;
	double vCenter;
	mDoc->GetDisplayCenter (hCenter, vCenter);

	mDoc->SetResolution(72);

	mDoc->SetDisplayCenter (hCenter, vCenter, Refresh_No);
}//end ExecuteCommand	
