/*
	File:		UseProxyCommand.cp

	Contains:	allow toggle of proxy usages

	Written by:	Dav Lion
	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		18 Sep 2000		drd		Redraw to show change
		24 Aug 2000		dml		Created
*/

#include "UseProxyCommand.h"

#include "PhotoPrintApp.h"
#include "PhotoPrintItem.h"

UseProxyCommand::UseProxyCommand				(const CommandT		inCommand,
												 PhotoPrintApp*		inApp) 
	: PhotoAppCommandAttachment(inCommand, inApp)
{
}//end ct											 

void		
UseProxyCommand::ExecuteCommand				(void*				/*inCommandData*/)
{
	PhotoPrintItem::gUseProxies = !PhotoPrintItem::gUseProxies;

	mApp->RefreshDocuments(false, false);		// Force redraw only
}//end ExecuteCommand


void		
UseProxyCommand::FindCommandStatus			(SCommandStatus*	inStatus)
{
	*(inStatus->enabled) = true;
	*(inStatus->usesMark) = true;
	*(inStatus->mark) = PhotoPrintItem::gUseProxies ? checkMark : 0;
}//end FindCommandStatus

