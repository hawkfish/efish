/*
	File:		VCSDialogContext.cp

	Contains:	Implementation of the CMM VCS context.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

        <12>      5/5/99    rmgw    Don't abort AppleEvent!
        <11>      5/5/99    rmgw    Split saving and prompting dialog.
        <10>      4/7/99    rmgw    Use display dialog for results windows under 32K
         <9>     3/31/99    rmgw    Check for cmd-period.
         <8>     3/31/99    rmgw    Add AE idling.
         <7>     3/31/99    rmgw    Fix comment result.
         <6>     3/30/99    rmgw    Crash recovery merging.
         <5>     3/22/99    rmgw    Add results dialog.
         <4>     3/19/99    rmgw    First pass at GUI.
         <3>     3/19/99    rmgw    Implement CM project documents.
         <2>     3/18/99    rmgw    Make file optional for handshake.
         <1>     3/17/99    rmgw    first checked in.
*/


#include "VCSDialogContext.h"

#include "VCSPrompt.h"
#include "VCSUtil.h"
#include "MoreFilesExtras.h"

#include <Dialogs.h>
#include <Navigation.h>
#include <Folders.h>
#include <Resources.h>
#include <TextUtils.h>

#include <string.h>

//	Constants

const	ResID	dlog_Comment 	= 2000;
const	ResID	alrt_Message 	= 2001;

// ---------------------------------------------------------------------------
//		€ CheckResult
// ---------------------------------------------------------------------------

CWResult
VCSDialogContext::CheckResult (

	CWResult	inResult)
		
	{ // begin CheckResult
		
		if (inResult != cwNoErr) throw inResult;
		
		return inResult;
		
	} // end CheckResult

// ---------------------------------------------------------------------------
//		€ CheckOptionalResult
// ---------------------------------------------------------------------------

CWResult
VCSDialogContext::CheckOptionalResult (

	CWResult	inResult)
		
	{ // begin CheckOptionalResult
		
		switch (inResult) {
			case cwNoErr:
			case cwErrInvalidCallback:
				return inResult;
			
			default:
				return CheckResult (inResult);
			} // switch
		
	} // end CheckOptionalResult

#pragma mark -

// ---------------------------------------------------------------------------
//		€ VCSDialogContext
// ---------------------------------------------------------------------------

VCSDialogContext::VCSDialogContext (
	
	const	FSSpec*						inContext,
	long								inRequest,
	Boolean								inAdvanced,
	Boolean								inRecursive,
	Boolean								inSupported)
	
	: mProgressDialog (*this)
	, mResultsDialog (*this)
	, mHaveResults (false)
	, mYieldResult (cwNoErr)
	
	, mItemCount (0)
	, mRequest (inRequest)
	, mAdvanced (inAdvanced)
	, mRecursive (inRecursive)
	, mSupported (inSupported)
	
	, mCurrentStatus (cwNoErr)
	, mCheckoutState (cwCheckoutStateUnknown)
	
	, mUseForAll (false)

	{ // begin VCSDialogContext
		
		//	Item list
		if (inContext) {
			mItem.fsItem = *inContext;
			mItem.eItemStatus = cwItemStatusUnprocessed;
			mItem.version.eVersionForm = cwVersionFormNone;
			mItemCount = 1;
			} // if
		
		//	Username
		mUserName[0] = 0;

		StringHandle	str = ::GetString (-16096);
		if (str) {
			::BlockMoveData (*str + 1, mUserName, **str);
			mUserName[**str] = 0;
			} // if
		
		//	Password
		mPassword[0] = 0;

		//	Comment
		mLastComment[0] = 0;
		
	} // end VCSDialogContext

// ---------------------------------------------------------------------------
//		€ ~VCSDialogContext
// ---------------------------------------------------------------------------

VCSDialogContext::~VCSDialogContext (void)
	
	{ // begin ~VCSDialogContext
		
		if (mHaveResults) mResultsDialog.DoModalDialog ();
		
	} // end VCSDialogContext

