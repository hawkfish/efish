#include "VCSV7Context.h"

// ---------------------------------------------------------------------------
//		¥ CheckResult
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::CheckResult (

	CWResult	inResult)
		
	{ // begin CheckResult
		
		if (inResult != cwNoErr) throw inResult;
		
		return inResult;
		
	} // end CheckResult

// ---------------------------------------------------------------------------
//		¥ CheckOptionalResult
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
//		¥ VCSV7Context
// ---------------------------------------------------------------------------

VCSV7Context::VCSV7Context (

	CWPluginContext	inContext)
	
	: mContext (inContext)
	
	{ // begin VCSV7Context
		
	} // end VCSV7Context

// ---------------------------------------------------------------------------
//		¥ ~VCSV7Context
// ---------------------------------------------------------------------------

VCSV7Context::~VCSV7Context (void)
	
	{ // begin ~VCSV7Context
		
	} // end VCSV7Context

// ---------------------------------------------------------------------------
//		¥ GetAPIVersion
// ---------------------------------------------------------------------------

long
VCSV7Context::GetAPIVersion (void) const
	
	{ // begin GetAPIVersion
	
		long result; 
		CheckResult (::CWGetAPIVersion (mContext, &result)); 
		return result;
		
	} // end GetAPIVersion

// ---------------------------------------------------------------------------
//		¥ GetIDEVersion
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::GetIDEVersion (

	CWIDEInfo&	outInfo) const
	
	{ // begin GetIDEVersion
	
		return ::CWGetIDEInfo (mContext, &outInfo);
		
	} // end GetIDEVersion

// ---------------------------------------------------------------------------
//		¥ YieldTime
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::YieldTime (void) const
	
	{ // begin YieldTime

		return ::CWUserBreak (mContext);

	} // end YieldTime

// ---------------------------------------------------------------------------
//		¥ GetRequest
// ---------------------------------------------------------------------------
	
long
VCSV7Context::GetRequest (void) const
	
	{ // begin GetRequest

		long result; 
		CheckResult (::CWGetPluginRequest (mContext, &result));
		return result;

	} // end GetRequest

// ---------------------------------------------------------------------------
//		¥ GetNamedPreferences
// ---------------------------------------------------------------------------

CWMemHandle
VCSV7Context::GetNamedPreferences (

	const char* prefsname) const
	
	{ // begin GetNamedPreferences

		CWMemHandle result; CheckResult (::CWGetNamedPreferences (mContext, prefsname, &result));
		return result;

	} // end GetNamedPreferences

// ---------------------------------------------------------------------------
//		¥ GetProjectFile
// ---------------------------------------------------------------------------

void
VCSV7Context::GetProjectFile (

	CWFileSpec& outProjectSpec) const
	
	{ // begin GetProjectFile

		CheckResult (::CWGetProjectFile (mContext, &outProjectSpec));

	} // end GetProjectFile

// ---------------------------------------------------------------------------
//		¥ LockMemHandle
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
//		¥ UnlockMemHandle
// ---------------------------------------------------------------------------

void
VCSV7Context::UnlockMemHandle (

	CWMemHandle inData) const
	
	{ // begin UnlockMemHandle

		CheckResult (::CWUnlockMemHandle (mContext, inData));

	} // end UnlockMemHandle

// ---------------------------------------------------------------------------
//		¥ GetCommandStatus
// ---------------------------------------------------------------------------	

CWVCSCommandStatus
VCSV7Context::GetCommandStatus (void) const
	
	{ // begin GetCommandStatus

		CWVCSCommandStatus result;
		CheckResult (::CWGetCommandStatus (mContext, &result));
		return result;

	} // end GetCommandStatus

// ---------------------------------------------------------------------------
//		¥ SetCommandStatus
// ---------------------------------------------------------------------------	

void
VCSV7Context::SetCommandStatus (

	CWVCSCommandStatus inStatus)
	
	{ // begin SetCommandStatus

		CheckResult (::CWSetCommandStatus (mContext, inStatus));

	} // end SetCommandStatus

// ---------------------------------------------------------------------------
//		¥ PreDialog
// ---------------------------------------------------------------------------	

CWResult
VCSV7Context::PreDialog (void) const
	
	{ // begin PreDialog

		return ::CWPreDialog (mContext);

	} // end PreDialog

// ---------------------------------------------------------------------------
//		¥ PostDialog
// ---------------------------------------------------------------------------	

CWResult
VCSV7Context::PostDialog (void) const
	
	{ // begin PostDialog

		return ::CWPostDialog (mContext);

	} // end PostDialog

// ---------------------------------------------------------------------------
//		¥ PreFileAction
// ---------------------------------------------------------------------------	

void
VCSV7Context::PreFileAction (

	const CWFileSpec& inFile) const
	
	{ // begin PreFileAction

		CheckOptionalResult (::CWPreFileAction (mContext, &inFile));

	} // end PreFileAction

// ---------------------------------------------------------------------------
//		¥ PostFileAction
// ---------------------------------------------------------------------------	

