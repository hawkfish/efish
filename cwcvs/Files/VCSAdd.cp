#include "VCSAdd.h"

#include "VCSError.h"
#include "VCSTask.h"
#include "VCSUtil.h"
#include "VCSVersion.h"

#include "CVSCommand.h"

#include "VCSAdvancedOptionsDialog.h"
#include "VCSDialogPopupItem.h"
#include "VCSDialogTextItem.h"

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
			
			kResourceID = 16240
			};
	
						CVSAddOptionsDialog		(const VCSContext&	mContext,
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
//		¥ CVSAddOptionsDialog
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
//		¥ ~CVSAddOptionsDialog
// ---------------------------------------------------------------------------

CVSAddOptionsDialog::~CVSAddOptionsDialog (void)

	{ // begin ~CVSAddOptionsDialog
	
	} // end ~CVSAddOptionsDialog

// ---------------------------------------------------------------------------
//		¥ DoDialog
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
//		¥ GetOptions
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
		if (fInfo.fdType != 'TEXT') {
			//	Non-text files default to binary
			if (noErr != (e = ::CVSAddCStringArg (&defaultList, "-kb"))) goto CleanUp;
			} // if
			
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
//		¥ VCSAdd
// ---------------------------------------------------------------------------

VCSAdd::VCSAdd (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, false, true)
		
	{ // begin VCSAdd
		
	} // end VCSAdd

// ---------------------------------------------------------------------------
//		¥ ~VCSAdd
// ---------------------------------------------------------------------------

VCSAdd::~VCSAdd (void)
	
	{ // begin ~VCSAdd
		
	} // end ~VCSAdd

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSAdd::ProcessRegularFile (
	
	CWVCSItem& 			inItem)

	{ // begin ProcessRegularFile

		FSSpec		cwd = inItem.fsItem;
		
		//	Stuff to clean up
		AEDesc 		command = {typeNull, nil};
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kAddTask, inItem.fsItem.name);
		
		//	Get the cwd for update
		if (noErr != VCSRaiseOSErr (mContext, FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;
		
		/* create an add command for MacCVS
		 * add <file>
		 */
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "add"))) goto CleanUp;

		//	Get the options.
		OSErr			e = noErr;
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
		
		AEDisposeDesc (&command);

		return inItem.eItemStatus;
				
	} // end ProcessRegularFile
