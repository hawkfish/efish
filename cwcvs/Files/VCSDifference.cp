#include "VCSDifference.h"

#include "VCSError.h"
#include "VCSGet.h"
#include "VCSPrefs.h"
#include "VCSResult.h"
#include "VCSTask.h"
#include "VCSUtil.h"
#include "VCSVersion.h"

#include "SSSignatureToApp.h"
#include "CVSCommand.h"
#include "StCurResFile.h"
#include "MoreFilesExtras.h"

#include "VCSAdvancedOptionsDialog.h"
#include "VCSDialogCheckboxItem.h"
#include "VCSDialogGroupBox.h"
#include "VCSDialogPopupItem.h"
#include "VCSDialogTextItem.h"

#include <Folders.h>
#include <TextUtils.h>

//	=== Types ===

#pragma mark CVSDifferenceOptionsDialog

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
//		¥ CVSDifferenceOptionsDialog
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
//		¥ ~CVSDifferenceOptionsDialog
// ---------------------------------------------------------------------------

CVSDifferenceOptionsDialog::~CVSDifferenceOptionsDialog (void)

	{ // begin ~CVSDifferenceOptionsDialog
	
	} // end ~CVSDifferenceOptionsDialog

// ---------------------------------------------------------------------------
//		¥ Validate
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
//		¥ OnItemHit
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
				//	Fall throughƒ
			
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
//		¥ DoDialog
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
//		¥ GetOptions
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
#pragma mark LocalDifferenceOptionsDialog

class LocalDifferenceOptionsDialog : public VCSAdvancedOptionsDialog 

	{

		VCSDialogPopupItem		mCompareToItem;
		VCSDialogTextItem		mCompareToTextItem;
	
	protected:
	
		//	Modal dialog handling
		virtual	Boolean OnItemHit 					(DialogItemIndex		inItemHit);

	public:
		
		enum {
			kPromptItem = cancel + 1,
			kCompareToItem,
			kCompareToTextItem,
			
			kResourceID = 16261
			};
	
						LocalDifferenceOptionsDialog	(const VCSContext&		mContext,
													 short					inDLOGid);
		virtual			~LocalDifferenceOptionsDialog	(void);
		
		Boolean 		Validate 					(void) const;
		
		static	OSErr	DoDialog					(const VCSContext&		mContext,
													 const	FSSpec&			inFile,
													 const	AEDescList&		inDefaults,
													 StringPtr				ioVersion,
													 AEDescList&			outOptions);
		static	OSErr 	GetOptions					(const 	VCSContext&		inContext,
													 const	FSSpec&			inFile,
													 StringPtr				ioVersion,
													 AEDescList&			outOptions);
	};
	
// ---------------------------------------------------------------------------
//		¥ LocalDifferenceOptionsDialog
// ---------------------------------------------------------------------------

LocalDifferenceOptionsDialog::LocalDifferenceOptionsDialog (

	const VCSContext&		inPB,
	short					inDLOGid)
	
	: VCSAdvancedOptionsDialog (inPB, inDLOGid)
	
	, mCompareToItem (*this, kCompareToItem)
	, mCompareToTextItem (*this, kCompareToTextItem)

	{ // begin LocalDifferenceOptionsDialog
		
	} // end LocalDifferenceOptionsDialog
	
// ---------------------------------------------------------------------------
//		¥ ~LocalDifferenceOptionsDialog
// ---------------------------------------------------------------------------

LocalDifferenceOptionsDialog::~LocalDifferenceOptionsDialog (void)

	{ // begin ~LocalDifferenceOptionsDialog
	
	} // end ~LocalDifferenceOptionsDialog

// ---------------------------------------------------------------------------
//		¥ Validate
// ---------------------------------------------------------------------------

Boolean 
LocalDifferenceOptionsDialog::Validate (void) const
	
	{ // begin Validate
		
		Str255	text;

		if (mCompareToItem.GetValue () == 1) return true;
		mCompareToTextItem.GetDescriptor (text);
		if (0 == text[0]) return false;
		
		return true;
		
	} // end Validate
	
