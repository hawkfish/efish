#include "VCSGet.h"

#include "VCSError.h"
#include "VCSPrefs.h"
#include "VCSPrompt.h"
#include "VCSResult.h"
#include "VCSTask.h"
#include "VCSUtil.h"
#include "VCSVersion.h"

#include "CVSCommand.h"
#include "StAEDesc.h"
#include "StHandle.h"

#include "MoreFilesExtras.h"

#include <Errors.h>
#include <TextUtils.h>

// ---------------------------------------------------------------------------
//		¥ VCSGet
// ---------------------------------------------------------------------------

VCSGet::VCSGet (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, true)
		
	{ // begin VCSGet
		
	} // end VCSGet

// ---------------------------------------------------------------------------
//		¥ ~VCSGet
// ---------------------------------------------------------------------------

VCSGet::~VCSGet (void)
	
	{ // begin ~VCSGet
		
	} // end ~VCSGet

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSGet::ProcessRegularFolder (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFolder
		
		OSErr		e = noErr;
		
		StAEDesc 	command;
		StHandle	output;
		FSSpec		cwd = inItem.fsItem;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kGetTask, inItem.fsItem.name);
		
		//	Get the db location
		CWVCSDatabaseConnection	db;
		mContext.GetDatabase (db);
		
		//	Handle project locking
		FSSpec		projectFile;
		mContext.GetProjectFile (projectFile);
		Boolean		wasLocked = (fLckdErr == ::FSpCheckObjectLock (&projectFile));
		if (wasLocked) ::FSpRstFLock (&projectFile);
		
		//	cvs -r update
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-r"))) return inItem.eItemStatus;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "update"))) return inItem.eItemStatus;

		//	Send the command to MacCVS
		switch (VCSRaiseOSErr (mContext, VCSSendOutputCommand (mContext, &command, &cwd, &output.mH))) {
			case noErr:
				break;
				
			case userCanceledErr:
				inItem.eItemStatus = cwItemStatusCancelled;
				
			default:
				return inItem.eItemStatus;
			} // if

		//	Restore lock state
		if (wasLocked) ::FSpSetFLock (&projectFile);
		
		//	Parse the output
		VCSDisplayResult (mContext, messagetypeInfo, kErrorStrings, kCvsInfo, output);

		//	Parse the output
		while (0 != GetHandleSize (output)) {
			Handle	line = nil;
			
			if (noErr != VCSRaiseOSErr (mContext, GetNextLine (&line, output))) return inItem.eItemStatus;
			if (noErr != VCSCheckCmdOutputLine (mContext, "\pupdate", line)) return inItem.eItemStatus;
			VCSUpdateFileStatus (mContext, &db.sProjectRoot, &projectFile, line);
				
			DisposeHandle (line);
			line = nil;
			} // while
					
		inItem.eItemStatus = cwItemStatusSucceeded;

		return inItem.eItemStatus;
		
	} // end ProcessRegularFolder

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSGet::ProcessRegularFile (
	
	CWVCSItem& 			inItem)

	{ // begin VCSGetFile
 
		//	Stuff to clean up
		StAEDesc 		command;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kGetTask, inItem.fsItem.name);
		
		//	Get the cwd for update
		FSSpec		cwd = inItem.fsItem;
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) return inItem.eItemStatus;
		
		//	cvs -r update <file>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-r"))) return inItem.eItemStatus;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "update"))) return inItem.eItemStatus;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) return inItem.eItemStatus;

		//	Send the command to MacCVS
		switch (VCSRaiseOSErr (mContext, VCSSendCommand (mContext, &command, &cwd))) {
			case noErr:
				break;
				
			case userCanceledErr:
				inItem.eItemStatus = cwItemStatusCancelled;
				
			default:
				return inItem.eItemStatus;
			} // if
		
		//	Make sure project is locked
		FSSpec		projectFile;
		mContext.GetProjectFile (projectFile);
		if (::FSpEqual (&projectFile, &inItem.fsItem))
			::FSpSetFLock (&inItem.fsItem);
			
		//	Success 
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
		
		return inItem.eItemStatus;
		
	} // end VCSGetFile

