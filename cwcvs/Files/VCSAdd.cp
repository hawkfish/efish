#include "VCSAdd.h"

#include "VCSAdvancedOptionsDialog.h"
#include "VCSDialogPopupItem.h"
#include "VCSDialogTextItem.h"
#include "VCSError.h"
#include "VCSPrefs.h"
#include "VCSPrompt.h"
#include "VCSTask.h"
#include "VCSUtil.h"
#include "VCSVersion.h"

#include "CVSCommand.h"

#include "StAEDesc.h"

#include <TextUtils.h>

//	=== Types ===

class CVSAddOptionsDialog : public VCSAdvancedOptionsDialog 

	{
	
	protected:
		
		VCSDialogTextItem		mDescriptionItem;
		VCSDialogPopupItem		mKeywordsItem;
		
	public:
		
		enum {
			kDescriptionItem = cancel + 1,
			kDescriptionLabelItem,
			kPromptItem,
			kKeywordsItem,
			
			kKeywordBaseIndex = 6,
			
			kResourceID = 16240
			};
	
						CVSAddOptionsDialog		(const VCSContext&		inContext,
												 short					inDLOGid);
		virtual			~CVSAddOptionsDialog	(void);
		
		static	OSErr	DoDialog				(const 	VCSContext&		inContext,
												 const	FSSpec&			inFile,
												 const	AEDescList&		inDefaults,
												 AEDescList&			outOptions);
		static	OSErr 	GetOptions				(const 	VCSContext&		inContext,
												 const	FSSpec&			inFile,
												 AEDescList&			outOptions);
	};

//	===	Class Members ===

// ---------------------------------------------------------------------------
//		€ CVSAddOptionsDialog
// ---------------------------------------------------------------------------

CVSAddOptionsDialog::CVSAddOptionsDialog (

	const VCSContext&	mContext,
	short				inDLOGid)
	
	: VCSAdvancedOptionsDialog (mContext, inDLOGid)
	
	, mDescriptionItem (*this, kDescriptionItem)
	, mKeywordsItem (*this, kKeywordsItem)
	
	{ // begin CVSAddOptionsDialog
	
	} // end CVSAddOptionsDialog
	
// ---------------------------------------------------------------------------
//		€ ~CVSAddOptionsDialog
// ---------------------------------------------------------------------------

CVSAddOptionsDialog::~CVSAddOptionsDialog (void)

	{ // begin ~CVSAddOptionsDialog
	
	} // end ~CVSAddOptionsDialog

// ---------------------------------------------------------------------------
//		€ DoDialog
// ---------------------------------------------------------------------------

