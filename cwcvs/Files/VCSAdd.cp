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
		virtual			~CVSAddOptionsDialog		(void);
		
		static	OSErr	DoDialog				(const 	VCSContext&		inContext,
												 const	FSSpec&			inFile,
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
	AEDescList&				outOptions)
	
	{ // begin DoDialog
		
		OSErr	e = noErr;

		VCSDialog::SetParamText (inFile.name);
		CVSAddOptionsDialog	d (inPB, kResourceID);
		if (ok != d.DoModalDialog ()) return false;
		
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
		
		//	If not advanced, just use the defaults 
		if (!inPB.Advanced ()) return noErr;
		
		if (noErr != (e = DoDialog (inPB, inFile, outOptions))) goto CleanUp;
			
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