void
VCSV7Context::PostFileAction (

	const CWFileSpec& inFile) const
	
	{ // begin PostFileAction

		CheckOptionalResult (::CWPostFileAction (mContext, &inFile));

	} // end PostFileAction

// ---------------------------------------------------------------------------
//		¥ CompletionRatio
// ---------------------------------------------------------------------------	

void
VCSV7Context::CompletionRatio (

	int 	total, 
	int 	completed) const
	
	{ // begin CompletionRatio

		CheckOptionalResult (::CWCompletionRatio (mContext, total, completed));

	} // end CompletionRatio

// ---------------------------------------------------------------------------
//		¥ ReportProgress
// ---------------------------------------------------------------------------	

void
VCSV7Context::ReportProgress (

	const char *line1, 
	const char *line2) const
	
	{ // begin ReportProgress

		CheckResult (::CWShowStatus (mContext, line1, line2));

	} // end ReportProgress

// ---------------------------------------------------------------------------
//		¥ CreateDocument
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
//		¥ CheckIfSupported
// ---------------------------------------------------------------------------

Boolean
VCSV7Context::CheckIfSupported (void) const

	{ // begin CheckIfSupported
	
		Boolean result; 
		
		CheckResult (::CWIsCommandSupportedRequest (mContext, &result)); 
		
		return result;
		
	} // end CheckIfSupported

// ---------------------------------------------------------------------------
//		¥ Advanced
// ---------------------------------------------------------------------------

Boolean
VCSV7Context::Advanced (void) const
	
	{ // begin Advanced

		Boolean result;
		CheckResult (::CWIsAdvancedRequest (mContext, &result));
		return result;

	} // end Advanced

// ---------------------------------------------------------------------------
//		¥ Recursive
// ---------------------------------------------------------------------------

Boolean
VCSV7Context::Recursive (void) const
	
	{ // begin Recursive

		Boolean result;
		CheckResult (::CWIsRecursiveRequest (mContext, &result));
		return result;

	} // end Recursive

// ---------------------------------------------------------------------------
//		¥ GetDatabase
// ---------------------------------------------------------------------------

void
VCSV7Context::GetDatabase (

	CWVCSDatabaseConnection&	dbConnection) const
	
	{ // begin GetDatabase

		CheckResult (::CWGetDatabaseConnectionInfo (mContext, &dbConnection));

	} // end GetDatabase

// ---------------------------------------------------------------------------
//		¥ GetItemCount
// ---------------------------------------------------------------------------

unsigned long
VCSV7Context::GetItemCount (void) const
	
	{ // begin GetItemCount

		unsigned long result;
		CheckResult (::CWGetVCSItemCount (mContext, &result));
		return result;

	} // end GetItemCount

// ---------------------------------------------------------------------------
//		¥ GetItem
// ---------------------------------------------------------------------------

void
VCSV7Context::GetItem (

	CWVCSItem&	outItem, 
	long 		inItemNo) const
	
	{ // begin 

		CheckResult (::CWGetVCSItem (mContext, inItemNo, &outItem));

	} // end GetItem

// ---------------------------------------------------------------------------
//		¥ SetItem
// ---------------------------------------------------------------------------

void
VCSV7Context::SetItem (

	const CWVCSItem& 	inItem, 
	long 				inItemNo)
	
	{ // begin SetItem

		CheckResult (::CWSetVCSItem (mContext, inItemNo, (CWVCSItem*) &inItem));

	} // end SetItem

// ---------------------------------------------------------------------------
//		¥ GetComment
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::GetComment (

	const 	char*	pPrompt, 
	char*			pComment, 
	const	long	lBufferSize) const
	
	{ // begin GetComment

		return ::CWGetComment (mContext, pPrompt, pComment, lBufferSize);

	} // end GetComment

// ---------------------------------------------------------------------------
//		¥ UpdateCheckoutState
// ---------------------------------------------------------------------------

void
VCSV7Context::UpdateCheckoutState (

	const CWFileSpec& 		inItem, 
	CWVCSCheckoutState 		inCheckoutState, 
	const CWVCSVersion& 	version) const
	
	{ // begin UpdateCheckoutState

		CheckResult (::CWFileStateChanged (mContext, &inItem, inCheckoutState, version));

	} // end UpdateCheckoutState

// ---------------------------------------------------------------------------
//		¥ MessageOutput
// ---------------------------------------------------------------------------

void
VCSV7Context::MessageOutput (

	short 			errorlevel, 
	const char*		line1, 
	const char*		line2, 
	long 			errorNumber) const
	
	{ // begin MessageOutput

		CheckResult (::CWReportMessage (mContext, 0, line1, line2, errorlevel, errorNumber));

	} // end MessageOutput

// ---------------------------------------------------------------------------
//		¥ VisualDifference
// ---------------------------------------------------------------------------

CWResult
VCSV7Context::VisualDifference (

	const CWFileSpec& file1, 
	const CWFileSpec& file2) const
	
	{ // begin VisualDifference

		return ::CWDoVisualDifference (mContext, &file1, 0, 0, 0, &file2, 0, 0, 0);

	} // end VisualDifference
	
// ---------------------------------------------------------------------------
//		¥ VisualDifference
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
