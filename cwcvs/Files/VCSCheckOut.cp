#include "VCSCheckOut.h"

#include "VCSError.h"
#include "VCSPrefs.h"
#include "VCSPrompt.h"
#include "VCSTask.h"
#include "VCSUtil.h"
#include "VCSVersion.h"

#include "CVSCommand.h"

#include "StAEDesc.h"
#include "StHandle.h"

// ---------------------------------------------------------------------------
//		¥ VCSCheckOut
// ---------------------------------------------------------------------------

VCSCheckOut::VCSCheckOut (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true)
		
	{ // begin VCSCheckOut
		
	} // end VCSCheckOut

// ---------------------------------------------------------------------------
//		¥ ~VCSCheckOut
// ---------------------------------------------------------------------------

VCSCheckOut::~VCSCheckOut (void)
	
	{ // begin ~VCSCheckOut
		
	} // end ~VCSCheckOut

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFile
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
		
		//	Get the filetype
		FInfo	fInfo;
		if (noErr != ::FSpGetFInfo (&inItem.fsItem, &fInfo)) goto CleanUp;
		
		//	Check whether the file is being edited
		if (fInfo.fdType != 'TEXT') {
			VCSTask 	editorsTask (mContext, kTaskStrings, kEditorsTask, inItem.fsItem.name);
			StAEDesc 	editorsCmd;
			StHandle	output;
			
			//	cvs editors <filename>
			if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "editors"))) goto CleanUp;
			if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) goto CleanUp;
			
			//	Send the command to MacCVS
			switch (VCSRaiseOSErr (mContext, VCSSendOutputCommand (mContext, &command, &cwd, &output.mH))) {
				case noErr:
					break;
					
				case userCanceledErr:
					inItem.eItemStatus = cwItemStatusCancelled;
					
				default:
					goto CleanUp;
				} // if
			
			//	If the result is non-empty
			if (output.GetSize () != 0) {
				//	Ask if they want to continue
				switch (VCSPromptYesNoCancel (mContext, kPromptStringsID, kCheckOutBinaryPrompt, inItem.fsItem.name)) {
					case kPromptYes:
						break;
						
					case kPromptCancel:
						inItem.eItemStatus = cwItemStatusCancelled;
					
					case kPromptNo:
					default:
						goto CleanUp;
					} // switch
				} // if
			} // if

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
