/*
	File:		OpenCommand.cp

	Contains:	Implementation of the Open menu command

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		09 Nov 2000		drd		Don't enable if a modal dialog is up
		23 Aug 2000		drd		Don't send an Apple Event if user cancels dialog
*/

#include "OpenCommand.h"

#include "EDialog.h"
#include "PhotoPrintDoc.h"
#include "MAEList.h"
#include "MAEDesc.h"
#include "MNavGetFile.h"
#include "MNavReplyRecord.h"
#include "MAppleEvent.h"

OpenCommand::OpenCommand(CommandT inCommand, PhotoPrintApp* inApp)
	:PhotoAppCommandAttachment (inCommand, inApp)
{
}//end OpenCommand

/*
FindCommandStatus {OVERRIDE}
*/
void		
OpenCommand::FindCommandStatus		(SCommandStatus*	inStatus)
{
	*(inStatus->enabled) = !EDialog::IsDialogShowing();
}// FindCommandStatus


//-------------------------------------------------------------------
// ExecuteCommandNumber  
// 
// Requires NavServices
//-------------------------------------------------------------------
void		
OpenCommand::ExecuteCommandNumber	(CommandT			/*inCommand*/,
									 void*				/*inCommandData*/)
{
	do {
		StResource			openTypesList ('open', 128);
		NavTypeListHandle	openTypes (reinterpret_cast<NavTypeListHandle> ((Handle)openTypesList));
		StDesktopDeactivator	deactivater;

		MNavGetFile				fileDialog;
		MNavReplyRecord			navReply;
		MAEList					targetList;
		fileDialog.DoGetFile(navReply, 0, openTypes);
		if (navReply.validRecord) {
			long count;
			OSErr e (::AECountItems (&(navReply.selection), &count));
			if (e != noErr) continue;
			for (long i = 1; i <= count; ++i) {
				AEDesc		resultDesc;
				e = ::AEGetNthDesc (&(navReply.selection), i, typeFSS, NULL, &resultDesc);
				if (e != noErr) continue;
				targetList.PutDesc (resultDesc);
			}//end for all items selected

			MAppleEvent		openEvent (kCoreEventClass, kAEOpen);
			openEvent.PutParamDesc (targetList);
			MAppleEvent		reply (openEvent, kAEWaitReply);
		}//endif happy
	} while (false);
} // ExecuteCommandNumber									 