// ---------------------------------------------------------------------------
//		¥ OnItemHit
// ---------------------------------------------------------------------------

Boolean 
LocalDifferenceOptionsDialog::OnItemHit (

	short	inItemHit)
	
	{ // begin OnItemHit
		
		Str31	blank = {0};
		Boolean	compareToActive = (mCompareToItem.GetValue () > 1);
		
		switch (inItemHit) {
			case kFirstTimeItem:
			case kCompareToItem:
				//	Text field
				mCompareToTextItem.SetEnable (compareToActive);
				mCompareToTextItem.SetShow (compareToActive);
				if (!compareToActive) mCompareToTextItem.SetDescriptor (blank);
				break;
			} // switch
		
		SetItemEnable (ok, Validate ());
			
		return VCSAdvancedOptionsDialog::OnItemHit (inItemHit);
		
	} // end OnItemHit
	
// ---------------------------------------------------------------------------
//		¥ DoDialog
// ---------------------------------------------------------------------------

OSErr 
LocalDifferenceOptionsDialog::DoDialog (
	
	const VCSContext&		inPB,
	const	FSSpec&			inFile,
	const	AEDescList&		inDefaults,
	StringPtr				ioVersion,
	AEDescList&				outOptions)
	
	{ // begin DoDialog
		
		OSErr				e = noErr;

		do {
			VCSDialog::SetParamText (inFile.name);
			LocalDifferenceOptionsDialog	d (inPB, kResourceID);
			if (noErr != (e = d.SetOptionsList (inDefaults, kResourceID))) break;
			d.mCompareToTextItem.SetDescriptor (ioVersion);
			
			if (ok != d.DoModalDialog ()) {
				e = userCanceledErr;
				break;
				} // if
				
			if (noErr != (e = d.GetOptionsList (outOptions, kResourceID))) break;
			d.mCompareToTextItem.GetDescriptor (ioVersion);
			} while (false);
			
		return e;
	
	} // end DoDialog

// ---------------------------------------------------------------------------
//		¥ GetOptions
// ---------------------------------------------------------------------------

OSErr 
LocalDifferenceOptionsDialog::GetOptions (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	StringPtr				ioVersion,
	AEDescList&				outOptions)
	
	{ // begin GetOptions
		
		OSErr				e = noErr;
		
		AEDescList			defaultList = {typeNull, nil};

		do {
			//	Get the defaults
			if (noErr != (e = ::AECreateList (nil, 0 , false, &defaultList))) break;
			if (noErr != (e = ::CVSAddCStringArg (&defaultList, "-r"))) break;
			if (noErr != (e = ::CVSAddPStringArg (&defaultList, ioVersion))) break;

			//	If not advanced, just use the defaults 
			if (!inPB.Advanced ()) {
				if (noErr != (e = ::CVSAddListArgs (&outOptions, &defaultList))) break;
				} // if
				
			else {
				if (noErr != (e = DoDialog (inPB, inFile, defaultList, ioVersion, outOptions))) break;
				} // else
			} while (false);
		
		::AEDisposeDesc (&defaultList);
		
		return e;
	
	} // end GetOptions

#pragma mark -

//	=== Types ===

#pragma options align=mac68k
typedef struct CompareTypeMapping {
	OSType	osType;
	OSType	compareType;
	}	CompareTypeRec,
		*CompareTypePtr,
		**CompareTypeHandle;
#pragma options align=reset

#include <string.h>

// ---------------------------------------------------------------------------
//		¥ FSpSetFLock
// ---------------------------------------------------------------------------

static OSErr
FSpSetFLock (

	const	FSSpec*	a,
	Boolean			l)
	
	{ // begin FSpSetFLock
		
		return l ? FSpSetFLock (a) : FSpRstFLock (a);
		
	} // end FSpSetFLock
	