// ---------------------------------------------------------------------------
//		€ GetAPIVersion
// ---------------------------------------------------------------------------

long
VCSDialogContext::GetAPIVersion (void) const
	
	{ // begin GetAPIVersion

		//	TODO:	Implement

		return 0;
		
	} // end GetAPIVersion

// ---------------------------------------------------------------------------
//		€ GetIDEVersion
// ---------------------------------------------------------------------------

CWResult
VCSDialogContext::GetIDEVersion (

	CWIDEInfo&	outInfo) const
	
	{ // begin GetIDEVersion
		
		//	TODO:	Something better

		outInfo.majorVersion = 0;
		outInfo.minorVersion = 0;
		outInfo.bugFixVersion = 0;
		outInfo.buildVersion = 0;
		outInfo.dropinAPIVersion = 0;

		return cwNoErr;
		
	} // end GetIDEVersion

// ---------------------------------------------------------------------------
//		€ YieldTime
// ---------------------------------------------------------------------------

CWResult
VCSDialogContext::YieldTime (void)
	
	{ // begin YieldTime
		
		EventRecord	theEvent;
		short		mask = mDownMask | mUpMask | keyDownMask | keyUpMask;
		::WaitNextEvent (updateMask, &theEvent, 6, nil);
		
		if (!::IsDialogEvent (&theEvent)) ::WaitNextEvent (mask, &theEvent, 6, nil);
		
		if (ok == mProgressDialog.DoModelessDialog (theEvent)) mYieldResult = cwErrUserCanceled;
			
		return mYieldResult;

	} // end YieldTime

// ---------------------------------------------------------------------------
//		€ GetRequest
// ---------------------------------------------------------------------------
	
long
VCSDialogContext::GetRequest (void) const
	
	{ // begin GetRequest

		return mRequest;

	} // end GetRequest

// ---------------------------------------------------------------------------
//		€ GetMemHandleSize
// ---------------------------------------------------------------------------

long
VCSDialogContext::GetMemHandleSize (

	CWMemHandle inData) const
	
	{ // begin GetMemHandleSize
		
		Handle	h = (Handle) inData;

		return ::GetHandleSize (h);

	} // end GetMemHandleSize

// ---------------------------------------------------------------------------
//		€ LockMemHandle
// ---------------------------------------------------------------------------

void*
VCSDialogContext::LockMemHandle (

	CWMemHandle inData, 
	Boolean 	inMoveHi) const
	
	{ // begin LockMemHandle
		
		Handle	h = (Handle) inData;
		if (inMoveHi) ::MoveHHi (h);
		::HLock (h);

		return *h;

	} // end LockMemHandle

// ---------------------------------------------------------------------------
//		€ UnlockMemHandle
// ---------------------------------------------------------------------------

void
VCSDialogContext::UnlockMemHandle (

	CWMemHandle inData) const
	
	{ // begin UnlockMemHandle
		
		Handle	h = (Handle) inData;
		::HUnlock (h);

	} // end UnlockMemHandle

// ---------------------------------------------------------------------------
//		€ GetCommandStatus
// ---------------------------------------------------------------------------	

CWVCSCommandStatus
VCSDialogContext::GetCommandStatus (void) const
	
	{ // begin GetCommandStatus

		return mCurrentStatus;

	} // end GetCommandStatus

// ---------------------------------------------------------------------------
//		€ SetCommandStatus
// ---------------------------------------------------------------------------	

void
VCSDialogContext::SetCommandStatus (

	CWVCSCommandStatus inStatus)
	
	{ // begin SetCommandStatus
		
		mCurrentStatus = inStatus;

	} // end SetCommandStatus

// ---------------------------------------------------------------------------
//		€ PreDialog
// ---------------------------------------------------------------------------	

