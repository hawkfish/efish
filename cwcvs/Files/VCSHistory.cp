#include "VCSHistory.h"

#include "VCSAdvancedOptionsDialog.h"
#include "VCSDialogCheckboxItem.h"
#include "VCSDialogGroupBox.h"
#include "VCSDialogPopupItem.h"
#include "VCSDialogTextItem.h"
#include "VCSError.h"
#include "VCSPrefs.h"
#include "VCSResult.h"
#include "VCSTask.h"
#include "VCSUtil.h"

#include "CVSCommand.h"

#include "StAEDesc.h"
#include "StHandle.h"

#include <TextUtils.h>

//	=== Types ===

class CVSHistoryOptionsDialog : public VCSAdvancedOptionsDialog 

	{
	
		//	Content group
		VCSDialogItem			mContentGroupLabelItem;
		VCSDialogPopupItem		mFileInfoItem;
		VCSDialogCheckboxItem	mNoTagsItem;
		VCSDialogGroupBox		mContentGroupItem;
		
		//	Revision group
		VCSDialogItem			mRevisionGroupLabelItem;
		VCSDialogTextItem		mDatesItem;
		VCSDialogTextItem		mStatusItem;
		VCSDialogTextItem		mRevisionsItem;
		VCSDialogTextItem		mEditorsItem;
		VCSDialogGroupBox		mRevisionGroupItem;
		
	public:
		
		enum {
			kPromptItem = cancel + 1,
			
			kContentGroupLabelItem,
			kFileInfoItem,
			kNoTagsItem,
			kContentGroupItem,
			
			kRevisionGroupLabelItem,
			kBranchItem,
			kDatesLabelItem,
			kDatesItem,
			kStatusLabelItem,
			kStatusItem,
			kRevisionsLabelItem,
			kRevisionsItem,
			kEditorsLabelItem,
			kEditorsItem,
			kRevisionGroupItem,
			
			kInfoBaseIndex = 5,
			
			kResourceID = 16280
			};
	
						CVSHistoryOptionsDialog		(const VCSContext&		mContext,
													 short					inDLOGid);
		virtual			~CVSHistoryOptionsDialog		(void);
		
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
//		€ CVSHistoryOptionsDialog
// ---------------------------------------------------------------------------

CVSHistoryOptionsDialog::CVSHistoryOptionsDialog (

	const VCSContext&	mContext,
	short				inDLOGid)
	
	: VCSAdvancedOptionsDialog (mContext, inDLOGid)
	
	, mContentGroupLabelItem (*this, kContentGroupLabelItem)
	, mFileInfoItem (*this, kFileInfoItem)
	, mNoTagsItem (*this, kNoTagsItem)
	, mContentGroupItem (*this, kContentGroupItem, &mContentGroupLabelItem)
	
	, mRevisionGroupLabelItem (*this, kRevisionGroupLabelItem)
	, mDatesItem (*this, kDatesItem)
	, mStatusItem (*this, kStatusItem)
	, mRevisionsItem (*this, kRevisionsItem)
	, mEditorsItem (*this, kEditorsItem)
	, mRevisionGroupItem (*this, kRevisionGroupItem, &mRevisionGroupLabelItem)

	{ // begin CVSHistoryOptionsDialog
	
	} // end CVSHistoryOptionsDialog
	
// ---------------------------------------------------------------------------
//		€ ~CVSHistoryOptionsDialog
// ---------------------------------------------------------------------------

CVSHistoryOptionsDialog::~CVSHistoryOptionsDialog (void)

	{ // begin ~CVSHistoryOptionsDialog
	
	} // end ~CVSHistoryOptionsDialog

// ---------------------------------------------------------------------------
//		€ DoDialog
// ---------------------------------------------------------------------------

OSErr 
CVSHistoryOptionsDialog::DoDialog (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	const	AEDescList&		inDefaults,
	AEDescList&				outOptions)
	
	{ // begin DoDialog
		
		OSErr	e = noErr;

		VCSDialog::SetParamText (inFile.name);
		CVSHistoryOptionsDialog	d (inPB, kResourceID);
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
CVSHistoryOptionsDialog::GetOptions (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	AEDescList&				outOptions)
	
	{ // begin GetOptions
		
		OSErr				e = noErr;
		
		//	Get the defaults
		AEDescList			defaultList = {typeNull, nil};
		if (noErr != (e = ::AECreateList (nil, 0 , false, &defaultList))) return e;
		if (noErr != (e = ::VCSGetDefaultOptionsList (inPB, kResourceID, kResourceID, defaultList))) return e;

		UInt16				infoIndex = VCSGetHistoryInfoDefault (inPB);
		Str255				info = {0};
		::GetIndString (info, kResourceID, kInfoBaseIndex + infoIndex);
		if (info[0] && (noErr != (e = ::CVSAddPStringArg (&defaultList, info)))) goto CleanUp;
				
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
//		€ VCSHistory
// ---------------------------------------------------------------------------

VCSHistory::VCSHistory (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, true, true, true)
	
	{ // begin VCSHistory
		
	} // end VCSHistory

// ---------------------------------------------------------------------------
//		€ ~VCSHistory
// ---------------------------------------------------------------------------

VCSHistory::~VCSHistory (void)
	
	{ // begin ~VCSHistory
		
	} // end ~VCSHistory

// ---------------------------------------------------------------------------
//		€ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSHistory::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFile
				
		OSErr				e = noErr;
		
		
		//	Stuff to clean up
		StAEDesc 			command;
		StHandle			output;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 			task (mContext, kTaskStrings, kHistoryTask, inItem.fsItem.name);

		//	Get the cwd for checkout
		FSSpec 				cwd = inItem.fsItem;
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	cvs log <filename>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "log"))) goto CleanUp;

		//	Get the options.
		switch (e = CVSHistoryOptionsDialog::GetOptions (mContext, inItem.fsItem, command)) {
			case userCanceledErr:
				return inItem.eItemStatus = cwItemStatusCancelled;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) return inItem.eItemStatus;
				break;
			} // switch
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
		if (noErr != (e = VCSCheckCmdOutput (mContext, "\plog", output))) goto CleanUp;

		Str255				title = {0};
		AppendPString (AppendPString (title, inItem.fsItem.name), "\p.history");
		mContext.CreateDocument (p2cstr (title), output);
			
		inItem.eItemStatus = cwItemStatusSucceeded;
		
	CleanUp:
	
		return inItem.eItemStatus;
	
	} // end ProcessRegularFile

// ---------------------------------------------------------------------------
//		€ ProcessRegularFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSHistory::ProcessRegularFolder (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFolder
				
		OSErr				e = noErr;
		FSSpec 				cwd = inItem.fsItem;
		StAEDesc 			command;
		StHandle			output;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 			task (mContext, kTaskStrings, kHistoryTask, inItem.fsItem.name);

		//	cvs log <options>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "log"))) goto CleanUp;

		//	Get the options.
		switch (e = CVSHistoryOptionsDialog::GetOptions (mContext, inItem.fsItem, command)) {
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

		//	Display the results
		if (noErr != (e = VCSCheckCmdOutput (mContext, "\plog", output))) goto CleanUp;

		Str255				title = {0};
		AppendPString (AppendPString (title, inItem.fsItem.name), "\p.history");
		mContext.CreateDocument (p2cstr (title), output);
			
		inItem.eItemStatus = cwItemStatusSucceeded;
		
	CleanUp:
	
		return inItem.eItemStatus;
	
	} // end ProcessRegularFolder

