#include "VCSHistory.h"

#include "VCSError.h"
#include "VCSResult.h"
#include "VCSTask.h"
#include "VCSUtil.h"

#include "CVSCommand.h"

#include <TextUtils.h>

// ---------------------------------------------------------------------------
//		¥ VCSHistory
// ---------------------------------------------------------------------------

VCSHistory::VCSHistory (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true)
	
	{ // begin VCSHistory
		
	} // end VCSHistory

// ---------------------------------------------------------------------------
//		¥ ~VCSHistory
// ---------------------------------------------------------------------------

VCSHistory::~VCSHistory (void)
	
	{ // begin ~VCSHistory
		
	} // end ~VCSHistory

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSHistory::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFile
				
		OSErr				e = noErr;
		
		
		//	Stuff to clean up
		AEDesc 				command = {typeNull, nil};
		Handle				output = nil;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 			task (mContext, kTaskStrings, kHistoryTask, inItem.fsItem.name);

		//	Get the cwd for checkout
		FSSpec 				cwd = inItem.fsItem;
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	cvs log <filename>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "log"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) goto CleanUp;
		if (noErr != (e = VCSSendOutputCommand (mContext, &command, &cwd, &output))) goto CleanUp;

		//	Display the results
		if (noErr != (e = VCSCheckCmdOutput (mContext, "\plog", output))) goto CleanUp;

		Str255				title = {0};
		AppendPString (AppendPString (title, inItem.fsItem.name), "\p.history");
		mContext.CreateDocument (p2cstr (title), output);
			
		inItem.eItemStatus = cwItemStatusSucceeded;
		
	CleanUp:
	
		if (output != nil) DisposeHandle (output);
		output = nil;

		AEDisposeDesc (&command);
				
		return inItem.eItemStatus;
	
	} // end ProcessRegularFile

