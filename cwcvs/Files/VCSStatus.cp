/*
	File:		VCSStatus.h

	Contains:	Implementation of the CVS Status command.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

*/

#include "VCSStatus.h"

#include "VCSAdvancedOptionsDialog.h"
#include "VCSDialogCheckboxItem.h"
#include "VCSError.h"
#include "VCSResult.h"
#include "VCSPrefs.h"
#include "VCSTask.h"
#include "VCSUtil.h"
#include "VCSVersion.h"

#include "CVSCommand.h"
#include "StAEDesc.h"
#include "StHandle.h"

#include <TextUtils.h>

//	=== Types ===

class CVSStatusOptionsDialog : public VCSAdvancedOptionsDialog 

	{
	
	protected:
		
		VCSDialogCheckboxItem	mTagInfoItem;
		
	public:
		
		enum {
			kPromptItem = cancel + 1,
			kTagInfoItem,
			
			kResourceID = 16290
			};
	
						CVSStatusOptionsDialog		(const VCSContext&		mContext,
													 short					inDLOGid);
		virtual			~CVSStatusOptionsDialog	(void);
		
		static	OSErr	DoDialog					(const 	VCSContext&		inContext,
													 const	FSSpec&			inFile,
													 const	AEDescList&		inDefaults,
													 AEDescList&			outOptions);
		static	OSErr 	GetOptions					(const 	VCSContext&		inContext,
													 const	FSSpec&			inFile,
													 AEDescList&			outOptions);
	};

//	===	Class Members ===

// ---------------------------------------------------------------------------
//		€ CVSStatusOptionsDialog
// ---------------------------------------------------------------------------

CVSStatusOptionsDialog::CVSStatusOptionsDialog (

	const VCSContext&	mContext,
	short				inDLOGid)
	
	: VCSAdvancedOptionsDialog (mContext, inDLOGid)
	
	, mTagInfoItem (*this, kTagInfoItem)
	
	{ // begin CVSStatusOptionsDialog
	
	} // end CVSStatusOptionsDialog
	
// ---------------------------------------------------------------------------
//		€ ~CVSStatusOptionsDialog
// ---------------------------------------------------------------------------

CVSStatusOptionsDialog::~CVSStatusOptionsDialog (void)

	{ // begin ~CVSStatusOptionsDialog
	
	} // end ~CVSStatusOptionsDialog

// ---------------------------------------------------------------------------
//		€ DoDialog
// ---------------------------------------------------------------------------

OSErr 
CVSStatusOptionsDialog::DoDialog (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	const	AEDescList&		inDefaults,
	AEDescList&				outOptions)
	
	{ // begin DoDialog
		
		OSErr	e = noErr;

		VCSDialog::SetParamText (inFile.name);
		CVSStatusOptionsDialog	d (inPB, kResourceID);
		if (noErr != (e = d.SetOptionsList (inDefaults, kResourceID))) goto CleanUp;
		if (ok != d.DoModalDialog ()) return userCanceledErr;
		
		if (noErr != (e = d.GetOptionsList (outOptions, kResourceID))) goto CleanUp;
		
	CleanUp:
	
		return e;
		
	} // end DoDialog

// ---------------------------------------------------------------------------
//		€ GetOptions
// ---------------------------------------------------------------------------

OSErr 
CVSStatusOptionsDialog::GetOptions (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	AEDescList&				outOptions)
	
	{ // begin GetOptions
		
		OSErr				e = noErr;
		
		//	Get the defaults
		AEDescList			defaultList = {typeNull, nil};
		if (noErr != (e = ::AECreateList (nil, 0 , false, &defaultList))) return e;
		if (noErr != (e = ::VCSGetDefaultOptionsList (inPB, kResourceID, kResourceID, defaultList))) return e;
			
		//	If not advanced, just use the defaults 
		if (!inPB.Advanced ()) {
			if (noErr != (e = ::CVSAddListArgs (&outOptions, &defaultList))) goto CleanUp;
			} // if
			
		else {
			if (noErr != (e = DoDialog (inPB, inFile, defaultList, outOptions))) goto CleanUp;
			} // else
			
	CleanUp:
	
		return e;
	
	} // end GetOptions

