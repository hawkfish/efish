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

#include <string.h>

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
		
		
		//	Stuff to clean up
		StAEDesc 	command;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kCheckOutTask, inItem.fsItem.name);
		
		do {
			//	Get the cwd for update
			FSSpec		cwd = inItem.fsItem;
			if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) break;
			
			//	Get the filetype
			FInfo	fInfo;
			if (noErr != ::FSpGetFInfo (&inItem.fsItem, &fInfo)) break;
			
			//	Is it a binary file?
			StHandle	keywords;
			if (noErr != VCSRaiseOSErr (mContext, VCSVersion::ParseEntriesFile (&inItem.fsItem, 0, 0, &keywords.mH))) break;
			
			//	Check whether the file is being edited
			if ((3 == keywords.GetSize ()) && (0 == ::memcmp (*keywords, "-kb", 3))) {
				VCSTask 	editorsTask (mContext, kTaskStrings, kEditorsTask, inItem.fsItem.name);
				StAEDesc 	editorsCmd;
				StHandle	output;
				
				//	cvs editors <filename>
				if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "editors"))) break;
				if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) break;
				
				//	Send the command to MacCVS
				switch (VCSRaiseOSErr (mContext, VCSSendOutputCommand (mContext, &command, &cwd, &output.mH))) {
					case noErr:
						break;
						
					case userCanceledErr:
						inItem.eItemStatus = cwItemStatusCancelled;
						
					default:
						return inItem.eItemStatus;
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
							return inItem.eItemStatus;
						} // switch
					} // if
				} // if

			//	cvs -w edit <file>
			if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-w"))) break;
			if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "edit"))) break;
			if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) break;
			if (noErr != VCSRaiseOSErr (mContext, VCSSendCommand (mContext, &command, &cwd))) break;
			
			//	Unlock the file
			::FSpRstFLock (&inItem.fsItem);
			
			//	Report status
			inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
			} while (false);
			
		return inItem.eItemStatus;
		
	} // end ProcessRegularFile
