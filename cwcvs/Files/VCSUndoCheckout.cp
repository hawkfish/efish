#include "VCSUndoCheckout.h"

#include "VCSError.h"
#include "VCSGet.h"
#include "VCSPrompt.h"
#include "VCSTask.h"
#include "VCSUtil.h"

#include "CVSCommand.h"

#include "StAEDesc.h"

//	=== Constants ===

// ---------------------------------------------------------------------------
//		¥ VCSUndoCheckout
// ---------------------------------------------------------------------------

VCSUndoCheckout::VCSUndoCheckout (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true)
		
	{ // begin VCSUndoCheckout
		
	} // end VCSUndoCheckout

// ---------------------------------------------------------------------------
//		¥ ~VCSUndoCheckout
// ---------------------------------------------------------------------------

VCSUndoCheckout::~VCSUndoCheckout (void)
	
	{ // begin ~VCSUndoCheckout
		
	} // end ~VCSUndoCheckout

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSUndoCheckout::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFile

		//	Stuff to clean up
		StAEDesc 	command;

		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kUndoCheckOutTask, inItem.fsItem.name);
		
		//	Make sure they want to...
		if (kPromptNo == VCSPromptYesNo (mContext, kPromptStringsID, kDiscardChangesPrompt, inItem.fsItem.name, NULL, NULL, NULL)) {
			inItem.eItemStatus = cwItemStatusCancelled;
			goto CleanUp;
			} // if
		
		//	Get the cwd for update
		FSSpec		cwd = inItem.fsItem;
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	cvs -r unedit <file>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-r"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "unedit"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, VCSSendCommand (mContext, &command, &cwd))) goto CleanUp;

		//	Delete the old copy
		FSpRstFLock (&inItem.fsItem);
		if (noErr != VCSRaiseOSErr (mContext, FSpDelete (&inItem.fsItem))) goto CleanUp;
		
		//	Get the new one
		inItem.eItemStatus = VCSGet (mContext).ProcessRegularFile (inItem);

	CleanUp:
	
		return inItem.eItemStatus;

	} // end ProcessRegularFile