#pragma mark -

// ---------------------------------------------------------------------------
//		€ VCSStatus
// ---------------------------------------------------------------------------

VCSStatus::VCSStatus (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, true, true, true)
	
	{ // begin VCSStatus
		
	} // end VCSStatus

// ---------------------------------------------------------------------------
//		€ ~VCSStatus
// ---------------------------------------------------------------------------

VCSStatus::~VCSStatus (void)
	
	{ // begin ~VCSStatus
		
	} // end ~VCSStatus

// ---------------------------------------------------------------------------
//		€ ProcessRegularFolder
// ---------------------------------------------------------------------------

CWVCSCommandStatus 
VCSStatus::ProcessRegularFolder (
					
	CWVCSItem& 		inItem)
	
	{ // begin ProcessRegularFolder
		
		OSErr				e = noErr;
		FSSpec 				cwd = inItem.fsItem;
		Str255				title;
		StAEDesc 			command;
		StHandle			output;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 			task (mContext, kTaskStrings, kStatusTask, inItem.fsItem.name);

		//	cvs status <options>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "status"))) return inItem.eItemStatus;
		
		//	Get the options.
		switch (e = CVSStatusOptionsDialog::GetOptions (mContext, inItem.fsItem, command)) {
			case userCanceledErr:
				return inItem.eItemStatus = cwItemStatusCancelled;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) return inItem.eItemStatus;
			} // switch
		
		//	Send the command to MacCVS
		if (noErr != (e = VCSSendOutputCommand (mContext, &command, &cwd, &output.mH))) {
			if (userCanceledErr == e) inItem.eItemStatus = cwItemStatusCancelled;
			return inItem.eItemStatus;
			} // if
			
		//	Display the results
		if (noErr != (e = VCSCheckCmdOutput (mContext, "\pstatus", output))) return inItem.eItemStatus;

		Str255	suffix;
		::GetIndString (suffix, kStringsID, kSuffixIndex);
		title[0] = 0; 
		AppendPString (AppendPString (title, inItem.fsItem.name), suffix);
		mContext.CreateDocument (p2cstr (title), output);

		inItem.eItemStatus = cwCommandStatusSucceeded;

		return inItem.eItemStatus;
	
	} // end ProcessRegularFolder

// ---------------------------------------------------------------------------
//		€ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSStatus::ProcessRegularFile (
	
	CWVCSItem& 		inItem)
	
	{ // begin ProcessRegularFile
		
		OSErr				e = noErr;
		FSSpec 				cwd = inItem.fsItem;
		Str255				title;
		StAEDesc 			command;
		StHandle			output;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 			task (mContext, kTaskStrings, kStatusTask, inItem.fsItem.name);

		//	Get the cwd for checkout
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) return inItem.eItemStatus;
		
		//	cvs status <options> <filename>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "status"))) return inItem.eItemStatus;

		//	Get the options.
		switch (e = CVSStatusOptionsDialog::GetOptions (mContext, inItem.fsItem, command)) {
			case userCanceledErr:
				return inItem.eItemStatus = cwItemStatusCancelled;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) return inItem.eItemStatus;
			} // switch

		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) return inItem.eItemStatus;

		//	Send the command to MacCVS
		if (noErr != (e = VCSSendOutputCommand (mContext, &command, &cwd, &output.mH))) {
			if (userCanceledErr == e) inItem.eItemStatus = cwItemStatusCancelled;
			return inItem.eItemStatus;
			} // if
			
		//	Display the results
		if (noErr != (e = VCSCheckCmdOutput (mContext, "\pstatus", output))) return inItem.eItemStatus;

		Str255	suffix;
		::GetIndString (suffix, kStringsID, kSuffixIndex);
		title[0] = 0; 
		AppendPString (AppendPString (title, inItem.fsItem.name), suffix);
		mContext.CreateDocument (p2cstr (title), output);

		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);

		return inItem.eItemStatus;
	
	} // end ProcessRegularFile