CWResult
VCSDialogContext::PreDialog (void) const
	
	{ // begin PreDialog

		//	TODO:	Deactivate top window?

		return cwNoErr;

	} // end PreDialog

// ---------------------------------------------------------------------------
//		€ PostDialog
// ---------------------------------------------------------------------------	

CWResult
VCSDialogContext::PostDialog (void) const
	
	{ // begin PostDialog

		//	TODO:	Activate top window?

		return cwNoErr;

	} // end PostDialog

// ---------------------------------------------------------------------------
//		€ PreFileAction
// ---------------------------------------------------------------------------	

void
VCSDialogContext::PreFileAction (

	const CWFileSpec& ) const
	
	{ // begin PreFileAction

	} // end PreFileAction

// ---------------------------------------------------------------------------
//		€ PostFileAction
// ---------------------------------------------------------------------------	

void
VCSDialogContext::PostFileAction (

	const CWFileSpec& ) const
	
	{ // begin PostFileAction

	} // end PostFileAction

// ---------------------------------------------------------------------------
//		€ CompletionRatio
// ---------------------------------------------------------------------------	

void
VCSDialogContext::CompletionRatio (

	int 	total, 
	int 	completed)
	
	{ // begin CompletionRatio
		
		mProgressDialog.CompletionRatio (total, completed);

	} // end CompletionRatio

// ---------------------------------------------------------------------------
//		€ ReportProgress
// ---------------------------------------------------------------------------	

void
VCSDialogContext::ReportProgress (

	const char *line1, 
	const char *line2)
	
	{ // begin ReportProgress
		
		//	Dialog
		DialogPtr	d (mProgressDialog.GetDialogPtr ());
		::ShowWindow (d);
		
		mProgressDialog.ReportProgress (line1, line2);
		::DrawDialog (d);

	} // end ReportProgress

// ---------------------------------------------------------------------------
//		€ CreateDocument
// ---------------------------------------------------------------------------

#include <StandardFile.h>

void
VCSDialogContext::CreateDocument (

	const	char*	inName,
	Handle			inData,
	Boolean			inDirty) const
		
	{ // begin CreateDocument
		
		CMResultsDialog		results (*this);
		results.CreateDocument (inName, inData, inDirty);
		
	} // end CreateDocument

#pragma mark -

// ---------------------------------------------------------------------------
//		€ CheckIfSupported
// ---------------------------------------------------------------------------

Boolean
VCSDialogContext::CheckIfSupported (void) const

	{ // begin CheckIfSupported
	
		return mSupported;
		
	} // end CheckIfSupported

// ---------------------------------------------------------------------------
//		€ Advanced
// ---------------------------------------------------------------------------

Boolean
VCSDialogContext::Advanced (void) const
	
	{ // begin Advanced

		return mAdvanced;

	} // end Advanced

// ---------------------------------------------------------------------------
//		€ Recursive
// ---------------------------------------------------------------------------

Boolean
VCSDialogContext::Recursive (void) const
	
	{ // begin Recursive

		return mRecursive;

	} // end Recursive

// ---------------------------------------------------------------------------
//		€ GetItemCount
// ---------------------------------------------------------------------------

unsigned long
VCSDialogContext::GetItemCount (void) const
	
	{ // begin GetItemCount

		return mItemCount;

	} // end GetItemCount

// ---------------------------------------------------------------------------
//		€ GetItem
// ---------------------------------------------------------------------------

void
VCSDialogContext::GetItem (

	CWVCSItem&	outItem, 
	long 		inItemNo) const
	
	{ // begin GetItem
		
		if (inItemNo >= GetItemCount ()) CheckResult(cwErrInvalidParameter);
		
		outItem = mItem;

	} // end GetItem

// ---------------------------------------------------------------------------
//		€ SetItem
// ---------------------------------------------------------------------------