// ---------------------------------------------------------------------------
//		¥ VCSDifference
// ---------------------------------------------------------------------------

VCSDifference::VCSDifference (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, true, true, true)
		
	, mCompareID (kNoIDEDiffResourceID)

	{ // begin VCSDifference
		
		StCurResFile	saveResFile;
		
		//	Get the owner resource
		Handle			cwOwner = ::GetIndResource ('CWIE', 1);
		if (cwOwner == nil) return;
		
		//	Get the IDE res file
		ResFileRefNum	cwResFile = ::HomeResFile (cwOwner);
		::UseResFile (cwResFile);
		
		//	Get the IDE vers resource
		Handle	versHandle = ::Get1Resource ('vers', 1);
		if (versHandle == nil) return;
		
		//	Get the IDE version
		VersRecHndl			vers = (VersRecHndl) versHandle;
		NumVersionVariant	cwVersion = {(**vers).numericVersion};
		
		//	Make sure it is high enough
		if (cwVersion.whole < 0x03000000) return;
		
		//	Use recent IDE
		mCompareID = kIDEDiffResourceID;
		
	} // end VCSDifference

// ---------------------------------------------------------------------------
//		¥ ~VCSDifference
// ---------------------------------------------------------------------------

VCSDifference::~VCSDifference (void)
	
	{ // begin ~VCSDifference
		
	} // end ~VCSDifference

// ---------------------------------------------------------------------------
//		¥ GetFileCompareType
// ---------------------------------------------------------------------------

OSType 
VCSDifference::GetFileCompareType (
	
	const	OSType		inType) const
	
	{ // begin GetFileCompareType
		
		CompareTypeHandle	typeList = (CompareTypeHandle) ::GetResource ('CTM#', mCompareID);
		if (!typeList || !*typeList) return kTypeUnknown;
		
		Size		typeCount = ::GetHandleSize ((Handle) typeList) / sizeof (**typeList);
		for (Size i = 0; i < typeCount; ++i) {
			if ((*typeList)[i].osType == inType) return (*typeList)[i].compareType;
			} // for
	
		return kTypeUnknown;
		
	} // end GetFileCompareType
	
// ---------------------------------------------------------------------------
//		¥ GetFileCompareType
// ---------------------------------------------------------------------------

OSType 
VCSDifference::GetFileCompareType (
	
	const	FSSpec&		inItem) const
	
	{ // begin GetFileCompareType
		
		FInfo	fInfo;
		if (noErr != VCSRaiseOSErr (mContext, ::FSpGetFInfo (&inItem, &fInfo))) return kTypeError;
		
		return GetFileCompareType (fInfo.fdType);
	
	} // end GetFileCompareType
	
// ---------------------------------------------------------------------------
//		¥ CompareIDEFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSDifference::CompareIDEFile (
	
	CWVCSItem& 			inItem,
	const	FSSpec&		inOldItem,
	ConstStr255Param	inVersionName)
	
	{ // begin CompareIDEFile
	
		short			fRefNum = -1;
		Handle			contents = nil;
		char*			cVersionName (::p2cstr ((StringPtr) inVersionName));
		
		//	Read in the file contents
		if (noErr != VCSRaiseOSErr (mContext, ::FSpOpenDF (&inOldItem, fsRdPerm,  &fRefNum))) goto CleanUp;
		
		long	logEOF = 0;
		if (noErr != VCSRaiseOSErr (mContext, ::GetEOF (fRefNum, &logEOF))) goto CleanUp;
		
		contents = ::NewHandle (logEOF);
		if (noErr != VCSRaiseOSErr (mContext, ::MemError ())) goto CleanUp;
		::MoveHHi (contents);
		::HLock (contents);
		
		long	count = logEOF;
		if (noErr != VCSRaiseOSErr (mContext, ::FSRead (fRefNum, &count, *contents))) goto CleanUp;

		//	Display the result
		if (cwNoErr != mContext.VisualDifference (cVersionName, *contents, count, inItem.fsItem)) {
			VCSRaiseString (mContext, kErrorStrings, kCantDifferenceAnyErr, inItem.fsItem.name);
			goto CleanUp;
			} // if
			
		// Check the status
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
		
	CleanUp:
	
		if (contents != nil) ::DisposeHandle (contents);
		contents = nil;
		
		if (fRefNum != -1) ::FSClose (fRefNum);
		fRefNum = -1;
		
		::c2pstr (cVersionName);
		
		return inItem.eItemStatus;
	
	} // end CompareIDEFile
	
