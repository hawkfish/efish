/*
	File:		DrawMaxBoundsCommand.cp

	Contains:	allow toggle of maxBounds drawing usages

	Written by:	Dav Lion
	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		02 Mar 2001		dml		Created
*/

#include "DrawMaxBounds.h"

#include "PhotoPrintApp.h"
#include "PhotoPrintItem.h"

DrawMaxBoundsCommand::DrawMaxBoundsCommand				(const CommandT		inCommand,
												 PhotoPrintApp*		inApp) 
	: PhotoAppCommandAttachment(inCommand, inApp)
{
}//end ct											 

void		
DrawMaxBoundsCommand::ExecuteCommand				(void*				/*inCommandData*/)
{
	PhotoPrintItem::gDrawMaxBounds = !PhotoPrintItem::gDrawMaxBounds;

	mApp->RefreshDocuments(false, false);		// Force redraw only
}//end ExecuteCommand


void		
DrawMaxBoundsCommand::FindCommandStatus			(SCommandStatus*	inStatus)
{
	*(inStatus->enabled) = true;
	*(inStatus->usesMark) = true;
	*(inStatus->mark) = PhotoPrintItem::gDrawMaxBounds ? checkMark : 0;
}//end FindCommandStatus

