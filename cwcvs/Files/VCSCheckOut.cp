#include "VCSCheckOut.h"

#include "VCSError.h"
#include "VCSGet.h"
#include "VCSPrefs.h"
#include "VCSPrompt.h"
#include "VCSResult.h"
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
			//	Get the cwd for commands
			FSSpec		cwd = inItem.fsItem;
			if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) break;
			
			//	Get the filetype
			FInfo	fInfo;
			if (noErr != ::FSpGetFInfo (&inItem.fsItem, &fInfo)) break;
			
			//	Is it a binary file?
			StHandle	keywords;
			if (noErr != VCSRaiseOSErr (mContext, VCSVersion::ParseEntriesFile (&inItem.fsItem, 0, 0, &keywords.mH))) break;
			Boolean		isBinary ((3 == keywords.GetSize ()) && (0 == ::memcmp (*keywords, "-kb", 3)));
			
			//	If it _is_ binary, make sure there is no recent checkin
			if (isBinary) {
				StAEDesc		updateCmd;
				StHandle		updateOutput;
				
				//	cvs -nq update <filename>
				if (noErr != VCSRaiseOSErr (mContext, ::CVSCreateCommand (&updateCmd, "-nq"))) break;
				if (noErr != VCSRaiseOSErr (mContext, ::CVSAddCStringArg (&updateCmd, "update"))) break;
				if (noErr != VCSRaiseOSErr (mContext, ::CVSAddPStringArg (&updateCmd, inItem.fsItem.name))) break;
				
				//	Send the command to MacCVS
				switch (VCSRaiseOSErr (mContext, ::VCSSendOutputCommand (mContext, &updateCmd, &cwd, &updateOutput.mH))) {
					case noErr:
						break;
						
					case userCanceledErr:
						inItem.eItemStatus = cwItemStatusCancelled;
						
					default:
						return inItem.eItemStatus;
					} // if
				
				//	Display the result as a message
				if (updateOutput.GetSize ()) {
					VCSDisplayResult (mContext, messagetypeInfo, kErrorStrings, kCvsInfo, updateOutput);
					::ParamText (inItem.fsItem.name, nil, nil, nil);
					switch (VCSPrompt (mContext, kBinaryALRT)) {
						case ok:
							break;
							
						case cancel:
							return inItem.eItemStatus = cwItemStatusCancelled;
							
						case kEditLatestItem:
							if (cwItemStatusSucceeded != (inItem.eItemStatus = VCSGet (mContext).ProcessRegularFile (inItem))) 
								return inItem.eItemStatus;
							break;
						} // switch
					} // if
				} // if

			//	Check whether the file is being edited
			{
				VCSTask 	editorsTask (mContext, kTaskStrings, kEditorsTask, inItem.fsItem.name);
				StAEDesc 	editorsCmd;
				StHandle	output;
				
				//	cvs editors <filename>
				if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&editorsCmd, "editors"))) break;
				if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&editorsCmd, inItem.fsItem.name))) break;
				
				//	Send the command to MacCVS
				switch (VCSRaiseOSErr (mContext, VCSSendOutputCommand (mContext, &editorsCmd, &cwd, &output.mH))) {
					case noErr:
						break;
						
					case userCanceledErr:
						inItem.eItemStatus = cwItemStatusCancelled;
						
					default:
						return inItem.eItemStatus;
					} // if
				
				//	Display the result as a message
				if (output.GetSize ()) VCSDisplayResult (mContext, messagetypeInfo, kErrorStrings, kCvsInfo, output);
				
				//	If the result is non-empty and we have a binary file
				if ((3 == keywords.GetSize ()) && (0 == ::memcmp (*keywords, "-kb", 3)) && output.GetSize ()) {
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
				} // editors block

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
