#include "VCSDifference.h"

#include "VCSError.h"
#include "VCSResult.h"
#include "VCSTask.h"
#include "VCSUtil.h"
#include "VCSVersion.h"

#include "CVSCommand.h"

#include "VCSAdvancedOptionsDialog.h"
#include "VCSDialogCheckboxItem.h"
#include "VCSDialogGroupBox.h"
#include "VCSDialogPopupItem.h"
#include "VCSDialogTextItem.h"

#include <TextUtils.h>

//	=== Types ===

class CVSDifferenceOptionsDialog : public VCSAdvancedOptionsDialog 

	{

		VCSDialogPopupItem		mKeywordsItem;
		VCSDialogPopupItem		mCompareToItem;
		VCSDialogTextItem		mCompareToTextItem;
		VCSDialogPopupItem		mCompareWithItem;
		VCSDialogTextItem		mCompareWithTextItem;
		VCSDialogItem			mSourceGroupLabel;
		VCSDialogGroupBox		mSourceGroupBox;
		
		VCSDialogCheckboxItem	mContextualItem;
		VCSDialogItem			mCompareGroupLabel;
		VCSDialogGroupBox		mCompareGroupBox;
	
	protected:
	
		//	Modal dialog handling
		virtual	Boolean OnItemHit 					(DialogItemIndex		inItemHit);

	public:
		
		enum {
			kPromptItem = cancel + 1,
			kSourceGroupLabelItem,
			kKeywordsItem,
			kCompareToItem,
			kCompareToTextItem,
			kCompareWithItem,
			kCompareWithTextItem,
			kSourceGroupItem,
			
			kCompareGroupLabelItem,
			kContextualItem,
			kCompareGroupItem,
			
			kResourceID = 16260
			};
	
						CVSDifferenceOptionsDialog	(const VCSContext&		mContext,
													 short					inDLOGid);
		virtual			~CVSDifferenceOptionsDialog	(void);
		
		Boolean 		Validate 					(void) const;
		
		static	OSErr	DoDialog					(const VCSContext&		mContext,
													 const	FSSpec&			inFile,
													 const	AEDescList&		inDefaults,
													 AEDescList&			outOptions);
		static	OSErr 	GetOptions					(const 	VCSContext&		inContext,
													 const	FSSpec&			inFile,
													 AEDescList&			outOptions);
	};
	
// ---------------------------------------------------------------------------
//		• CVSDifferenceOptionsDialog
// ---------------------------------------------------------------------------

CVSDifferenceOptionsDialog::CVSDifferenceOptionsDialog (

	const VCSContext&		inPB,
	short					inDLOGid)
	
	: VCSAdvancedOptionsDialog (inPB, inDLOGid)
	
	, mKeywordsItem (*this, kKeywordsItem)
	, mCompareToItem (*this, kCompareToItem)
	, mCompareToTextItem (*this, kCompareToTextItem)
	, mCompareWithItem (*this, kCompareWithItem)
	, mCompareWithTextItem (*this, kCompareWithTextItem)
	, mSourceGroupLabel (*this, kSourceGroupLabelItem)
	, mSourceGroupBox (*this, kSourceGroupItem, &mSourceGroupLabel)
	
	, mContextualItem (*this, kContextualItem)
	, mCompareGroupLabel (*this, kCompareGroupLabelItem)
	, mCompareGroupBox (*this, kCompareGroupItem, &mCompareGroupLabel)

	{ // begin CVSDifferenceOptionsDialog
		
	} // end CVSDifferenceOptionsDialog
	
// ---------------------------------------------------------------------------
//		• ~CVSDifferenceOptionsDialog
// ---------------------------------------------------------------------------

CVSDifferenceOptionsDialog::~CVSDifferenceOptionsDialog (void)

	{ // begin ~CVSDifferenceOptionsDialog
	
	} // end ~CVSDifferenceOptionsDialog

// ---------------------------------------------------------------------------
//		• Validate
// ---------------------------------------------------------------------------

Boolean 
CVSDifferenceOptionsDialog::Validate (void) const
	
	{ // begin Validate
		
		Str255	text;

		if (mCompareToItem.GetValue () == 1) return true;
		mCompareToTextItem.GetDescriptor (text);
		if (0 == text[0]) return false;
		
		if (mCompareWithItem.GetValue () == 1) return true;
		mCompareWithTextItem.GetDescriptor (text);
		if (0 == text[0]) return false;
		
		return true;
		
	} // end Validate
	
// ---------------------------------------------------------------------------
//		• OnItemHit
// ---------------------------------------------------------------------------

Boolean 
CVSDifferenceOptionsDialog::OnItemHit (

	short	inItemHit)
	
	{ // begin OnItemHit
		
		Str31	blank = {0};
		Boolean	compareToActive = (mCompareToItem.GetValue () > 1);
		Boolean	compareWithActive = (mCompareWithItem.GetValue () > 1);
		
		switch (inItemHit) {
			case kFirstTimeItem:
			case kCompareToItem:
				//	Text field
				mCompareToTextItem.SetEnable (compareToActive);
				mCompareToTextItem.SetShow (compareToActive);
				mCompareWithItem.SetEnable (compareToActive);
				mCompareWithItem.SetShow (compareToActive);
				if (!compareToActive) {
					mCompareToTextItem.SetDescriptor (blank);
					mCompareWithItem.SetValue (1);
					compareWithActive = false;
					} // if
				//	Fall through…
			
			case kCompareWithItem:
				mCompareWithTextItem.SetEnable (compareToActive && compareWithActive);
				mCompareWithTextItem.SetShow (compareToActive && compareWithActive);
				if (!compareWithActive) {
					mCompareWithTextItem.SetDescriptor (blank);
					} // if
				break;
			} // switch
		
		SetItemEnable (ok, Validate ());
			
		return VCSAdvancedOptionsDialog::OnItemHit (inItemHit);
		
	} // end OnItemHit
	
