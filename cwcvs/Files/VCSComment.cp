#include "VCSComment.h"

#include "VCSAdvancedOptionsDialog.h"
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

#include <TextUtils.h>

#include <string.h>

//	=== Types ===

class CVSCommentOptionsDialog : public VCSAdvancedOptionsDialog 

	{
	
	protected:
		
		VCSDialogTextItem		mRevisionItem;
		
	public:
		
		enum {
			kPromptItem = cancel + 1,
			kRevisionLabelItem,
			kRevisionItem,
			
			kResourceID = 16300
			};
	
						CVSCommentOptionsDialog		(const VCSContext&		mContext,
													 short					inDLOGid);
		virtual			~CVSCommentOptionsDialog	(void);
		
		static	OSErr	DoDialog					(const 	VCSContext&		inContext,
													 const	FSSpec&			inFile,
													 const	AEDescList&		inDefaults,
													 AEDescList&			outOptions);
		static	OSErr 	GetOptions					(const 	VCSContext&		inContext,
													 const	FSSpec&			inFile,
													 ConstStr255Param		inComment,
													 AEDescList&			outOptions);
	};

//	===	Class Members ===

static char
sModifyArg [] = "-m";

static char
sSepArg [] = ":";

// ---------------------------------------------------------------------------
//		€ MakeModifyOption
// ---------------------------------------------------------------------------

static OSErr 
MakeModifyOption (
	
	AEDesc&				outOption,
	ConstStr255Param	inRev,
	ConstStr255Param	inMsg)
	
	{ // begin MakeModifyOption
		
		OSErr	e = noErr;

		do {
			outOption.descriptorType = typeChar;
			outOption.dataHandle = ::NewHandle (0);
			if (noErr != (e = ::PtrAndHand (sModifyArg, outOption.dataHandle, ::strlen (sModifyArg)))) break;
			if (noErr != (e = ::PtrAndHand (inRev + 1, outOption.dataHandle, StrLength (inRev)))) break;
			if (noErr != (e = ::PtrAndHand (sSepArg, outOption.dataHandle, ::strlen (sSepArg)))) break;
			if (noErr != (e = ::PtrAndHand (inMsg + 1, outOption.dataHandle, StrLength (inMsg)))) break;
			} while (false);
			
		return e;
		
	} // end MakeModifyOption

// ---------------------------------------------------------------------------
//		€ ParseModifyOption
// ---------------------------------------------------------------------------

static void 
ParseModifyOption (
	
	StringPtr			outRev,
	StringPtr			outMsg,
	const	AEDesc&		inOption)
	
	{ // begin ParseModifyOption
		
		long	optStart = ::Munger (inOption.dataHandle, 0, sModifyArg, ::strlen (sModifyArg), nil, 0);
		if (optStart < 0) return;

		if (outRev) outRev[0] = 0;
		if (outMsg) outMsg[0] = 0;
		
		long	revStart = optStart + ::strlen (sModifyArg);
		long	sepStart = ::Munger (inOption.dataHandle, revStart, sSepArg, ::strlen (sSepArg), nil, 0);
		if (sepStart < 0) return;

		long	msgStart = sepStart + ::strlen (sSepArg);
		long	msgEnd = ::GetHandleSize (inOption.dataHandle);
		
		if (outRev) ::BlockMoveData (*inOption.dataHandle + revStart, outRev + 1, outRev[0] = sepStart - revStart);
		if (outMsg) ::BlockMoveData (*inOption.dataHandle + sepStart, outMsg + 1, outMsg[0] = msgEnd - msgStart);
		
	} // end ParseModifyOption

// ---------------------------------------------------------------------------
//		€ ParseFileVersion
// ---------------------------------------------------------------------------

