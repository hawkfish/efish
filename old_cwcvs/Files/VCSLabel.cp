#include "VCSLabel.h"

#include "VCSError.h"
#include "VCSPrefs.h"
#include "VCSPrompt.h"
#include "VCSTask.h"
#include "VCSUtil.h"

#include "CVSCommand.h"

#include <Dialogs.h>

//	=== Constants ===

const	short	kLabelDLOGid = 16012;
	
// ---------------------------------------------------------------------------
//		¥ VCSLabel
// ---------------------------------------------------------------------------

VCSLabel::VCSLabel (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, false, false, false, true)
	
	{ // begin VCSLabel
		
	} // end VCSLabel

// ---------------------------------------------------------------------------
//		¥ ~VCSLabel
// ---------------------------------------------------------------------------

VCSLabel::~VCSLabel (void)
	
	{ // begin ~VCSLabel
		
	} // end ~VCSLabel

// ---------------------------------------------------------------------------
//		¥ ProcessAdvancedFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSLabel::ProcessAdvancedFolder (

	CWVCSItem&	inItem)
	
	{ // begin ProcessAdvancedFolder
		
		OSErr		e = noErr;
		
		//	Stuff to clean up
		AEDesc 		command = {typeNull, nil};
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kLabelTask, inItem.fsItem.name);
		
		//	Get the cwd for update
		CWVCSDatabaseConnection	db;
		mContext.GetDatabase (db);
		FSSpec		cwd = db.sProjectRoot;
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	Get the label
		Str255		label = {0};
		if (ok != VCSPromptString (mContext, kLabelDLOGid, label, sizeof (label))) {
			inItem.eItemStatus = cwItemStatusCancelled;
			goto CleanUp;
			} // if
			
		//	cvs tag -F <tag>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "tag"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "-F"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, label))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, VCSSendCommand (mContext, &command, &cwd))) goto CleanUp;

		inItem.eItemStatus = cwItemStatusSucceeded;
		
	CleanUp:
		
		AEDisposeDesc (&command);

		return inItem.eItemStatus;
			
	} // end ProcessAdvancedFolder
