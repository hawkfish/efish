// OpenCommand.cp
// Copyright © 2000 Electric Fish, Inc

#include "OpenCommand.h"
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



void		
OpenCommand::FindCommandStatus		(SCommandStatus*	inStatus)
{
	*(inStatus->enabled) = true;
}//end FindCommandStatus



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
		MAEList				targetList;
		fileDialog.DoGetFile(navReply, 0, openTypes);
		if (navReply.validRecord) {
			long count;
			OSErr e (::AECountItems (&(navReply.selection), &count));
			if (e != noErr) continue;
			for (long i = 1; i <= count; ++i) {
				AEDesc 	resultDesc;
				e = ::AEGetNthDesc (&(navReply.selection), i, typeFSS, NULL, &resultDesc);
				if (e != noErr) continue;
				targetList.PutDesc (resultDesc);
				}//end for all items selected
			}//endif happy

		MAppleEvent openEvent (kCoreEventClass, kAEOpen);
		openEvent.PutParamDesc (targetList);
		MAppleEvent reply (openEvent, kAEWaitReply);
	} while (false);
	
}//end ExecuteCommandNumber									 

