/*
	File:		ImportCommand.cp

	Contains:	Implementation of the Import menu command

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 Aug 2000		drd		Created
*/

#include "ImportCommand.h"
#include "MNavGetFile.h"
#include "MNavReplyRecord.h"
#include "MAppleEvent.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintEvents.h"

ImportCommand::ImportCommand(CommandT inCommand, PhotoPrintDoc* inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
}//end ct

/*
~ImportCommand
*/
ImportCommand::~ImportCommand(void)
{
}//end dt

/*
FindCommandStatus {OVERRIDE}
*/
void		
ImportCommand::FindCommandStatus(SCommandStatus*	ioStatus)
{
	*(ioStatus->enabled) = true;
} // FindCommandStatus

/*
ExecuteCommand {OVERRIDE}
	Put up Import dialog
*/
void		
ImportCommand::ExecuteCommand(void*				/*inCommandData*/)
{
	do {
		StDesktopDeactivator	deactivater;

		MNavGetFile			fileDialog;
		MNavReplyRecord		navReply;
		MAEList				targetList;
		fileDialog.DoGetFile(navReply, 0, nil);		// Open all types
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

			MAppleEvent		openEvent(kAEPhotoPrintSuite, kAEImport);
			StAEDescriptor	modelSpec;
			mDoc->MakeSpecifier(modelSpec);
			openEvent.PutParamDesc(modelSpec, keyDirectObject);
			openEvent.PutParamDesc(targetList, keyAEData);

			openEvent.Send();
		}//endif happy
	} while (false);
} // ExecuteCommand										 