// ---------------------------------------------------------------------------
//		¥ CompareFile
// ---------------------------------------------------------------------------

//	Resourcerer's Compare suite
const	DescType	kAECompareClass = 'Comp';
const	DescType	kAECompareEvent = 'Comp';
	const	DescType	keyNewer		= 'kNew';
	const	DescType	keyOlder		= 'kOld';
	const	DescType	keyShowDiff		= 'kShD';
	const	DescType	keyReturnDiff	= 'kRtD';
		
CWVCSItemStatus 
VCSDifference::CompareFile (
	
	CWVCSItem& 			inItem,
	OSType				inCreator,
	const	FSSpec&		inOldItem,
	ConstStr255Param	inVersionName)
	
	{ // begin CompareFile
		
		//	Local data
		ProcessSerialNumber		psn;
		FSSpec					olderItem = inOldItem;
		Boolean					inShowDiffs = true;
		
		//	AE data
		AEIdleUPP				idleProc = nil;

		AEDesc					newerDesc = {typeNull, nil};
		AEDesc					olderDesc = {typeNull, nil};
		AEDesc					showDesc = {typeNull, nil};

		AEDesc					addressDesc = {typeNull, nil};
		AppleEvent				sendEvent = {typeNull, nil};
		AppleEvent				replyEvent = {typeNull, nil};
		
		//	Check for the IDE
		if (inCreator == kTypeIDE) return CompareIDEFile (inItem, inOldItem, inVersionName);
		
		do {
			//	Rename the file with its version (if possible)
			if (noErr == ::FSpRename (&olderItem, inVersionName)) 
				::BlockMoveData (inVersionName + 1, olderItem.name + 1, olderItem.name[0] = inVersionName[0]);
			
			//	Launch Resourcerer
			{
				Str15				creatorText;
				FSSpec				appSpec;
				::BlockMoveData (&inCreator, creatorText + 1, creatorText[0] = sizeof (inCreator));
				VCSTask 			task (mContext, kTaskStrings, kFindingHelperTask, creatorText);
				if (noErr != VCSRaiseOSErr (mContext, ::SignatureToApp (inCreator, 0, &psn, &appSpec, nil, Sig2App_LaunchApplication, launchContinue + launchDontSwitch))) break;
			}
			
			//	Create the parameters
			if (noErr != VCSRaiseOSErr (mContext, ::AECreateDesc (typeFSS, &inItem.fsItem, sizeof (inItem.fsItem), &newerDesc))) break;
			if (noErr != VCSRaiseOSErr (mContext, ::AECreateDesc (typeFSS, &olderItem, sizeof (olderItem), &olderDesc))) break;
			if (noErr != VCSRaiseOSErr (mContext, ::AECreateDesc (typeBoolean, &inShowDiffs, sizeof (inShowDiffs), &showDesc))) break;
			
			//	Create the event
			if (noErr != VCSRaiseOSErr (mContext, ::AECreateDesc (typeProcessSerialNumber, &psn, sizeof (psn), &addressDesc))) break;
			if (noErr != VCSRaiseOSErr (mContext, ::AECreateAppleEvent (kAECompareClass, kAECompareEvent, &addressDesc, kAutoGenerateReturnID, kAnyTransactionID, &sendEvent))) break;
			
			//	Add the parameters
			if (noErr != VCSRaiseOSErr (mContext, ::AEPutParamDesc (&sendEvent, keyNewer, &newerDesc))) break;
			if (noErr != VCSRaiseOSErr (mContext, ::AEPutParamDesc (&sendEvent, keyOlder, &olderDesc))) break;
			if (noErr != VCSRaiseOSErr (mContext, ::AEPutParamDesc (&sendEvent, keyShowDiff, &showDesc))) break;
			
			//	Send the event
			idleProc = mContext.BeginAEIdle ();
			if (noErr != VCSRaiseOSErr (mContext, ::AESend (&sendEvent, &replyEvent, kAENoReply, kAENormalPriority, kNoTimeOut, idleProc, nil))) break;
			
			//	If this is the only file, then activate the application
			if (1 == mContext.GetItemCount ()) ::SetFrontProcess (&psn);
			
			//	Update the status
			inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
			} while (false);
			
		::AEDisposeDesc (&replyEvent);
		::AEDisposeDesc (&sendEvent);
		::AEDisposeDesc (&addressDesc);

		::AEDisposeDesc (&showDesc);
		::AEDisposeDesc (&olderDesc);
		::AEDisposeDesc (&newerDesc);
		
		mContext.EndAEIdle (idleProc);

		return inItem.eItemStatus;
	
	} // end CompareFile

