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

#include "MoreFilesExtras.h"

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
		
		//	Stuff to clean up
		StAEDesc 	command;
		Handle		output = nil;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kGetTask, inItem.fsItem.name);
		
		//	Get the db location
		CWVCSDatabaseConnection	db;
		mContext.GetDatabase (db);
		FSSpec		projectFile;
		mContext.GetProjectFile (projectFile);
		
		//	Get the cwd for update
		FSSpec		cwd = db.sProjectRoot;
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	cvs -r update <project name>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-r"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "update"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, db.sProjectRoot.name))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, VCSSendOutputCommand (mContext, &command, &cwd, &output))) goto CleanUp;
					
		//	Parse the output
		while (0 != GetHandleSize (output)) {
			Handle	line = nil;
			
			if (noErr != VCSRaiseOSErr (mContext, GetNextLine (&line, output))) goto CleanUp;
			if (noErr != VCSCheckCmdOutputLine (mContext, "\pupdate", line)) goto CleanUp;
			VCSUpdateFileStatus (mContext, &db.sProjectRoot, &projectFile, line);
				
			DisposeHandle (line);
			line = nil;
			} // while
					
		inItem.eItemStatus = cwItemStatusSucceeded;

	CleanUp:
		
		if (output != nil) DisposeHandle ((Handle) output);
		output = nil;

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
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	cvs -r update <file>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-r"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "update"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, VCSSendCommand (mContext, &command, &cwd))) goto CleanUp;
		
		//	Make sure project is locked
		FSSpec		projectFile;
		mContext.GetProjectFile (projectFile);
		if (::FSpEqual (&projectFile, &inItem.fsItem))
			::FSpSetFLock (&inItem.fsItem);
			
		//	Success 
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
		
	CleanUp:
		
		return inItem.eItemStatus;
		
	} // end VCSGetFile