OSErr 
CVSAddOptionsDialog::DoDialog (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	const	AEDescList&		inDefaults,
	AEDescList&				outOptions)
	
	{ // begin DoDialog
		
		OSErr	e = noErr;

		VCSDialog::SetParamText (inFile.name);
		CVSAddOptionsDialog	d (inPB, kResourceID);
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
CVSAddOptionsDialog::GetOptions (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	AEDescList&				outOptions)
	
	{ // begin GetOptions
		
		OSErr				e = noErr;
		
		//	Get the filetype
		FInfo	fInfo;
		if (noErr != (e = ::FSpGetFInfo (&inFile, &fInfo))) goto CleanUp;

		//	Get the defaults
		AEDescList			defaultList = {typeNull, nil};
		if (noErr != (e = ::AECreateList (nil, 0 , false, &defaultList))) return e;
		UInt16				keywordIndex = (fInfo.fdType == 'TEXT') 
								? VCSGetTextKeywordDefault (inPB)
								: VCSGetBinaryKeywordDefault (inPB);

		Str255				keyword = {0};
		::GetIndString (keyword, kResourceID, kKeywordBaseIndex + keywordIndex);
		if (keyword[0] && (noErr != (e = ::CVSAddPStringArg (&defaultList, keyword)))) goto CleanUp;
			
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
//		€ VCSAdd
// ---------------------------------------------------------------------------

VCSAdd::VCSAdd (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, false, true)
		
	{ // begin VCSAdd
		
	} // end VCSAdd

// ---------------------------------------------------------------------------
//		€ ~VCSAdd
// ---------------------------------------------------------------------------

VCSAdd::~VCSAdd (void)
	
	{ // begin ~VCSAdd
		
	} // end ~VCSAdd

// ---------------------------------------------------------------------------
//		€ AddDirectoryOfFile
// ---------------------------------------------------------------------------
//	N.B. Recursive

OSErr
VCSAdd::AddDirectoryOfFile (

	const	FSSpec&		inDirFile)
	
	{ // begin AddDirectoryOfFile
		
		OSErr		e = noErr;

		//	Stuff to clean up
		StAEDesc 	command;

		//	Check for the CVS directory
		FSSpec		cvsSpec;
		if (noErr == ::FSMakeFSSpec (inDirFile.vRefNum, inDirFile.parID, "\pCVS", &cvsSpec)) return e;
		
		//	Add the parent
		FSSpec		dirSpec;
		if (noErr != (e = VCSRaiseOSErr (mContext, ::FSMakeFSSpec (inDirFile.vRefNum, inDirFile.parID, nil, &dirSpec)))) return e;
		if (noErr != (e = AddDirectoryOfFile (dirSpec))) return e;
		
		//	Ask if they wish to add it
		switch (::VCSPromptYesNoCancel (mContext, kPromptStringsID, kAddFolderPrompt, dirSpec.name)) {
			case kPromptYes:
				break;
				
			case kPromptCancel:
				return userCanceledErr;
				
			case kPromptNo:
				return e;		//	This will result in an error, but it may give them useful information
			} // switch
			
		//	Get the cwd for update
		FSSpec			cwd = dirSpec;
		if (noErr != VCSRaiseOSErr (mContext, ::FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) return e;
		
		//	Show task
		VCSTask 	task (mContext, kTaskStrings, kAddTask, dirSpec.name);

		/* create an add command for MacCVS
		 * add <directory>
		 */
		if (noErr != (e = VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "add")))) return e;
		if (noErr != (e = VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, dirSpec.name)))) return e;
		
		// send the command to MacCVS
		if (noErr != (e = VCSSendCommand (mContext, &command, &cwd))) return e;
	
		return e;
		
	} // end AddDirectoryOfFile

// ---------------------------------------------------------------------------
//		€ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSAdd::ProcessRegularFile (
	
	CWVCSItem& 			inItem)

	{ // begin ProcessRegularFile

		OSErr			e = noErr;
		FSSpec			cwd = inItem.fsItem;
		
		//	Stuff to clean up
		StAEDesc 		command;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kAddTask, inItem.fsItem.name);
		
		//	Add the directory hierarchy
		switch (e = AddDirectoryOfFile (inItem.fsItem)) {
			case userCanceledErr:
				inItem.eItemStatus = cwItemStatusCancelled;
				goto CleanUp;
		
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) goto CleanUp;
			} // switch
		
		//	Get the cwd for add
		if (noErr != VCSRaiseOSErr (mContext, ::FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		/* create an add command for MacCVS
		 * add <file>
		 */
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "add"))) goto CleanUp;

		//	Get the options.
		switch (e = CVSAddOptionsDialog::GetOptions (mContext, inItem.fsItem, command)) {
			case userCanceledErr:
				inItem.eItemStatus = cwItemStatusCancelled;
				goto CleanUp;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) goto CleanUp;
			} // switch
		
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) goto CleanUp;
		
		// send the command to MacCVS
		if (noErr != (VCSSendCommand (mContext, &command, &cwd))) goto CleanUp;
		
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
		
	CleanUp:
		
		return inItem.eItemStatus;
				
	} // end ProcessRegularFile