// ---------------------------------------------------------------------------
//		¥ IterateFile
// ---------------------------------------------------------------------------

void 
VCSDifference::IterateFile (

	const CInfoPBRec& 	cpb,
	long				dirID,
	Boolean&			quitFlag)

	{ // begin IterateFile
		
		VCSTask 	task (mContext, kTaskStrings, kExaminingFileTask, cpb.hFileInfo.ioNamePtr);
		
		if (kTypeUnknown == GetFileCompareType (cpb.hFileInfo.ioFlFndrInfo.fdType)) return;
		
		OSErr		e = noErr;
		CWVCSItem	item;
		if (noErr != (e = ::FSMakeFSSpec (cpb.hFileInfo.ioVRefNum, dirID, cpb.hFileInfo.ioNamePtr, &item.fsItem))) return;
		
		//	Is the file locked?
		if (cpb.hFileInfo.ioFlAttrib & 0x01) return;

		//	Unlocked, so check its status
		item.eItemStatus = VCSVersion (mContext).GetCheckoutState (&item.fsItem);
		switch (item.eItemStatus) {
			case cwCheckoutStateNotInDatabase:
			case cwCheckoutStateNotCheckedOut:
				return;
			} // switch

		VCSFileCommand::IterateFile (cpb, dirID, quitFlag);

	} // end IterateFile
	
#pragma mark -

// ---------------------------------------------------------------------------
//		¥ GetBaseVersion
// ---------------------------------------------------------------------------

Boolean
VCSDifference::GetBaseVersion (

	StringPtr		outVersion,
	const	FSSpec&	inSpec)
	
	{ // begin GetBaseVersion
	
		Handle				versionText = nil;
		
		outVersion[0] = 0;
		
		do {
			//	Get the revision number
			if (noErr != VCSRaiseOSErr (mContext, VCSVersion::ParseEntriesFile (&inSpec, 0, &versionText))) break;
			if (!versionText || (**versionText == '-')) break;
			
			::BlockMoveData (*versionText, outVersion + 1, outVersion[0] = GetHandleSize (versionText));
			} while (false);

		if (versionText != nil) DisposeHandle (versionText);
		versionText = nil;
				
		return 0 != outVersion[0];
		
	} // end GetBaseVersion
	
