/*
	File:		VCSV7Context.cp

	Contains:	Implementation of the v7 API context.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

        <13>      5/5/99    rmgw    Don't abort AppleEvent!
        <12>     3/31/99    rmgw    Add user cancel routine.
        <11>     3/31/99    rmgw    Add AE idling.
        <10>     3/31/99    rmgw    Move comment paste kludge to IDE wrapper.
         <9>     3/30/99    rmgw    Crash recovery merging.
         <8>     3/22/99    rmgw    MessageOutput should not be const.
         <7>     3/19/99    rmgw    ReportProgress should not be const.
         <6>     3/19/99    rmgw    Add handle size and project file accessors.
         <5>     3/18/99    rmgw    UpdateCheckoutState no longer const.
         <4>     12/4/98    rmgw    Add GetIDEVersion.
         <3>     12/1/98    rmgw    Add memory based difference.
         <2>    11/12/98    rmgw    Return visual difference result code.
         <1>    11/10/98    rmgw    first checked in.
*/


#include "VCSV7Context.h"

#include "VCSUtil.h"

#include <string.h>

#include <MacWindows.h>
#include <Scrap.h>
#include <TextEdit.h>

// ---------------------------------------------------------------------------
//		€ CheckResult
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::CheckResult (

	CWResult	inResult)
		
	{ // begin CheckResult
		
		if (inResult != cwNoErr) throw inResult;
		
		return inResult;
		
	} // end CheckResult

