#include "VCSGet.h"

#include "VCSAdvancedOptionsDialog.h"
#include "VCSDialogCheckboxItem.h"
#include "VCSDialogGroupBox.h"
#include "VCSDialogPopupItem.h"
#include "VCSDialogTextItem.h"
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

//	=== Types ===

class CVSGetOptionsDialog : public VCSAdvancedOptionsDialog 

	{
	
		//	Sticky group
		VCSDialogItem			mStickyGroupLabelItem;
		VCSDialogPopupItem		mKeywordItem;
		VCSDialogCheckboxItem	mResetTagsItem;
		VCSDialogGroupBox		mStickyGroupItem;
		
		//	Revision group
		VCSDialogItem			mRevisionGroupLabelItem;
		VCSDialogCheckboxItem	mPruneDirectoriesItem;
		VCSDialogCheckboxItem	mCreateDirectoriesItem;
		VCSDialogPopupItem		mRevisionTypeItem;
		VCSDialogTextItem		mRevisionTextItem;
		VCSDialogCheckboxItem	mRevisionRecentItem;
		VCSDialogTextItem		mRevisionMergeItem;
		VCSDialogTextItem		mRevisionIntoLabelItem;
		VCSDialogTextItem		mRevisionIntoItem;
		VCSDialogGroupBox		mRevisionGroupItem;
		
	protected:
	
		//	Modal dialog handling
		virtual	Boolean OnItemHit 					(DialogItemIndex		inItemHit);

	public:
		
		enum {
			kPromptItem = cancel + 1,
			
			kStickyGroupLabelItem,
			kKeywordItem,
			kResetTagsItem,
			kStickyGroupItem,
			
			kRevisionGroupLabelItem,
			kPruneDirectoriesItem,
			kCreateDirectoriesItem,
			kRevisionTypeItem,
			kRevisionTextItem,
			kRevisionRecentItem,
			kRevisionMergeLabelItem,
			kRevisionMergeItem,
			kRevisionIntoLabelItem,
			kRevisionIntoItem,
			kRevisionGroupItem,
			
			kResourceID = 16220
			};
	
						CVSGetOptionsDialog			(const VCSContext&		mContext,
													 short					inDLOGid);
		virtual			~CVSGetOptionsDialog		(void);
		
		Boolean 		Validate 					(void) const;

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
//		• CVSGetOptionsDialog
// ---------------------------------------------------------------------------

CVSGetOptionsDialog::CVSGetOptionsDialog (

	const VCSContext&	mContext,
	short				inDLOGid)
	
	: VCSAdvancedOptionsDialog (mContext, inDLOGid)
	
	, mStickyGroupLabelItem (*this, kStickyGroupLabelItem)
	, mKeywordItem (*this, kKeywordItem)
	, mResetTagsItem (*this, kResetTagsItem)
	, mStickyGroupItem (*this, kStickyGroupItem, &mStickyGroupLabelItem)
	
	, mRevisionGroupLabelItem (*this, kRevisionGroupLabelItem)
	, mPruneDirectoriesItem (*this, kPruneDirectoriesItem)
	, mCreateDirectoriesItem (*this, kCreateDirectoriesItem)
	, mRevisionTypeItem (*this, kRevisionTypeItem)
	, mRevisionTextItem (*this, kRevisionTextItem)
	, mRevisionRecentItem (*this, kRevisionRecentItem)
	, mRevisionMergeItem (*this, kRevisionMergeItem)
	, mRevisionIntoLabelItem (*this, kRevisionIntoLabelItem)
	, mRevisionIntoItem (*this, kRevisionIntoItem)
	, mRevisionGroupItem (*this, kRevisionGroupItem, &mRevisionGroupLabelItem)

	{ // begin CVSGetOptionsDialog
	
	} // end CVSGetOptionsDialog
	
// ---------------------------------------------------------------------------
//		• ~CVSGetOptionsDialog
// ---------------------------------------------------------------------------

CVSGetOptionsDialog::~CVSGetOptionsDialog (void)

	{ // begin ~CVSGetOptionsDialog
	
	} // end ~CVSGetOptionsDialog

// ---------------------------------------------------------------------------
//		• Validate
// ---------------------------------------------------------------------------

Boolean 
CVSGetOptionsDialog::Validate (void) const
	
	{ // begin Validate
		
		Str255	text;

		if (mRevisionTypeItem.GetValue () == 1) return true;
		mRevisionTextItem.GetDescriptor (text);
		if (0 == text[0]) return false;
		
		return true;
		
	} // end Validate
	
// ---------------------------------------------------------------------------
//		• OnItemHit
// ---------------------------------------------------------------------------

Boolean 
CVSGetOptionsDialog::OnItemHit (

	short	inItemHit)
	
	{ // begin OnItemHit
		
		Boolean	revisionActive = (mRevisionTypeItem.GetValue () > 1);
		
		Str31	blank = {0};
		Str255	revisionName;
		mRevisionMergeItem.GetDescriptor (revisionName);
		Boolean	mergeActive = (revisionName[0] != 0);
		
		switch (inItemHit) {
			case kFirstTimeItem:
			case kRevisionTypeItem:
				//	Argument Text
				mRevisionTextItem.SetEnable (revisionActive);
				mRevisionTextItem.SetShow (revisionActive);
				
				//	Recent Checkbox
				mRevisionRecentItem.SetEnable (revisionActive);
				mRevisionRecentItem.SetShow (revisionActive);
				if (inItemHit != kFirstTimeItem) break;
				//	Fall throughä
				
			case kRevisionMergeItem:
				//	Into Label
				mRevisionIntoLabelItem.SetEnable (mergeActive);
				mRevisionIntoLabelItem.SetShow (mergeActive);

				//	Into Text
				mRevisionIntoItem.SetEnable (mergeActive);
				mRevisionIntoItem.SetShow (mergeActive);
				if (!mergeActive) mRevisionIntoItem.SetDescriptor (blank);
				break;
			} // switch
		
		SetItemEnable (ok, Validate ());
			
		return VCSAdvancedOptionsDialog::OnItemHit (inItemHit);
		
	} // end OnItemHit
	
// ---------------------------------------------------------------------------
//		• DoDialog
// ---------------------------------------------------------------------------

OSErr 
CVSGetOptionsDialog::DoDialog (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	const	AEDescList&		inDefaults,
	AEDescList&				outOptions)
	
	{ // begin DoDialog
		
		OSErr	e = noErr;

		VCSDialog::SetParamText (inFile.name);
		CVSGetOptionsDialog	d (inPB, kResourceID);
		if (noErr != (e = d.SetOptionsList (inDefaults, kResourceID))) goto CleanUp;
		if (ok != d.DoModalDialog ()) return userCanceledErr;
		
		if (noErr != (e = d.GetOptionsList (outOptions, kResourceID))) goto CleanUp;
		
		//	Parse -j options
		Str255	arg = {0};
		Str255	text;
		d.GetItemText (kRevisionMergeItem, text);
		if (text[0] && d.GetItemEnable (kRevisionMergeItem)) {
			arg[0] = 0;
			AppendPString (AppendPString (arg, "\p-j"), text);
			if (noErr != (e = ::CVSAddPStringArg (&outOptions, arg))) goto CleanUp;
			} // if
			
		d.GetItemText (kRevisionIntoItem, text);
		if (text[0] && d.GetItemEnable (kRevisionIntoItem)) {
			arg[0] = 0;
			AppendPString (AppendPString (arg, "\p-j"), text);
			if (noErr != (e = ::CVSAddPStringArg (&outOptions, arg))) goto CleanUp;
			} // if

	CleanUp:
	
		return e;
		
	} // end DoDialog

// ---------------------------------------------------------------------------
//		• GetOptions
// ---------------------------------------------------------------------------

OSErr 
CVSGetOptionsDialog::GetOptions (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	AEDescList&				outOptions)
	
	{ // begin GetOptions
		
		OSErr				e = noErr;
		
		//	Get the defaults
		AEDescList			defaultList = {typeNull, nil};
		if (noErr != (e = ::AECreateList (nil, 0 , false, &defaultList))) return e;
			
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
//		• VCSGet
// ---------------------------------------------------------------------------

VCSGet::VCSGet (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, true, true, true)
		
	{ // begin VCSGet
		
	} // end VCSGet

// ---------------------------------------------------------------------------
//		• ~VCSGet
// ---------------------------------------------------------------------------

VCSGet::~VCSGet (void)
	
	{ // begin ~VCSGet
		
	} // end ~VCSGet

// ---------------------------------------------------------------------------
//		• ProcessRegularFolder
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
		
		//	cvs -r update <options>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-r"))) return inItem.eItemStatus;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "update"))) return inItem.eItemStatus;

		//	Get the options.
		switch (e = CVSGetOptionsDialog::GetOptions (mContext, inItem.fsItem, command)) {
			case userCanceledErr:
				return inItem.eItemStatus = cwItemStatusCancelled;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) return inItem.eItemStatus;
				break;
			} // switch
		
		//	Send the command to MacCVS
		switch (VCSRaiseOSErr (mContext, VCSSendOutputCommand (mContext, &command, &cwd, &output.mH))) {
			case noErr:
				break;
				
			case userCanceledErr:
				inItem.eItemStatus = cwItemStatusCancelled;
				
			default:
				return inItem.eItemStatus;
			} // if

		//	Display the output
		VCSDisplayResult (mContext, messagetypeInfo, kErrorStrings, kCvsInfo, output);

		//	Parse the output
		while (0 != GetHandleSize (output)) {
			Handle	line = nil;
			
			if (noErr != VCSRaiseOSErr (mContext, GetNextLine (&line, output))) return inItem.eItemStatus;
			if (noErr != VCSCheckCmdOutputLine (mContext, "\pupdate", line)) return inItem.eItemStatus;
			VCSUpdateFileStatus (mContext, &db.sProjectRoot, line);
				
			DisposeHandle (line);
			line = nil;
			} // while
					
		inItem.eItemStatus = cwItemStatusSucceeded;

		return inItem.eItemStatus;
		
	} // end ProcessRegularFolder

// ---------------------------------------------------------------------------
//		• ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSGet::ProcessRegularFile (
	
	CWVCSItem& 			inItem)

	{ // begin VCSGetFile
 
		OSErr		e = noErr;
		
		StAEDesc 		command;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kGetTask, inItem.fsItem.name);
		
		//	Get the cwd for update
		FSSpec		cwd = inItem.fsItem;
		if (noErr != VCSRaiseOSErr (mContext, ::FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) return inItem.eItemStatus;
		
		//	cvs -r update <options> <file>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-r"))) return inItem.eItemStatus;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "update"))) return inItem.eItemStatus;

		//	Get the options.
		switch (e = CVSGetOptionsDialog::GetOptions (mContext, inItem.fsItem, command)) {
			case userCanceledErr:
				return inItem.eItemStatus = cwItemStatusCancelled;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) return inItem.eItemStatus;
				break;
			} // switch
				
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
		
		//	Success 
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
		
		return inItem.eItemStatus;
		
	} // end VCSGetFile

