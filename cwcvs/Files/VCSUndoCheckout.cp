#include "VCSUndoCheckout.h"

#include "VCSError.h"
#include "VCSGet.h"
#include "VCSPrompt.h"
#include "VCSTask.h"
#include "VCSUtil.h"

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

		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kUndoCheckOutTask, inItem.fsItem.name);
		
		//	Make sure they want to...
		if (kPromptNo == VCSPromptYesNo (mContext, kPromptStringsID, kDiscardChangesPrompt, inItem.fsItem.name, NULL, NULL, NULL)) {
			inItem.eItemStatus = cwItemStatusCancelled;
			goto CleanUp;
			} // if
		
		//	Delete the old copy
		FSpRstFLock (&inItem.fsItem);
		if (noErr != VCSRaiseOSErr (mContext, FSpDelete (&inItem.fsItem))) goto CleanUp;
		
		//	Get the new one
		inItem.eItemStatus = VCSGet (mContext).ProcessRegularFile (inItem);

	CleanUp:
	
		return inItem.eItemStatus;

	} // end ProcessRegularFile
