#include "VCSLabel.h"

#include "VCSAdvancedOptionsDialog.h"
#include "VCSDialogCheckboxItem.h"
#include "VCSDialogPopupItem.h"
#include "VCSDialogTextItem.h"
#include "VCSError.h"
#include "VCSPrefs.h"
#include "VCSPrompt.h"
#include "VCSTask.h"
#include "VCSUtil.h"

#include "StAEDesc.h"

#include "CVSCommand.h"

#include <Dialogs.h>

//	=== Types ===

class CVSLabelOptionsDialog : public VCSAdvancedOptionsDialog 

	{
	
		VCSDialogCheckboxItem	mReplaceItem;
		VCSDialogCheckboxItem	mBranchItem;
		VCSDialogCheckboxItem	mCheckItem;
		VCSDialogCheckboxItem	mDeleteItem;
		VCSDialogTextItem		mLabelTextItem;

	protected:
	
		//	Modal dialog handling
		virtual	Boolean OnItemHit 					(DialogItemIndex		inItemHit);

	public:
		
		enum {
			kReplaceItem = cancel + 1,
			kBranchItem,
			kCheckItem,
			kDeleteItem,

			kPromptItem,
			kLabelTextItem,
			
			kResourceID = 16310
			};
	
						CVSLabelOptionsDialog		(const VCSContext&		mContext,
													 short					inDLOGid);
		virtual			~CVSLabelOptionsDialog		(void);
		
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
//		€ CVSLabelOptionsDialog
// ---------------------------------------------------------------------------

CVSLabelOptionsDialog::CVSLabelOptionsDialog (

	const VCSContext&	mContext,
	short				inDLOGid)
	
	: VCSAdvancedOptionsDialog (mContext, inDLOGid)
	
	, mReplaceItem (*this, kReplaceItem)
	, mBranchItem (*this, kBranchItem)
	, mCheckItem (*this, kCheckItem)
	, mDeleteItem (*this, kDeleteItem)
	, mLabelTextItem (*this, kLabelTextItem)

	{ // begin CVSLabelOptionsDialog
	
	} // end CVSLabelOptionsDialog
	
// ---------------------------------------------------------------------------
//		€ ~CVSLabelOptionsDialog
// ---------------------------------------------------------------------------

CVSLabelOptionsDialog::~CVSLabelOptionsDialog (void)

	{ // begin ~CVSLabelOptionsDialog
	
	} // end ~CVSLabelOptionsDialog

// ---------------------------------------------------------------------------
//		€ OnItemHit
// ---------------------------------------------------------------------------

Boolean 
CVSLabelOptionsDialog::OnItemHit (

	short	inItemHit)
	
	{ // begin OnItemHit
		
		switch (inItemHit) {
			case kFirstTimeItem:
			case kLabelTextItem:
				Str255	label;
				mLabelTextItem.GetDescriptor (label);
				SetItemEnable (ok, label[0] ? true : false);
				break;
			} // switch
		
		return VCSAdvancedOptionsDialog::OnItemHit (inItemHit);
		
	} // end OnItemHit
	
// ---------------------------------------------------------------------------
//		€ DoDialog
// ---------------------------------------------------------------------------

OSErr 
CVSLabelOptionsDialog::DoDialog (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	const	AEDescList&		inDefaults,
	AEDescList&				outOptions)
	
	{ // begin DoDialog
		
		OSErr	e = noErr;

		VCSDialog::SetParamText (inFile.name);
		CVSLabelOptionsDialog	d (inPB, kResourceID);
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
CVSLabelOptionsDialog::GetOptions (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	AEDescList&				outOptions)
	
	{ // begin GetOptions
		
		OSErr				e = noErr;
		
		//	Get the defaults
		StAEDesc			defaultList;
		if (noErr != (e = ::AECreateList (nil, 0 , false, &defaultList))) return e;
		if (noErr != (e = ::VCSGetDefaultOptionsList (inPB, kResourceID, kResourceID, defaultList))) return e;

		if (noErr != (e = DoDialog (inPB, inFile, defaultList, outOptions))) goto CleanUp;
			
	CleanUp:
	
		return e;
	
	} // end GetOptions

#pragma mark -

class CVSRemoteLabelOptionsDialog : public VCSAdvancedOptionsDialog 

	{
	
		VCSDialogCheckboxItem	mReplaceItem;
		VCSDialogCheckboxItem	mBranchItem;
		VCSDialogCheckboxItem	mDeleteItem;
		VCSDialogCheckboxItem	mAtticItem;
		VCSDialogCheckboxItem	mNoProgramItem;
		
		VCSDialogPopupItem		mRevisionTypeItem;
		VCSDialogTextItem		mRevisionTextItem;
		VCSDialogCheckboxItem	mRevisionRecentItem;

		VCSDialogTextItem		mLabelTextItem;

	protected:
	
		//	Modal dialog handling
		virtual	Boolean OnItemHit 					(DialogItemIndex		inItemHit);
		
	public:
		
		enum {
			kReplaceItem = cancel + 1,
			kBranchItem,
			kDeleteItem,
			kAtticItem,

			kRevisionTypeItem,
			kRevisionTextItem,
			kRevisionRecentItem,

			kNoProgramItem,

			kPromptItem,
			kLabelTextItem,
			
			kResourceID = 16320
			};
	
						CVSRemoteLabelOptionsDialog	(const VCSContext&		mContext,
													 short					inDLOGid);
		virtual			~CVSRemoteLabelOptionsDialog(void);
		
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
//		€ CVSRemoteLabelOptionsDialog
// ---------------------------------------------------------------------------

CVSRemoteLabelOptionsDialog::CVSRemoteLabelOptionsDialog (

	const VCSContext&	mContext,
	short				inDLOGid)
	
	: VCSAdvancedOptionsDialog (mContext, inDLOGid)
	
	, mReplaceItem (*this, kReplaceItem)
	, mBranchItem (*this, kBranchItem)
	, mDeleteItem (*this, kDeleteItem)
	, mAtticItem (*this, kAtticItem)
	, mNoProgramItem (*this, kNoProgramItem)

	, mRevisionTypeItem (*this, kRevisionTypeItem)
	, mRevisionTextItem (*this, kRevisionTextItem)
	, mRevisionRecentItem (*this, kRevisionRecentItem)

	, mLabelTextItem (*this, kLabelTextItem)

	{ // begin CVSRemoteLabelOptionsDialog
	
	} // end CVSRemoteLabelOptionsDialog
	
// ---------------------------------------------------------------------------
//		€ ~CVSRemoteLabelOptionsDialog
// ---------------------------------------------------------------------------

CVSRemoteLabelOptionsDialog::~CVSRemoteLabelOptionsDialog (void)

	{ // begin ~CVSRemoteLabelOptionsDialog
	
	} // end ~CVSRemoteLabelOptionsDialog

// ---------------------------------------------------------------------------
//		€ Validate
// ---------------------------------------------------------------------------

Boolean 
CVSRemoteLabelOptionsDialog::Validate (void) const
	
	{ // begin Validate
		
		Str255	text;
		mLabelTextItem.GetDescriptor (text);
		if (text[0] == 0) return false;
		
		if (mRevisionTypeItem.GetValue () == 1) return true;
		mRevisionTextItem.GetDescriptor (text);
		if (0 == text[0]) return false;
		
		return true;
		
	} // end Validate
	
// ---------------------------------------------------------------------------
//		€ OnItemHit
// ---------------------------------------------------------------------------

Boolean 
CVSRemoteLabelOptionsDialog::OnItemHit (

	short	inItemHit)
	
	{ // begin OnItemHit
		
		Boolean	revisionActive = (mRevisionTypeItem.GetValue () > 1);

		switch (inItemHit) {
			case kFirstTimeItem:
			case kRevisionTypeItem:
				//	Argument Text
				mRevisionTextItem.SetEnable (revisionActive);
				mRevisionTextItem.SetShow (revisionActive);
				
				//	Recent Checkbox
				mRevisionRecentItem.SetEnable (revisionActive);
				mRevisionRecentItem.SetShow (revisionActive);
				break;
			} // switch
		
		SetItemEnable (ok, Validate ());

		return VCSAdvancedOptionsDialog::OnItemHit (inItemHit);
		
	} // end OnItemHit
	
// ---------------------------------------------------------------------------
//		€ DoDialog
// ---------------------------------------------------------------------------

OSErr 
CVSRemoteLabelOptionsDialog::DoDialog (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	const	AEDescList&		inDefaults,
	AEDescList&				outOptions)
	
	{ // begin DoDialog
		
		OSErr	e = noErr;

		VCSDialog::SetParamText (inFile.name);
		CVSRemoteLabelOptionsDialog	d (inPB, kResourceID);
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
CVSRemoteLabelOptionsDialog::GetOptions (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	AEDescList&				outOptions)
	
	{ // begin GetOptions
		
		OSErr				e = noErr;
		
		//	Get the defaults
		StAEDesc			defaultList;
		if (noErr != (e = ::AECreateList (nil, 0 , false, &defaultList))) return e;
		if (noErr != (e = ::VCSGetDefaultOptionsList (inPB, kResourceID, kResourceID, defaultList))) return e;

		if (noErr != (e = DoDialog (inPB, inFile, defaultList, outOptions))) goto CleanUp;
			
	CleanUp:
	
		return e;
	
	} // end GetOptions

#pragma mark -

// ---------------------------------------------------------------------------
//		€ VCSLabel
// ---------------------------------------------------------------------------

VCSLabel::VCSLabel (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, false, true, false, true)
	
	{ // begin VCSLabel
		
	} // end VCSLabel

// ---------------------------------------------------------------------------
//		€ ~VCSLabel
// ---------------------------------------------------------------------------

VCSLabel::~VCSLabel (void)
	
	{ // begin ~VCSLabel
		
	} // end ~VCSLabel

// ---------------------------------------------------------------------------
//		€ ProcessRegularFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSLabel::ProcessRegularFolder (

	CWVCSItem&	inItem)
	
	{ // begin ProcessRegularFolder
		
		OSErr		e = noErr;
		
		//	Stuff to clean up
		StAEDesc 	command;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kLabelTask, inItem.fsItem.name);
		
		//	Get the cwd for update
		CWVCSDatabaseConnection	db;
		mContext.GetDatabase (db);
		FSSpec		cwd = db.sProjectRoot;
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	cvs tag <options> <tag>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "tag"))) goto CleanUp;

		//	Get the options.
		switch (e = CVSLabelOptionsDialog::GetOptions (mContext, inItem.fsItem, command)) {
			case userCanceledErr:
				return inItem.eItemStatus = cwItemStatusCancelled;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) return inItem.eItemStatus;
				break;
			} // switch
		
		if (noErr != VCSRaiseOSErr (mContext, VCSSendCommand (mContext, &command, &cwd))) goto CleanUp;

		inItem.eItemStatus = cwItemStatusSucceeded;
		
	CleanUp:
		
		return inItem.eItemStatus;
			
	} // end ProcessRegularFolder

