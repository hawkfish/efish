/*
	File:		OpenCommand.cp

	Contains:	Implementation of the Open menu command

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		10 Jul 2001		drd		91 Added OpenDialog, send either kAEImport or kAEOpen depending on selection
		29 Jun 2001		rmgw	Use 'open' 129 for Open command.  Bug #84.
		09 Nov 2000		drd		Don't enable if a modal dialog is up
		23 Aug 2000		drd		Don't send an Apple Event if user cancels dialog
*/

#include "OpenCommand.h"

#include "EDialog.h"
#include "MAEList.h"
#include "MAEDesc.h"
#include "MAEDescExtractors.h"
#include "MNavDialogOptions.h"
#include "MNavGetFile.h"
#include "MNavReplyRecord.h"
#include "MAppleEvent.h"
#include "NewCommand.h"
#include "PhotoPrintApp.h"
#include "PhotoPrintEvents.h"

// Globals
NavEventUPP			OpenDialog::gEventUPP = nil;
NavObjectFilterUPP	OpenDialog::gFilterUPP = nil;

const ResIDT	alrt_MixedFiles = 141;

/*
OpenCommand
*/
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
// Requires NavServices 3.0
//-------------------------------------------------------------------
void		
OpenCommand::ExecuteCommandNumber	(CommandT			/*inCommand*/,
									 void*				/*inCommandData*/)
{
	do {
		StDesktopDeactivator	deactivater;

		OpenDialog				fileDialog;
		MNavReplyRecord			navReply;
		MAEList					targetList;

		fileDialog.DoCarbonGetFile(&navReply, nil /* openTypes */);

		if (navReply.validRecord) {
			long			count;
			OSErr			e (::AECountItems (&(navReply.selection), &count));
			if (e != noErr) continue;
			FSSpec			descSpec;
			SInt32			documents = 0;
			SInt32			images = 0;
			for (long i = 1; i <= count; ++i) {
				AEDesc		resultDesc;
				e = ::AEGetNthDesc (&(navReply.selection), i, typeFSS, NULL, &resultDesc);
				if (e != noErr) continue;
				targetList.PutDesc (resultDesc);

				// Check on what sort of file we intend to open
				resultDesc >> descSpec;
				MFileSpec	theSpec(descSpec);
				CInfoPBRec	info;
				theSpec.GetCatInfo(info);
				if (info.hFileInfo.ioFlFndrInfo.fdType == 'TEXT' &&
					info.hFileInfo.ioFlFndrInfo.fdCreator == MFileSpec::sDefaultCreator)
					documents++;
				else
					images++;
			}//end for all items selected

			// We can't easily handle types of file. If Nav Services worked better, we might
			// not need to do this (it'd be better to disable the Open button)
			if (documents > 0 && images > 0) {
				UModalAlerts::StopAlert(alrt_MixedFiles);
				return;
			}

			AEEventClass	theClass;
			AEEventID		theID;
			if (documents > 0) {
				theClass = kCoreEventClass;
				theID = kAEOpen;
			} else {
				theClass = kAEPhotoPrintSuite;
				theID = kAEImport;
			}

			MAppleEvent		openEvent(theClass, theID);
			if (documents > 0) {
				openEvent.PutParamDesc (targetList);
			} else {
				// Make a new document if necessary, so we have something to import into
				if (LDocument::GetDocumentList().GetCount() == 0) {
					NewCommand	command('grid', mApp);
					command.Execute('grid', nil);
				}

				LModelObject*	theDoc = mApp->GetDefaultSubModel();
				StAEDescriptor	modelSpec;
				theDoc->MakeSpecifier(modelSpec);
				openEvent.PutParamDesc(modelSpec, keyDirectObject);
				openEvent.PutParamDesc(targetList, keyAEData);

			}
			openEvent.Send();
		}//endif happy
	} while (false);
} // ExecuteCommandNumber									 

#pragma mark -

OpenDialog::OpenDialog()
	: MNavGetFile()
{
} // OpenDialog

/*
DoCarbonGetFile
	This is sort of a streamlined version of MNavGetFileÕs method of the same name
*/
void
OpenDialog::DoCarbonGetFile (NavReplyRecord*		outReply,
							NavTypeListHandle		inTypes)
{
	if (gEventUPP == nil)
		gEventUPP = ::NewNavEventUPP(&HandleEvent);
	if (gFilterUPP == nil)
		gFilterUPP = ::NewNavObjectFilterUPP(&FileObjectFilter);

	NavDialogCreationOptions	options;
	OSStatus				s = ::NavGetDefaultDialogCreationOptions(&options);
	ThrowIfOSErr_(s);

	mCurState = kNavNormalState;

	MNavGetFile::DoCarbonGetFile(gEventUPP, nil, gFilterUPP, outReply, &options, this, inTypes);
} // DoCarbonGetFile

