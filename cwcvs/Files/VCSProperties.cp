#include "VCSProperties.h"

#include "VCSError.h"
#include "VCSResult.h"
#include "VCSTask.h"
#include "VCSUtil.h"

#include "CVSCommand.h"

#include "StAEDesc.h"
#include "StHandle.h"

#include <string.h>

#include <Errors.h>
#include <TextUtils.h>

// ---------------------------------------------------------------------------
//		€ VCSProperties
// ---------------------------------------------------------------------------

VCSProperties::VCSProperties (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, true, true, true)
	
	{ // begin VCSProperties
		
	} // end VCSProperties

// ---------------------------------------------------------------------------
//		€ ~VCSProperties
// ---------------------------------------------------------------------------

VCSProperties::~VCSProperties (void)
	
	{ // begin ~VCSProperties
		
	} // end ~VCSProperties

// ---------------------------------------------------------------------------
//		€ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSProperties::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFile
				
		OSErr				e = noErr;
		
		//	Stuff to clean up
		StAEDesc 			command;
		StHandle			output;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 			task (mContext, kTaskStrings, kPropertiesTask, inItem.fsItem.name);

		//	Get the cwd for checkout
		FSSpec 				cwd = inItem.fsItem;
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	cvs [watchers | editors] <filename>
		const	char*	cmdName = mContext.Advanced () ? "editors" : "watchers";
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, cmdName))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) goto CleanUp;
		
		//	Send the command to MacCVS
		switch (VCSRaiseOSErr (mContext, VCSSendOutputCommand (mContext, &command, &cwd, &output.mH))) {
			case noErr:
				break;
				
			case userCanceledErr:
				inItem.eItemStatus = cwItemStatusCancelled;
				
			default:
				return inItem.eItemStatus;
			} // if

		//	Display the results
		Str255		suffix;
		::BlockMoveData (cmdName, suffix + 1, suffix[0] = ::strlen (cmdName));
		if (noErr != (e = VCSCheckCmdOutput (mContext, suffix, output))) goto CleanUp;

		Str255		title = {0};
		::BlockMoveData (cmdName, suffix + 2, suffix[0] = ::strlen (cmdName));
		suffix[1] = '.';
		++suffix[0];
		AppendPString (AppendPString (title, inItem.fsItem.name), suffix);
		mContext.CreateDocument (p2cstr (title), output);
			
		inItem.eItemStatus = cwItemStatusSucceeded;
		
	CleanUp:
	
		return inItem.eItemStatus;
	
	} // end ProcessRegularFile

// ---------------------------------------------------------------------------
//		€ ProcessRegularFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSProperties::ProcessRegularFolder (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFolder
				
		OSErr				e = noErr;
		FSSpec 				cwd = inItem.fsItem;
		StAEDesc 			command;
		StHandle			output;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 			task (mContext, kTaskStrings, kHistoryTask, inItem.fsItem.name);

		//	cvs [watchers | editors]
		const	char*	cmdName = mContext.Advanced () ? "editors" : "watchers";
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, cmdName))) goto CleanUp;

		//	Send the command to MacCVS
		switch (VCSRaiseOSErr (mContext, VCSSendOutputCommand (mContext, &command, &cwd, &output.mH))) {
			case noErr:
				break;
				
			case userCanceledErr:
				inItem.eItemStatus = cwItemStatusCancelled;
				
			default:
				return inItem.eItemStatus;
			} // if

		//	Display the results
		Str255		suffix;
		::BlockMoveData (cmdName, suffix + 1, suffix[0] = ::strlen (cmdName));
		if (noErr != (e = VCSCheckCmdOutput (mContext, suffix, output))) goto CleanUp;

		Str255		title = {0};
		::BlockMoveData (cmdName, suffix + 2, suffix[0] = ::strlen (cmdName));
		suffix[1] = '.';
		++suffix[0];
		AppendPString (AppendPString (title, inItem.fsItem.name), suffix);
		mContext.CreateDocument (p2cstr (title), output);
			
		inItem.eItemStatus = cwItemStatusSucceeded;
		
	CleanUp:
	
		return inItem.eItemStatus;
	
	} // end ProcessRegularFolder