void
VCSDialogContext::SetItem (

	const CWVCSItem& 	inItem, 
	long 				inItemNo)
	
	{ // begin SetItem

		if (inItemNo >= GetItemCount ()) CheckResult(cwErrInvalidParameter);

		mItem = inItem;

	} // end SetItem

// ---------------------------------------------------------------------------
//		€ GetComment
// ---------------------------------------------------------------------------

#include "SSTaskDialog.h"

CWResult
VCSDialogContext::GetComment (

	const 	char*	pPrompt, 
	char*			pComment, 
	const	long	lBufferSize)
	
	{ // begin GetComment
		
		Str255	prompt;
		::BlockMoveData (pPrompt, prompt + 1, prompt[0] = ::strlen (pPrompt));
		
		StringPtr		comment = ::c2pstr (pComment);

		if (!SSTaskDialog::DoDialog (*this, dlog_Comment, prompt, comment, lBufferSize, mUseForAll, mLastComment)) 
			return cwErrUserCanceled;
		
		::BlockMoveData (mLastComment + 1, pComment, mLastComment[0]);
		pComment[mLastComment[0]] = 0;
		
		return cwNoErr;
		
	} // end GetComment

// ---------------------------------------------------------------------------
//		€ UpdateCheckoutState
// ---------------------------------------------------------------------------

void
VCSDialogContext::UpdateCheckoutState (

	const CWFileSpec& 		/*inItem*/, 
	CWVCSCheckoutState 		inCheckoutState, 
	const CWVCSVersion& 	version)
	
	{ // begin UpdateCheckoutState
		
		mCheckoutState = inCheckoutState;
		mItem.version = version;
		
	} // end UpdateCheckoutState

// ---------------------------------------------------------------------------
//		€ MessageOutput
// ---------------------------------------------------------------------------

void
VCSDialogContext::MessageOutput (

	short 			errorlevel, 
	const char*		inLine1, 
	const char*		inLine2, 
	long 			inErrorNumber)
	
	{ // begin MessageOutput
		
		mResultsDialog.MessageOutput (errorlevel, inLine1, inLine2, inErrorNumber);
		mHaveResults = true;
			
	} // end MessageOutput

// ---------------------------------------------------------------------------
//		€ VisualDifference
// ---------------------------------------------------------------------------

CWResult
VCSDialogContext::VisualDifference (

	const CWFileSpec& /*file1*/, 
	const CWFileSpec& /*file2*/) const
	
	{ // begin VisualDifference

		return cwErrInvalidCallback;

	} // end VisualDifference
	
// ---------------------------------------------------------------------------
//		€ VisualDifference
// ---------------------------------------------------------------------------

CWResult
VCSDialogContext::VisualDifference (

	const char*			/*file1Name*/, 
	const char*			/*file1Text*/, 
	unsigned	long	/*file1TextLength*/, 
	const CWFileSpec& 	/*file2*/) const
	
	{ // begin VisualDifference

		return cwErrInvalidCallback;

	} // end VisualDifference

#pragma mark -

// ---------------------------------------------------------------------------
//		€ OnAEIdle
// ---------------------------------------------------------------------------

CWResult
VCSDialogContext::OnAEIdle (

	EventRecord&				theEvent, 
	long&						sleepTime, 
	RgnHandle&					mouseRgn)
	
	{ // begin OnAEIdle

		switch (theEvent.what) {
			case nullEvent:
				mouseRgn = nil;
				sleepTime = 6;
				break;
			} // switch
		
		if (ok == mProgressDialog.DoModelessDialog (theEvent)) mYieldResult = cwErrUserCanceled;

		return cwNoErr;

	} // end OnAEIdle
	
#pragma mark -

// ---------------------------------------------------------------------------
//		€ GetCheckoutState
// ---------------------------------------------------------------------------

CWVCSCheckoutState
VCSDialogContext::GetCheckoutState (void) const
	
	{ // begin GetCheckoutState

		return mCheckoutState;

	} // end GetCheckoutState