/*
FileObjectFilter [static]
*/
pascal	Boolean
OpenDialog::FileObjectFilter(AEDesc *inItem, void *inInfo, void *inUserData, NavFilterModes inMode)
{
#pragma unused(inUserData)
#pragma unused(inMode)		// We could instead check for kNavFilteringBrowserList or something

	NavFileOrFolderInfo*		theInfo;

	switch (inItem->descriptorType) {
		case typeFSS: {
			// Check for folder
			theInfo = static_cast<NavFileOrFolderInfo*>(inInfo);
			if (theInfo->isFolder) break;

			// Check for our XML file
			if (theInfo->fileAndFolder.fileInfo.finderInfo.fdType == 'TEXT' &&
				theInfo->fileAndFolder.fileInfo.finderInfo.fdCreator == MFileSpec::sDefaultCreator)
				break;

			// See if QuickTime can find an importer
			MAEDesc					theDesc(*inItem);
			FSSpec					theSpec;
			theDesc >> theSpec;
			ComponentResult			res;
			GraphicsImportComponent	importer;
			res = ::GetGraphicsImporterForFileWithFlags(&theSpec, &importer, kDontUseValidateToFindGraphicsImporter);
			if (importer != nil)
				break;
			return false;
		}
	} // switch
		
	return true;
} // FileObjectFilter

/*
HandleEvent
	By existing, we allow the dialog to be movable
*/
pascal	void
OpenDialog::HandleEvent(NavEventCallbackMessage inMessage, NavCBRecPtr inCallback, void* inUserData)
{
#pragma unused(inCallback)	// Not used at present

	OpenDialog*		theDialog;

	switch (inMessage) {
		case kNavCBEvent:
			/* an event has occurred (update, idle events, etc.) */
			break;
			
		case kNavCBCustomize:
			/* protocol for negotiating customization space */
			break;
			
		case kNavCBStart:
			/* the navigation dialog is starting up */
			break;
			
		case kNavCBTerminate:
			/* the navigation dialog is closing down */
			break;
			
		case kNavCBAdjustRect:
			/* the navigation dialog is being resized */
			break;
			
		case kNavCBNewLocation:
			/* user has chosen a new location in the browser */
			break;
			
		case kNavCBShowDesktop:
			/* user has navigated to the desktop */
			break;
			
		case kNavCBSelectEntry:
			// The user has selected or deselected a file or folder. The
			// NavCBRec.eventData.eventDataParms.param field is an AEDescList* identifying the currently
			// selected items.
			theDialog = static_cast<OpenDialog*>(inUserData);
//			theDialog->SelectEntry(static_cast<AEDescList*>(inCallback->eventData.eventDataParms.param));
			break;
			
		case kNavCBPopupMenuSelect:
			/* signifies that a popup menu selection was made */
			break;

		case kNavCBAccept:
			/* user has accepted the navigation dialog */
			break;
									
		case kNavCBCancel:
			/* user has cancelled the navigation dialog */
			break;
			
		case kNavCBAdjustPreview:
			/* preview button was clicked or the preview was resized */
			break;

		case kNavCBOpenSelection:
			theDialog = static_cast<OpenDialog*>(inUserData);
//			theDialog->OpenSelection();
			break;
		} // switch
} // HandleEvent

void
OpenDialog::OpenSelection()
{
	AEDesc		theList;
	this->DoCustomControl(mDialog, kNavCtlGetSelection, &theList);
} //

/*
SelectEntry
	Handle Nav Services callback kNavCBSelectEntry
	We enable or disable the Open button depending on what is selected
*/
void
OpenDialog::SelectEntry(
	AEDescList*				inSelection)
{
	if (inSelection->descriptorType != typeNull) {
		MAEList				theSelection(*inSelection);
		SInt32				count = theSelection.GetCount();
		AEKeyword			theKW;					// Will always be filled in to typeWildcard
		MAEDesc				theDesc;
		SInt32				i;
		SInt32				albums = 0;
		SInt32				folders = 0;
		SInt32				imageFiles = 0;
		for (i = 1; i <= count; i++) {
			theSelection.GetNthDesc(theDesc, theKW, i, typeFSS);
			FSSpec			rawSpec;
			theDesc >> rawSpec;
			MFileSpec		theSpec(rawSpec);
			if (theSpec.IsFolder()) {
				folders++;
			} else {
				CInfoPBRec	info;
				theSpec.GetCatInfo(info);
				if (info.hFileInfo.ioFlFndrInfo.fdType == 'TEXT')
					albums++;
				else
					imageFiles++;
			}
		}
		NavActionState		state = kNavNormalState;
		// It doesn't seem as if Navigation allows the selection of more than one folder, or of a folder
		// and a non-folder. But it should be OK to leave in this test.
		if (folders > 0) {
			if (folders > 1 || albums > 0 || imageFiles > 0) {
				state = kNavDontOpenState;
			}
		} else {
			if (albums > 0 && imageFiles > 0) {
				state = kNavDontOpenState;
			}
		}
		this->SetActionState(state);
	}
} // SelectEntry

void
OpenDialog::SetActionState(const NavActionState inState)
{
	// We get a paramErr if we set it to kNavNormalState at the beginningÉ
	if (inState != mCurState) {
		this->DoCustomControl(mDialog, kNavCtlSetActionState, (void*) inState);
		mCurState = inState;
	}
}