// ===========================================================================
//	VCSDelete.c ©1997 Electric Fish, Inc. All rights reserved.
// ===========================================================================

#include "VCSDelete.h"

#include "VCSError.h"
#include "VCSPrefs.h"
#include "VCSPrompt.h"
#include "VCSTask.h"
#include "VCSUtil.h"
#include "VCSVersion.h"

#include "CVSCommand.h"

#include <Folders.h>

//	=== Constants ===

// ---------------------------------------------------------------------------
//		Ä VCSDelete
// ---------------------------------------------------------------------------

VCSDelete::VCSDelete (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true)
		
	{ // begin VCSDelete
		
	} // end VCSDelete

// ---------------------------------------------------------------------------
//		Ä ~VCSDelete
// ---------------------------------------------------------------------------

VCSDelete::~VCSDelete (void)
	
	{ // begin ~VCSDelete
		
	} // end ~VCSDelete

// ---------------------------------------------------------------------------
//		Ä VCSDeleteFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSDelete::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin VCSDeleteFile
		
		OSErr		e = noErr;
		
		FSSpec 		cwd = inItem.fsItem;
		FSSpec		tempSpec = inItem.fsItem;
		FSSpec		tempDirSpec = inItem.fsItem;
		
		//	Stuff to clean up
		AEDesc 		command = {typeNull, nil};
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kDeleteTask, inItem.fsItem.name, nil, nil, nil);
		
		// confirm the obsoletion
		if (kPromptNo == VCSPromptYesNo (mContext, kPromptStringsID, kDeletePrompt, inItem.fsItem.name, nil, nil, nil)) {
			inItem.eItemStatus = cwItemStatusCancelled;
			goto CleanUp;
			} // if
			
		//	Get the cwd for remove
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	We must delete it first. But since the IDE expects the file to still exist,
		//	we unlock it and move it to the temp items folder.  This looks like a delete to
		//	cvs.
		if (noErr != VCSRaiseOSErr (mContext, FindFolder (tempSpec.vRefNum, kTemporaryFolderType, kCreateFolder, &tempSpec.vRefNum, &tempSpec.parID))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (tempSpec.vRefNum, tempSpec.parID, nil, &tempDirSpec))) goto CleanUp;
		FSpRstFLock (&inItem.fsItem);
		if (noErr != VCSRaiseOSErr (mContext, FSpCatMove (&inItem.fsItem, &tempDirSpec))) goto CleanUp;

		//	cvs remove <file>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "remove"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) goto CleanUp;
		if (noErr != VCSSendCommand (mContext, &command, &cwd)) goto CleanUp;
		
		//	Now, move it backä
		if (noErr != VCSRaiseOSErr (mContext, FSpCatMove (&tempSpec, &cwd))) goto CleanUp;
		
		//	Report results
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);

		FSpDelete (&inItem.fsItem);

	CleanUp:
		
		AEDisposeDesc (&command);

		return inItem.eItemStatus;
		
	} // end VCSDeleteFile