// ---------------------------------------------------------------------------
//		€ ProcessAdvancedFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSLabel::ProcessAdvancedFolder (

	CWVCSItem&	inItem)
	
	{ // begin ProcessAdvancedFolder
		
		OSErr		e = noErr;
		
		//	Stuff to clean up
		StAEDesc 	command;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kLabelTask, inItem.fsItem.name);
		
		//	Get the cwd for update
		CWVCSDatabaseConnection	db;
		mContext.GetDatabase (db);
		FSSpec		cwd = db.sProjectRoot;
		if (noErr != VCSRaiseOSErr (mContext, ::FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	cvs rtag <options> <tag> <module>
		if (noErr != VCSRaiseOSErr (mContext, ::CVSCreateCommand (&command, "rtag"))) goto CleanUp;

		//	Get the options.
		switch (e = CVSRemoteLabelOptionsDialog::GetOptions (mContext, inItem.fsItem, command)) {
			case userCanceledErr:
				return inItem.eItemStatus = cwItemStatusCancelled;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) return inItem.eItemStatus;
				break;
			} // switch
		
		if (noErr != VCSRaiseOSErr (mContext, ::CVSAddPStringArg (&command, db.sProjectRoot.name))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, ::VCSSendCommand (mContext, &command, &cwd))) goto CleanUp;

		inItem.eItemStatus = cwItemStatusSucceeded;
		
	CleanUp:
		
		return inItem.eItemStatus;
			
	} // end ProcessAdvancedFolder