// ---------------------------------------------------------------------------
//		¥ GetVersionFileFromUpdate
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSDifference::GetVersionFileFromUpdate (
	
	FSSpec&				outVersionSpec,
	const	FSSpec&		inSpec,
	ConstStr255Param	/*inVersion*/)	

	{ // begin GetVersionFileFromUpdate
		
		CWVCSItemStatus		result = cwItemStatusFailed;
		
		FSSpec				cwd = inSpec;
		
		//	Stuff to clean up
		AEDesc 				command = {typeNull, nil};
		
		do {
			//	Find the file location
			if (noErr != VCSRaiseOSErr (mContext, ::FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) break;

			//	Find the temp item location.
			outVersionSpec = inSpec;
			if (noErr != VCSRaiseOSErr (mContext, ::FindFolder (outVersionSpec.vRefNum, kTemporaryFolderType, kCreateFolder, &outVersionSpec.vRefNum,  &outVersionSpec.parID))) break;
			::FSpRstFLock (&outVersionSpec);
			::FSpDelete (&outVersionSpec);
			
			//	Find the temp item folder.
			FSSpec				tempDir = outVersionSpec;
			if (noErr != VCSRaiseOSErr (mContext, ::FSMakeFSSpec (outVersionSpec.vRefNum, outVersionSpec.parID, nil, &tempDir))) break;
			
			//	Move the user's version to the temp folder
			Boolean	fileLocked = (fLckdErr == FSpCheckObjectLock (&inSpec));
			if (noErr != VCSRaiseOSErr (mContext, ::FSpCatMove (&inSpec, &tempDir))) break;
			
			do {
				//	cvs -rq update <options> <file>
				if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-rq"))) break;
				if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "update"))) break;

				//	Get the options.

				//	Add the file.
				if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inSpec.name))) break;

				//	Send the command to MacCVS
				switch (VCSRaiseOSErr (mContext, VCSSendCommand (mContext, &command, &cwd))) {
					case noErr:
						result = cwItemStatusSucceeded;
						break;
						
					case userCanceledErr:
						result = cwItemStatusCancelled;
						
					default:
						break;
					} // switch
				} while (false);
				
			//	Swap them back
			Boolean	tempLocked = (fLckdErr == FSpCheckObjectLock (&inSpec));
			if (noErr != VCSRaiseOSErr (mContext, ::FSpExchangeFiles (&inSpec, &outVersionSpec))) {
				//	Uh oh, swap failed, try to put it back...
				VCSRaiseOSErr (mContext, ::FSpCatMove (&outVersionSpec, &cwd));
				break;
				} // if
			
			//	Swap the locks as well
			FSpSetFLock (&inSpec,			fileLocked);
			FSpSetFLock (&outVersionSpec, 	tempLocked);
			} while (false);
		
		AEDisposeDesc (&command);
		
		return result;
		
	} // end GetVersionFileFromUpdate
	