// ---------------------------------------------------------------------------
//		€ CheckOptionalResult
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::CheckOptionalResult (

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
//		€ VCSV7Context
// ---------------------------------------------------------------------------

VCSV7Context::VCSV7Context (

	CWPluginContext	inContext)
	
	: mContext (inContext)
	
	{ // begin VCSV7Context
		
	} // end VCSV7Context

// ---------------------------------------------------------------------------
//		€ ~VCSV7Context
// ---------------------------------------------------------------------------

VCSV7Context::~VCSV7Context (void)
	
	{ // begin ~VCSV7Context
		
	} // end VCSV7Context

// ---------------------------------------------------------------------------
//		€ GetAPIVersion
// ---------------------------------------------------------------------------

long
VCSV7Context::GetAPIVersion (void) const
	
	{ // begin GetAPIVersion
	
		long result; 
		CheckResult (::CWGetAPIVersion (mContext, &result)); 
		return result;
		
	} // end GetAPIVersion

// ---------------------------------------------------------------------------
//		€ GetIDEVersion
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::GetIDEVersion (

	CWIDEInfo&	outInfo) const
	
	{ // begin GetIDEVersion
	
		return ::CWGetIDEInfo (mContext, &outInfo);
		
	} // end GetIDEVersion

// ---------------------------------------------------------------------------
//		€ YieldTime
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::YieldTime (void)
	
	{ // begin YieldTime

		CWResult result = ::CWUserBreak (mContext);
		
		do {
			if (result != cwNoErr) break;
			
			//	With version 4.0.x of the IDE there is a bug with CWUserBreak that
			//	causes background apps to not get any time.  Basically, the callback does
			//	bourgeous
			
			//	Check the version
			CWIDEInfo	info;
			GetIDEVersion (info);
			if (info.majorVersion != 4) break;
			if (info.minorVersion != 0) break;
			
			//	Yield for real...
			EventRecord	theEvent;
			if (!EventAvail (everyEvent, &theEvent)) return result;
			
			switch (theEvent.what) {
				case updateEvt:
					if (::WaitNextEvent (updateMask, &theEvent, 6, nil)) {
						WindowPtr	w = (WindowPtr) theEvent.message;
						::BeginUpdate (w);
						::EndUpdate (w);
						} // if
					break;

				default:
					::WaitNextEvent (everyEvent, &theEvent, 6, nil);
					break;
				} // switch
			} while (false);
			
		return result;
		
	} // end YieldTime

// ---------------------------------------------------------------------------
//		€ GetRequest
// ---------------------------------------------------------------------------
	
long
VCSV7Context::GetRequest (void) const
	
	{ // begin GetRequest

		long result; 
		CheckResult (::CWGetPluginRequest (mContext, &result));
		return result;

	} // end GetRequest

// ---------------------------------------------------------------------------
//		€ GetNamedPreferences
// ---------------------------------------------------------------------------

CWMemHandle
VCSV7Context::GetNamedPreferences (

	const char* prefsname) const
	
	{ // begin GetNamedPreferences

		CWMemHandle result; CheckResult (::CWGetNamedPreferences (mContext, prefsname, &result));
		return result;

	} // end GetNamedPreferences

// ---------------------------------------------------------------------------
//		€ GetProjectFile
// ---------------------------------------------------------------------------

void
VCSV7Context::GetProjectFile (

	CWFileSpec& outProjectSpec) const
	
	{ // begin GetProjectFile

		CheckResult (::CWGetProjectFileSpecifier (mContext, &outProjectSpec));

	} // end GetProjectFile

// ---------------------------------------------------------------------------
//		€ GetMemHandleSize
// ---------------------------------------------------------------------------

long
VCSV7Context::GetMemHandleSize (

	CWMemHandle inData) const
	
	{ // begin GetMemHandleSize

		long	 result;
		CheckResult (::CWGetMemHandleSize (mContext, inData, &result));
		return result;

	} // end GetMemHandleSize

// ---------------------------------------------------------------------------
//		€ LockMemHandle
// ---------------------------------------------------------------------------

void*
VCSV7Context::LockMemHandle (

	CWMemHandle inData, 
	Boolean 	inMoveHi) const
	
	{ // begin LockMemHandle

		void* result;
		CheckResult (::CWLockMemHandle (mContext, inData, inMoveHi, &result));
		return result;

	} // end LockMemHandle

// ---------------------------------------------------------------------------
//		€ UnlockMemHandle
// ---------------------------------------------------------------------------

void
VCSV7Context::UnlockMemHandle (

	CWMemHandle inData) const
	
	{ // begin UnlockMemHandle

		CheckResult (::CWUnlockMemHandle (mContext, inData));

	} // end UnlockMemHandle

// ---------------------------------------------------------------------------
//		€ GetCommandStatus
// ---------------------------------------------------------------------------	

CWVCSCommandStatus
VCSV7Context::GetCommandStatus (void) const
	
	{ // begin GetCommandStatus

		CWVCSCommandStatus result;
		CheckResult (::CWGetCommandStatus (mContext, &result));
		return result;

	} // end GetCommandStatus

// ---------------------------------------------------------------------------
//		€ SetCommandStatus
// ---------------------------------------------------------------------------	

void
VCSV7Context::SetCommandStatus (

	CWVCSCommandStatus inStatus)
	
	{ // begin SetCommandStatus

		CheckResult (::CWSetCommandStatus (mContext, inStatus));

	} // end SetCommandStatus

// ---------------------------------------------------------------------------
//		€ PreDialog
// ---------------------------------------------------------------------------	

CWResult
VCSV7Context::PreDialog (void) const
	
	{ // begin PreDialog

		return ::CWPreDialog (mContext);

	} // end PreDialog

// ---------------------------------------------------------------------------
//		€ PostDialog
// ---------------------------------------------------------------------------	

CWResult
VCSV7Context::PostDialog (void) const
	
	{ // begin PostDialog

		return ::CWPostDialog (mContext);

	} // end PostDialog

// ---------------------------------------------------------------------------
//		€ PreFileAction
// ---------------------------------------------------------------------------	

void
VCSV7Context::PreFileAction (

	const CWFileSpec& inFile) const
	
	{ // begin PreFileAction

		CheckOptionalResult (::CWPreFileAction (mContext, &inFile));

	} // end PreFileAction

// ---------------------------------------------------------------------------
//		€ PostFileAction
// ---------------------------------------------------------------------------	

void
VCSV7Context::PostFileAction (

	const CWFileSpec& inFile) const
	
	{ // begin PostFileAction

		CheckOptionalResult (::CWPostFileAction (mContext, &inFile));

	} // end PostFileAction

// ---------------------------------------------------------------------------
//		€ CompletionRatio
// ---------------------------------------------------------------------------	

void
VCSV7Context::CompletionRatio (

	int 	total, 
	int 	completed)
	
	{ // begin CompletionRatio

		CheckOptionalResult (::CWCompletionRatio (mContext, total, completed));

	} // end CompletionRatio

// ---------------------------------------------------------------------------
//		€ ReportProgress
// ---------------------------------------------------------------------------	

void
VCSV7Context::ReportProgress (

	const char *line1, 
	const char *line2)
	
	{ // begin ReportProgress

		CheckResult (::CWShowStatus (mContext, line1, line2));

	} // end ReportProgress

// ---------------------------------------------------------------------------
//		€ CreateDocument
// ---------------------------------------------------------------------------

void
VCSV7Context::CreateDocument (

	const	char*	inName,
	Handle			inData,
	Boolean			inDirty) const
		
	
	{ // begin CreateDocument
		
		const	long			dataSize (::GetHandleSize (inData));
		
		CWMemHandle				cwData = 0;
		CheckResult (::CWAllocMemHandle (mContext, dataSize, true, &cwData));
		
		void*					p = LockMemHandle (cwData);
		::BlockMoveData (*inData, p, dataSize);
		UnlockMemHandle (cwData);
		
		CWNewTextDocumentInfo	docinfo = {inName, cwData, inDirty};

		CheckResult (::CWCreateNewTextDocument (mContext, &docinfo));

	} // end CreateDocument

#pragma mark -

// ---------------------------------------------------------------------------
//		€ CheckIfSupported
// ---------------------------------------------------------------------------

Boolean
VCSV7Context::CheckIfSupported (void) const

	{ // begin CheckIfSupported
	
		Boolean result; 
		
		CheckResult (::CWIsCommandSupportedRequest (mContext, &result)); 
		
		return result;
		
	} // end CheckIfSupported

// ---------------------------------------------------------------------------
//		€ Advanced
// ---------------------------------------------------------------------------

Boolean
VCSV7Context::Advanced (void) const
	
	{ // begin Advanced

		Boolean result;
		CheckResult (::CWIsAdvancedRequest (mContext, &result));
		return result;

	} // end Advanced

// ---------------------------------------------------------------------------
//		€ Recursive
// ---------------------------------------------------------------------------

Boolean
VCSV7Context::Recursive (void) const
	
	{ // begin Recursive

		Boolean result;
		CheckResult (::CWIsRecursiveRequest (mContext, &result));
		return result;

	} // end Recursive

// ---------------------------------------------------------------------------
//		€ GetDatabase
// ---------------------------------------------------------------------------

void
VCSV7Context::GetDatabase (

	CWVCSDatabaseConnection&	dbConnection) const
	
	{ // begin GetDatabase

		CheckResult (::CWGetDatabaseConnectionInfo (mContext, &dbConnection));

	} // end GetDatabase

// ---------------------------------------------------------------------------
//		€ GetItemCount
// ---------------------------------------------------------------------------

unsigned long
VCSV7Context::GetItemCount (void) const
	
	{ // begin GetItemCount

		unsigned long result;
		CheckResult (::CWGetVCSItemCount (mContext, &result));
		return result;

	} // end GetItemCount

// ---------------------------------------------------------------------------
//		€ GetItem
// ---------------------------------------------------------------------------

void
VCSV7Context::GetItem (

	CWVCSItem&	outItem, 
	long 		inItemNo) const
	
	{ // begin 

		CheckResult (::CWGetVCSItem (mContext, inItemNo, &outItem));

	} // end GetItem

// ---------------------------------------------------------------------------
//		€ SetItem
// ---------------------------------------------------------------------------

void
VCSV7Context::SetItem (

	const CWVCSItem& 	inItem, 
	long 				inItemNo)
	
	{ // begin SetItem

		CheckResult (::CWSetVCSItem (mContext, inItemNo, (CWVCSItem*) &inItem));

	} // end SetItem

// ---------------------------------------------------------------------------
//		€ GetComment
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::GetComment (

	const 	char*	pPrompt, 
	char*			pComment, 
	const	long	lBufferSize)
	
	{ // begin GetComment

		//	Kludge to make previous comment available to the user
		ScrapStuff	saveScrap = *::InfoScrap ();
		Boolean		restore = *pComment ? true : false;
		if (restore) {
			::HandToHand (&saveScrap.scrapHandle);
			::ZeroScrap ();
			::PutScrap (::strlen (pComment), 'TEXT', pComment);
			::TEFromScrap ();
			} // if
			
		CWResult	result = ::CWGetComment (mContext, pPrompt, pComment, lBufferSize);

		if (restore) {
			ScrapStuff*	mungeScrap = ::InfoScrap ();
			Size		saveSize = ::GetHandleSize (saveScrap.scrapHandle);
			::SetHandleSize (mungeScrap->scrapHandle, saveSize);
			::BlockMoveData (*saveScrap.scrapHandle, *(mungeScrap->scrapHandle), saveSize);
			mungeScrap->scrapSize = saveScrap.scrapSize;
			mungeScrap->scrapCount = saveScrap.scrapCount;
			mungeScrap->scrapState = saveScrap.scrapState;
			::DisposeHandle (saveScrap.scrapHandle);
			saveScrap.scrapHandle = nil;
			::TEFromScrap ();
			} // if
		
		return result;
		
	} // end GetComment

// ---------------------------------------------------------------------------
//		€ UpdateCheckoutState
// ---------------------------------------------------------------------------

void
VCSV7Context::UpdateCheckoutState (

	const CWFileSpec& 		inItem, 
	CWVCSCheckoutState 		inCheckoutState, 
	const CWVCSVersion& 	version)
	
	{ // begin UpdateCheckoutState

		CheckResult (::CWFileStateChanged (mContext, &inItem, inCheckoutState, version));

	} // end UpdateCheckoutState

// ---------------------------------------------------------------------------
//		€ MessageOutput
// ---------------------------------------------------------------------------

void
VCSV7Context::MessageOutput (

	short 			errorlevel, 
	const char*		line1, 
	const char*		line2, 
	long 			errorNumber)
	
	{ // begin MessageOutput

		CheckResult (::CWReportMessage (mContext, 0, line1, line2, errorlevel, errorNumber));

	} // end MessageOutput

// ---------------------------------------------------------------------------
//		€ VisualDifference
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::VisualDifference (

	const CWFileSpec& file1, 
	const CWFileSpec& file2) const
	
	{ // begin VisualDifference

		return ::CWDoVisualDifference (mContext, &file1, 0, 0, 0, &file2, 0, 0, 0);

	} // end VisualDifference
	
// ---------------------------------------------------------------------------
//		€ VisualDifference
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::VisualDifference (

	const char*			file1Name, 
	const char*			file1Text, 
	unsigned	long	file1TextLength, 
	const CWFileSpec& 	file2) const
	
	{ // begin VisualDifference

		return ::CWDoVisualDifference (mContext, 0, file1Name, file1Text, file1TextLength, &file2, 0, 0, 0);

	} // end VisualDifference
#pragma mark -

// ---------------------------------------------------------------------------
//		€ OnAEIdle
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::OnAEIdle (

	EventRecord&				theEvent, 
	long&						sleepTime, 
	RgnHandle&					mouseRgn)
	
	{ // begin OnAEIdle

		switch (theEvent.what) {
			case nullEvent:
				mouseRgn = nil;
				sleepTime = 6;
				//return gPB->pVCSYieldTimeRoutine ();
				break;
			} // switch
		
		return cwNoErr;

	} // end OnAEIdle
	
