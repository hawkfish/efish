#include "VCSRename.h"

#include "VCSError.h"
#include "VCSPrefs.h"
#include "VCSPrompt.h"
#include "VCSTask.h"
#include "VCSUtil.h"
#include "VCSVersion.h"

#include "CVSCommand.h"
#include "FileCopy.h"

#include <Dialogs.h>

//	=== Constants ===

const	short		kRenameDLOGid = 16013;

// ---------------------------------------------------------------------------
//		€ VCSRename
// ---------------------------------------------------------------------------

VCSRename::VCSRename (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, false, false, true)
		
	{ // begin VCSRename
		
	} // end VCSRename

// ---------------------------------------------------------------------------
//		€ ~VCSRename
// ---------------------------------------------------------------------------

VCSRename::~VCSRename (void)
	
	{ // begin ~VCSRename
		
	} // end ~VCSRename

// ---------------------------------------------------------------------------
//		€ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSRename::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFile
		
		OSErr		e = noErr;
		
		FSSpec 		cwd = inItem.fsItem;
		FSSpec		newSpec = inItem.fsItem;
		
		//	Stuff to clean up
		AEDesc 		command = {typeNull, nil};
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kRenameTask, inItem.fsItem.name);
		
		// Get the filename
		::ParamText (newSpec.name, nil,  nil, nil);
		if (ok != VCSPromptString (mContext, kRenameDLOGid, newSpec.name, sizeof (newSpec.name) - 1)) {
			inItem.eItemStatus = cwItemStatusCancelled;
			goto CleanUp;
			} // if
			
		//	Get the cwd for remove
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	$ mv old new
		FSpRstFLock (&inItem.fsItem);
		if (noErr != VCSRaiseOSErr (mContext, FSpRename (&inItem.fsItem, newSpec.name))) goto CleanUp;
		
		//	$ cvs remove old
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "remove"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) goto CleanUp;
		if (noErr != VCSSendCommand (mContext, &command, &cwd)) goto CleanUp;
		
		//	$ cvs add new
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "add"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, newSpec.name))) goto CleanUp;
		if (noErr != VCSSendCommand (mContext, &command, &cwd)) goto CleanUp;
		
		//	$ cp new old
		//	This is because the IDE won't update status if the file vanishes.
		if (noErr != VCSRaiseOSErr (mContext, FSpFileCopy (&newSpec, &cwd, inItem.fsItem.name, 0, 0, true))) goto CleanUp;
		
		//	success
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
		
		FSpDelete (&inItem.fsItem);

	CleanUp:
		
		AEDisposeDesc (&command);

		return inItem.eItemStatus;
		
	} // end ProcessRegularFile