// ---------------------------------------------------------------------------
//		¥ GetVersionFileFromCheckout
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSDifference::GetVersionFileFromCheckout (
	
	FSSpec&				outVersionSpec,
	const	FSSpec&		inSpec,
	StringPtr			ioVersion)	

	{ // begin GetVersionFileFromCheckout
		
		CWVCSItemStatus	result = cwItemStatusFailed;

		static	char	null = 0;
		static	char	colon = ':';
		static	char	slash = '/';
		
		OSErr			e = noErr;
		FSSpec			cwd;
		FSSpec			cvsSpec;
		FSSpec			coDir;
		
		//	Stuff to clean up
		AEDesc 			command = {typeNull, nil};
		Handle			contents = nil;
		Handle			root = nil;
		Handle			module = nil;
		
		do {
			//	Get the root path
			if (noErr != ::FSMakeFSSpec (inSpec.vRefNum, inSpec.parID, "\p:CVS:Root", &cvsSpec)) break;
			if (noErr != VCSRaiseOSErr (mContext, ::ReadFileContents (&contents, &cvsSpec))) break;
			if (noErr != VCSRaiseOSErr (mContext, ::GetNextLine (&root, contents))) break;
			DisposeHandle (contents);
			contents = nil;
			
			if (noErr != VCSRaiseOSErr (mContext, ::PtrAndHand (&null, root, sizeof (null)))) break;
			MoveHHi (root);
			HLock (root);
			char*	cvspath = std::strrchr (*root, colon) + 1;
			
			//	Get the repository path
			if (noErr != VCSRaiseOSErr (mContext, ::FSMakeFSSpec (inSpec.vRefNum, inSpec.parID, "\p:CVS:Repository", &cvsSpec))) break;
			if (noErr != VCSRaiseOSErr (mContext, ::ReadFileContents (&contents, &cvsSpec))) break;
			if (noErr != VCSRaiseOSErr (mContext, ::GetNextLine (&module, contents))) break;
			DisposeHandle (contents);
			contents = nil;
			
			//	Create the module name
			::Munger (module, 0, cvspath, std::strlen (cvspath), cvspath, 0);
			if (**module == slash) Munger (module, 0, &slash, sizeof (slash), &slash, 0);
			if (noErr != VCSRaiseOSErr (mContext, ::PtrAndHand (&slash, module, sizeof (slash)))) break;
			if (noErr != VCSRaiseOSErr (mContext, ::PtrAndHand (inSpec.name + 1, module, inSpec.name[0]))) break;
			if (noErr != VCSRaiseOSErr (mContext, ::PtrAndHand (&null, module, sizeof (null)))) break;
			HLock (module);
			
			//	Find the temp item location.
			outVersionSpec = inSpec;
			if (noErr != VCSRaiseOSErr (mContext, ::FindFolder (inSpec.vRefNum, kTemporaryFolderType, kCreateFolder, &outVersionSpec.vRefNum,  &outVersionSpec.parID))) break;
			::FSpRstFLock (&outVersionSpec);
			::FSpDelete (&outVersionSpec);

			//	Find the checkout directory
			coDir = outVersionSpec;
			::BlockMoveData (*module, coDir.name + 1, coDir.name[0] = (std::strchr (*module, slash) - *module));
			DeleteDirectory (coDir.vRefNum, coDir.parID, coDir.name);
			
			//	Set the cwd to the temp items folder
			if (noErr != VCSRaiseOSErr (mContext, ::FSMakeFSSpec (outVersionSpec.vRefNum, outVersionSpec.parID, nil, &cwd))) break;
			
			//	cvs checkout [{-r <rev>} | {-D <date>}] <module>
			if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "checkout"))) break;

			//	Get the options.
			switch (e = LocalDifferenceOptionsDialog::GetOptions (mContext, inSpec, ioVersion, command)) {
				case userCanceledErr:
					result = cwItemStatusCancelled;
					break;
				
				default:
					if (noErr == VCSRaiseOSErr (mContext, e)) result = cwItemStatusSucceeded;
					break;
				} // switch
			if (cwItemStatusSucceeded != result) break;

			if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, *module))) break;

			//	Send the command to MacCVS
			switch (VCSRaiseOSErr (mContext, VCSSendCommand (mContext, &command, &cwd))) {
				case noErr:
					result = cwItemStatusSucceeded;
					break;
					
				case userCanceledErr:
					result = cwItemStatusCancelled;
					
				default:
					break;
				} // switch
			if (cwItemStatusSucceeded != result) break;
			
			//	Dig out the file
			cvsSpec = cwd;
			for (char* p = *module;;) {
				//	Get the DirID
				long	dirID;
				Boolean	isDir;
				if (noErr != VCSRaiseOSErr (mContext, ::FSpGetDirectoryID (&cvsSpec, &dirID, &isDir))) break;
				cvsSpec.parID = dirID;
				
				//	Get the name
				char*	pSlash = std::strchr (p, slash);
				if (!pSlash) {
					::BlockMoveData (p, cvsSpec.name + 1, cvsSpec.name[0] = std::strlen (p));
					break;
					} // if
				
				::BlockMoveData (p, cvsSpec.name + 1, cvsSpec.name[0] = (pSlash - p));
				p = pSlash + 1;
				} // for
			
			//	Move it to the top
			::FSpRstFLock (&cvsSpec);
			if (noErr != VCSRaiseOSErr (mContext, ::FSpCatMove (&cvsSpec, &cwd))) break;
			
			//	Delete the checkout junque
			DeleteDirectory (coDir.vRefNum, coDir.parID, coDir.name);
			} while (false);
		
		AEDisposeDesc (&command);

		if (module) DisposeHandle (module);
		module = nil;
		
		if (root) DisposeHandle (root);
		root = nil;
		
		if (contents) DisposeHandle (contents);
		contents = nil;
		
		return result;
		
	} // end GetVersionFileFromCheckout
	
