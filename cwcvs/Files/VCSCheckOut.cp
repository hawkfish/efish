#include "VCSCheckOut.h"

#include "VCSError.h"
#include "VCSPrefs.h"
#include "VCSPrompt.h"
#include "VCSTask.h"
#include "VCSUtil.h"
#include "VCSVersion.h"

#include "CVSCommand.h"

#include "StAEDesc.h"

// ---------------------------------------------------------------------------
//		€ VCSCheckOut
// ---------------------------------------------------------------------------

VCSCheckOut::VCSCheckOut (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true)
		
	{ // begin VCSCheckOut
		
	} // end VCSCheckOut

// ---------------------------------------------------------------------------
//		€ ~VCSCheckOut
// ---------------------------------------------------------------------------

VCSCheckOut::~VCSCheckOut (void)
	
	{ // begin ~VCSCheckOut
		
	} // end ~VCSCheckOut

// ---------------------------------------------------------------------------
//		€ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSCheckOut::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFile
		
		FSSpec		cwd = inItem.fsItem;
		
		//	Stuff to clean up
		StAEDesc 	command;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kCheckOutTask, inItem.fsItem.name);
		
		//	Get the cwd for update
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	cvs -w edit <file>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-w"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "edit"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, VCSSendCommand (mContext, &command, &cwd))) goto CleanUp;
		
		//	Unlock the file
		::FSpRstFLock (&inItem.fsItem);
		
		//	Report status
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);

	CleanUp:
		
		return inItem.eItemStatus;
		
	} // end ProcessRegularFile
