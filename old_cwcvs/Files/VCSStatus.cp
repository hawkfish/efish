#include "VCSStatus.h"

#include "VCSError.h"
#include "VCSResult.h"
#include "VCSTask.h"
#include "VCSUtil.h"
#include "VCSVersion.h"

#include "CVSCommand.h"

#include <TextUtils.h>

// ---------------------------------------------------------------------------
//		¥ VCSStatus
// ---------------------------------------------------------------------------

VCSStatus::VCSStatus (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true)
	
	{ // begin VCSStatus
		
	} // end VCSStatus

// ---------------------------------------------------------------------------
//		¥ ~VCSStatus
// ---------------------------------------------------------------------------

VCSStatus::~VCSStatus (void)
	
	{ // begin ~VCSStatus
		
	} // end ~VCSStatus

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSStatus::ProcessRegularFile (
	
	CWVCSItem& 		inItem)
	
	{ // begin ProcessRegularFile
		
		OSErr				e = noErr;
		FSSpec 				cwd = inItem.fsItem;
		Str255				title;
		
		//	Stuff to clean up
		AEDesc 				command = {typeNull, nil};
		Handle				output = nil;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 			task (mContext, kTaskStrings, kStatusTask, inItem.fsItem.name);

		//	Get the cwd for checkout
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	cvs status <filename>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "status"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) goto CleanUp;
		if (noErr != (e = VCSSendOutputCommand (mContext, &command, &cwd, &output))) goto CleanUp;
		
		//	Display the results
		if (noErr != (e = VCSCheckCmdOutput (mContext, "\pstatus", output))) goto CleanUp;

		title[0] = 0; 
		AppendPString (AppendPString (title, inItem.fsItem.name), "\p.status");
		mContext.CreateDocument (p2cstr (title), output);

		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);

	CleanUp:
	
		if (output != nil) ::DisposeHandle (output);
		output = nil;

		return inItem.eItemStatus;
	
	} // end ProcessRegularFile