// ---------------------------------------------------------------------------
//		¥ GetVersionFile
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSDifference::GetVersionFile (
	
	FSSpec&				outVersionSpec,
	const	FSSpec&		inSpec,
	StringPtr			ioVersion)	

	{ // begin GetVersionFile
		
		return GetVersionFileFromCheckout (outVersionSpec, inSpec, ioVersion);
		
	} // end GetVersionFile
	
// ---------------------------------------------------------------------------
//		¥ MacProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSDifference::MacProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin MacProcessRegularFile
		
		OSErr				e = noErr;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 			task (mContext, kTaskStrings, kDifferenceTask, inItem.fsItem.name);

		//	Make sure it is a file we can compare
		OSType			compareType = GetFileCompareType (inItem.fsItem);
		switch (compareType) {
			case kTypeError:
				goto CleanUp;
				
			case kTypeUnknown:
				VCSRaiseString (mContext, kErrorStrings, kCantDifferenceTextErr, inItem.fsItem.name);
				goto CleanUp;
				
			case kTypeIgnore:
				inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
				goto CleanUp;
			} // switch
		
		//	Get the compare version
		Str255		versionStr;
		if (!GetBaseVersion (versionStr, inItem.fsItem)) goto CleanUp;
		
		//	Get the temp file
		FSSpec				tempItem;
		inItem.eItemStatus = GetVersionFile (tempItem, inItem.fsItem, versionStr);
		if (cwItemStatusSucceeded != inItem.eItemStatus) goto CleanUp;
		
		//	Compare the results
		Str255	versionName;
		versionName[0] = 0;
		::AppendPString (versionName, inItem.fsItem.name);
		::AppendPString (versionName, "\p,");
		::AppendPString (versionName, versionStr);
		inItem.eItemStatus = CompareFile (inItem, compareType, tempItem, versionName);
					
	CleanUp:
	
		return inItem.eItemStatus;
	
	} // end MacProcessRegularFile

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ CVSProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSDifference::CVSProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin CVSProcessRegularFile
		
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
	
	} // end CVSProcessRegularFile

// ---------------------------------------------------------------------------
//		¥ CVSProcessRegularFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSDifference::CVSProcessRegularFolder (
	
	CWVCSItem& 		inItem)

	{ // begin CVSProcessRegularFolder
		
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
	
	} // end CVSProcessRegularFolder

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ UsesCVSDiff
// ---------------------------------------------------------------------------

Boolean 
VCSDifference::UsesCVSDiff (void) const
	
	{ // begin UsesCVSDiff
		
		return (VCSGetDefaultOptions (mContext, OptionsRec::kDiffGetOptions) & kUsesCVSDiffMask);
		
	} // end UsesCVSDiff
	
// ---------------------------------------------------------------------------
//		¥ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSDifference::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFile
		
		if (UsesCVSDiff ()) return CVSProcessRegularFile (inItem);
			
		return MacProcessRegularFile (inItem);
		
	} // end ProcessRegularFile

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSDifference::ProcessRegularFolder (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFolder
		
		if (UsesCVSDiff ()) return CVSProcessRegularFolder (inItem);

		return VCSFileCommand::ProcessRegularFolder (inItem);
		
	} // end ProcessRegularFolder