static Boolean
ParseFileVersion (

	const	FSSpec*			inSpec,
	Handle					inLine,
	StringPtr				outRev)
	
	{ // begin ParseTimeStamp
	
		const	char		slash = '/';
		
		
		/*	
		/CVSVCS.mcp/1.2/Fri Dec 10 23:43:38 1993//

		/VCSAbout.c/1.1/Sat Dec 11 23:41:43 1993//
		/VCSAbout.c/1.1/Wed Dec 10 15:41:43 1997//

		/VCSHistory.c/1.4/Fri Dec 10 22:36:38 1993//
		/VCSHistory.c/1.4/Tue Dec  9 14:36:38 1997//

		/VCSVersion.c/1.4/Fri Dec 10 23:20:32 1993//
		/VCSVersion.c/1.4/Thu Dec 11 09:16:32 1993//
		*/

		//	Parse the Command
		long	slashPos = ::Munger (inLine, 0, &slash, sizeof (slash), nil, 0);
		if (slashPos < 0) return false;
		::Munger (inLine, 0, nil, slashPos + 1, &slash, 0);

		//	Parse the file name
		Str255	fileName;
		slashPos = ::Munger (inLine, 0, &slash, sizeof (slash), nil, 0);
		if (slashPos < 0) return false;
		
		::BlockMoveData (*inLine, fileName + 1, fileName[0] = slashPos);
		if (!EqualString (fileName, inSpec->name, false, true)) return false;
		::Munger (inLine, 0, nil, slashPos + 1, &slash, 0);
		
		//	Parse the version
		slashPos = Munger (inLine, 0, &slash, sizeof (slash), nil, 0);
		if (slashPos < 0) return false;

		//	Negative version is uncommited delete
		if (**inLine == '-') 
			::BlockMoveData (*inLine + 1, outRev + 1, outRev[0] = slashPos - 1);
		else ::BlockMoveData (*inLine, outRev + 1, outRev[0] = slashPos);
		
		return true;
	
	} // end ParseFileVersion
	
#pragma mark -

// ---------------------------------------------------------------------------
//		€ CVSCommentOptionsDialog
// ---------------------------------------------------------------------------

CVSCommentOptionsDialog::CVSCommentOptionsDialog (

	const VCSContext&	mContext,
	short				inDLOGid)
	
	: VCSAdvancedOptionsDialog (mContext, inDLOGid)
	
	, mRevisionItem (*this, kRevisionItem)
	
	{ // begin CVSCommentOptionsDialog
	
	} // end CVSCommentOptionsDialog
	
// ---------------------------------------------------------------------------
//		€ ~CVSCommentOptionsDialog
// ---------------------------------------------------------------------------

CVSCommentOptionsDialog::~CVSCommentOptionsDialog (void)

	{ // begin ~CVSCommentOptionsDialog
	
	} // end ~CVSCommentOptionsDialog

// ---------------------------------------------------------------------------
//		€ DoDialog
// ---------------------------------------------------------------------------

OSErr 
CVSCommentOptionsDialog::DoDialog (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	const	AEDescList&		inDefaults,
	AEDescList&				outOptions)
	
	{ // begin DoDialog
		
		OSErr	e = noErr;

		do {
			VCSDialog::SetParamText (inFile.name);
			CVSCommentOptionsDialog	d (inPB, kResourceID);
			
			//	Set options
			Str255		rev;
			Str255		msg;

			long		optionCount;
			if (noErr != (e = ::AECountItems (&inDefaults, &optionCount))) break;
			for (long i = 1; i <= optionCount; ++i) {
				AEKeyword	theAEKeyword;
				StAEDesc	argDesc;
				if (noErr != (e = ::AEGetNthDesc (&inDefaults, i, typeChar, &theAEKeyword, &argDesc))) continue;
				
				ParseModifyOption (rev, msg, argDesc);
				} // for
			
			d.SetItemText (kRevisionItem, rev);
			::SelectDialogItemText (d.GetDialogPtr (), kRevisionItem, 0, 0x7FFF);
			
			//	Run the dialog
			if (ok != d.DoModalDialog ()) return userCanceledErr;
			
			//	Get options
			d.GetItemText (kRevisionItem, rev);

			StAEDesc	argDesc;
			MakeModifyOption (argDesc, rev, msg);
			if (noErr != (e = AEPutDesc (&outOptions, 0, &argDesc))) break;
			} while (false);
			
		return e;
		
	} // end DoDialog

