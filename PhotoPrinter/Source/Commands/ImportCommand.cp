/*
	File:		ImportCommand.cp

	Contains:	Implementation of the Import menu command

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		12 Jul 2001		rmgw	Convert the import event to make new import.
		19 Sep 2000		drd		Give the dialog a title
		23 Aug 2000		drd		Created
*/

#include "ImportCommand.h"

#include "MAEDescIterator.h"
#include "MNavDialogOptions.h"
#include "MNavGetFile.h"
#include "MNavReplyRecord.h"
#include "MAppleEvent.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintEvents.h"
#include "PhotoPrintResources.h"

/*
ImportCommand
*/
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

		MNavDialogOptions		options;
		::GetIndString (options.clientName, STRx_Standards, str_ProgramName);
		::GetIndString(options.windowTitle, str_Navigation, si_Import);
		
		MNavReplyRecord			navReply;
		MNavGetFile				fileDialog;
		fileDialog.DoGetFile(navReply, &options, nil);		// Open all types
		if (navReply.validRecord) {
			MAEList				targetList;
			
			MAEDescIterator		end (&navReply.selection);
			for (MAEDescIterator i (end); ++i != end; ) 
				targetList.PutDesc (*i);

			MAppleEvent				createEvent (kAECoreSuite, kAECreateElement);
				//	keyAEObjectClass
				DescType				classKey = PhotoPrintDoc::cImportClass;
				createEvent.PutParamPtr (typeType, &classKey, sizeof (classKey), keyAEObjectClass);
				
				//	keyAEInsertHere
				StAEDescriptor	docSpec;
				mDoc->MakeSpecifier (docSpec);

				StAEDescriptor	locationDesc;
				UAEDesc::MakeInsertionLoc (docSpec, kAEEnd, locationDesc);
				createEvent.PutParamDesc (locationDesc, keyAEInsertHere);
				
				//	keyAEPropData
				createEvent.PutParamDesc (targetList, keyAEData);
			
			createEvent.Send ();
		}//endif happy
	} while (false);

} // ExecuteCommand										 