// ---------------------------------------------------------------------------
//		• DoDialog
// ---------------------------------------------------------------------------

OSErr 
CVSDifferenceOptionsDialog::DoDialog (
	
	const VCSContext&		inPB,
	const	FSSpec&			inFile,
	const	AEDescList&		inDefaults,
	AEDescList&				outOptions)
	
	{ // begin DoDialog
		
		OSErr				e = noErr;

		VCSDialog::SetParamText (inFile.name);
		CVSDifferenceOptionsDialog	d (inPB, kResourceID);
		if (noErr != (e = d.SetOptionsList (inDefaults, kResourceID))) goto CleanUp;
		if (ok != d.DoModalDialog ()) return userCanceledErr;
		
		if (noErr != (e = d.GetOptionsList (outOptions, kResourceID))) goto CleanUp;

	CleanUp:
	
		return e;
	
	} // end DoDialog

// ---------------------------------------------------------------------------
//		• GetOptions
// ---------------------------------------------------------------------------

OSErr 
CVSDifferenceOptionsDialog::GetOptions (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	AEDescList&				outOptions)
	
	{ // begin GetOptions
		
		OSErr				e = noErr;
		
		//	Get the defaults
		AEDescList			defaultList = {typeNull, nil};
		if (noErr != (e = ::AECreateList (nil, 0 , false, &defaultList))) return e;
		if (noErr != (e = ::CVSAddCStringArg (&defaultList, "-c"))) goto CleanUp;

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
//		• VCSDifference
// ---------------------------------------------------------------------------

VCSDifference::VCSDifference (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, true, true, true)
		
	{ // begin VCSDifference
		
	} // end VCSDifference

// ---------------------------------------------------------------------------
//		• ~VCSDifference
// ---------------------------------------------------------------------------

VCSDifference::~VCSDifference (void)
	
	{ // begin ~VCSDifference
		
	} // end ~VCSDifference

// ---------------------------------------------------------------------------
//		• ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSDifference::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFile
		
		OSErr				e = noErr;
		
		Str255				title;
		FSSpec				cwd = inItem.fsItem;
		
		//	Stuff to clean up
		AEDesc 				command = {typeNull, nil};
		Handle				output = nil;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 			task (mContext, kTaskStrings, kDifferenceTask, inItem.fsItem.name);

		//	Get the cwd for update
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		//	cvs diff <options> <filename>
		if (noErr != VCSRaiseOSErr (mContext, ::CVSCreateCommand (&command, "diff"))) goto CleanUp;

		//	Get the options.
		switch (e = CVSDifferenceOptionsDialog::GetOptions (mContext, inItem.fsItem, command)) {
			case userCanceledErr:
				inItem.eItemStatus = cwItemStatusCancelled;
				goto CleanUp;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) goto CleanUp;
				break;
			} // switch
		
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, VCSSendOutputCommand (mContext, &command, &cwd, &output))) goto CleanUp;
		
		//	Send the command
		if (noErr != (e = VCSCheckCmdOutput (mContext, "\pdiff", output))) goto CleanUp;
		
		//	Display the results
		title[0] = 0; 
		AppendPString (AppendPString (title, inItem.fsItem.name), "\p.diff");
		mContext.CreateDocument (p2cstr (title), output);

		// Check the status
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);

	CleanUp:
	
		if (output != nil) DisposeHandle (output);
		output = nil;

		AEDisposeDesc (&command);
		
		return inItem.eItemStatus;
	
	} // end ProcessRegularFile

// ---------------------------------------------------------------------------
//		• ProcessRegularFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSDifference::ProcessRegularFolder (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFolder
		
		OSErr				e = noErr;
		
		Str255				title;
		FSSpec				cwd = inItem.fsItem;
		
		//	Stuff to clean up
		AEDesc 				command = {typeNull, nil};
		Handle				output = nil;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 			task (mContext, kTaskStrings, kDifferenceTask, inItem.fsItem.name);

		//	cvs diff <options>
		if (noErr != VCSRaiseOSErr (mContext, ::CVSCreateCommand (&command, "diff"))) goto CleanUp;

		//	Get the options.
		switch (e = CVSDifferenceOptionsDialog::GetOptions (mContext, inItem.fsItem, command)) {
			case userCanceledErr:
				inItem.eItemStatus = cwItemStatusCancelled;
				goto CleanUp;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) goto CleanUp;
			} // switch
		
		if (noErr != VCSRaiseOSErr (mContext, VCSSendOutputCommand (mContext, &command, &cwd, &output))) goto CleanUp;
		
		//	Send the command
		if (noErr != (e = VCSCheckCmdOutput (mContext, "\pdiff", output))) goto CleanUp;
		
		//	Display the results
		title[0] = 0; 
		AppendPString (AppendPString (title, inItem.fsItem.name), "\p.diff");
		mContext.CreateDocument (p2cstr (title), output);

		// Check the status
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);

	CleanUp:
	
		if (output != nil) DisposeHandle (output);
		output = nil;

		AEDisposeDesc (&command);
		
		return inItem.eItemStatus;
	
	} // end ProcessRegularFolder