// ---------------------------------------------------------------------------
//		€ GetOptions
// ---------------------------------------------------------------------------

OSErr 
CVSCommentOptionsDialog::GetOptions (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	ConstStr255Param		inMsg,
	AEDescList&				outOptions)
	
	{ // begin GetOptions
		
		OSErr				e = noErr;
		
		do {
			//	Get the defaults
			AEDescList			defaultList = {typeNull, nil};
			if (noErr != (e = ::AECreateList (nil, 0 , false, &defaultList))) break;
				
			//	Read the Entries file
			Str255				rev = {0};
			FSSpec				entriesSpec;
			if (noErr == ::FSMakeFSSpec (inFile.vRefNum, inFile.parID, "\p:CVS:ENTRIES", &entriesSpec)) {
				StHandle			entries;
				if (noErr != (e = ReadFileContents (&entries.mH, &entriesSpec))) break;
				
				//	Find the file data
				while (0 != GetHandleSize (entries.mH)) {
					StHandle			line;
					if (noErr != GetNextLine (&line.mH, entries.mH)) break;
					
					ParseFileVersion (&inFile, line.mH, rev);
					} // while
				} // if
			// else no CVS:ENTRIES => not in DB
			
			StAEDesc	argDesc;
			MakeModifyOption (argDesc, rev, inMsg);
			if (noErr != (e = AEPutDesc (&defaultList, 0, &argDesc))) break;
			
			//	If not advanced, just use the defaults 
			if (!inPB.Advanced ()) {
				if (noErr != (e = ::CVSAddListArgs (&outOptions, &defaultList))) break;
				} // if
				
			else {
				if (noErr != (e = DoDialog (inPB, inFile, defaultList, outOptions))) break;
				} // else
			} while (false);
			
		return e;
	
	} // end GetOptions

#pragma mark -

//	=== Constants ===

// ---------------------------------------------------------------------------
//		€ VCSComment
// ---------------------------------------------------------------------------

VCSComment::VCSComment (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, false, true)
		
	{ // begin VCSComment
		
	} // end VCSComment

// ---------------------------------------------------------------------------
//		€ ~VCSComment
// ---------------------------------------------------------------------------

VCSComment::~VCSComment (void)
	
	{ // begin ~VCSComment
		
	} // end ~VCSComment

// ---------------------------------------------------------------------------
//		€ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSComment::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFile
		
		FSSpec		cwd = inItem.fsItem;
		Str255		comment;
		
		//	Stuff to clean up
		StAEDesc 	command;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kCommentTask, inItem.fsItem.name);
		
		//	Get the cwd for update
		if (noErr != VCSRaiseOSErr (mContext, ::FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) return inItem.eItemStatus;

		//	Get the checkin comment
		comment[0] = 0;
		if (!VCSPromptComment (mContext, kCheckinCommentPrompt, inItem.fsItem.name, comment)) 
			return inItem.eItemStatus = cwItemStatusCancelled;
			
		//	cvs admin -m<rev>:<comment> <file>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "admin"))) return inItem.eItemStatus;

		//	Get the options.
		OSErr			e = noErr;
		switch (e = CVSCommentOptionsDialog::GetOptions (mContext, inItem.fsItem, comment, command)) {
			case userCanceledErr:
				return inItem.eItemStatus = cwItemStatusCancelled;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) return inItem.eItemStatus;
			} // switch

		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) return inItem.eItemStatus;
		
		// send the command to MacCVS
		if (noErr != (VCSSendCommand (mContext, &command, &cwd))) return inItem.eItemStatus;
		
		//	Update status
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
			
		return inItem.eItemStatus;
		
	} // end ProcessRegularFile

