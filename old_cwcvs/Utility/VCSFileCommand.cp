/*
	File:		VCSFileCommand.cp

	Contains:	Implementation of the generic file handling object.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>    10/26/98    rmgw    Abort recursion only on cancel.
         <1>    10/14/98    rmgw    first checked in.
*/


#include "VCSFileCommand.h"

#include "IterateDirectory.h"

#include <Errors.h>

//	=== Static Members ===

UInt16
VCSFileCommand::sMaxRecursion = 16;

// ---------------------------------------------------------------------------
//		¥ VCSContext
// ---------------------------------------------------------------------------

VCSFileCommand::VCSFileCommand (

	VCSContext&	inContext,
	Boolean		inHasCommand,
	Boolean		inHasRecursive,
	Boolean		inHasAdvanced,
	Boolean		inHasAdvancedRecursive)
	
	: VCSCommand (inContext, inHasCommand, inHasRecursive, inHasAdvanced, inHasAdvancedRecursive)
	
	{ // begin VCSFileCommand
		
	} // end VCSFileCommand

// ---------------------------------------------------------------------------
//		¥ ~VCSFileCommand
// ---------------------------------------------------------------------------

VCSFileCommand::~VCSFileCommand (void)
	
	{ // begin ~VCSFileCommand
				
	} // end ~VCSFileCommand

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ IterateFile
// ---------------------------------------------------------------------------

void 
VCSFileCommand::IterateFile (

	const CInfoPBRec& 	 	cpbPtr,
	long					dirID,
	Boolean&				quitFlag)

	{ // begin IterateFile
		
		CWVCSItem						item;
		if (noErr != ::FSMakeFSSpec (cpbPtr.hFileInfo.ioVRefNum, dirID, cpbPtr.hFileInfo.ioNamePtr, &item.fsItem)) return;
		
		if (ProcessFile (item) == cwItemStatusCancelled) quitFlag = true;
		
	} // end IterateFile
	
// ---------------------------------------------------------------------------
//		¥ IterateFolder
// ---------------------------------------------------------------------------

void 
VCSFileCommand::IterateFolder (

	const CInfoPBRec& 	 	/* cpbPtr */,
	long					/* dirID */,
	Boolean&				/* quitFlag */)

	{ // begin IterateFolder
		
	} // end IterateFolder
	
// ---------------------------------------------------------------------------
//		¥ IterateFilter
// ---------------------------------------------------------------------------

void 
VCSFileCommand::IterateFilter (

	const CInfoPBRec& 	 	cpbPtr,
	long					dirID,
	Boolean&				quitFlag)

	{ // begin IterateFilter
		
		if ((cpbPtr.hFileInfo.ioFlAttrib & ioDirMask) != 0) 
			IterateFolder (cpbPtr, dirID, quitFlag);
		else IterateFile (cpbPtr, dirID, quitFlag);

		if (mContext.YieldTime ()) quitFlag = true;
		
	} // end IterateFilter
	
// ---------------------------------------------------------------------------
//		¥ IterateProc
// ---------------------------------------------------------------------------

pascal void 
VCSFileCommand::IterateProc (

	const CInfoPBRec * const 	cpbPtr,
	long						dirID,
	Boolean*					quitFlag,
	void*						yourDataPtr)

	{ // begin IterateProc
				
		VCSFileCommand*			that = (VCSFileCommand*) yourDataPtr;
		that->IterateFilter (*cpbPtr, dirID, *quitFlag);
		
		return;

	} // end IterateProc
	
#pragma mark -

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSFileCommand::ProcessRegularFile (

	CWVCSItem&	/*inItem*/)
	
	{ // begin ProcessRegularFile
		
		return cwItemStatusSucceeded;
		
	} // end ProcessRegularFile
	
// ---------------------------------------------------------------------------
//		¥ ProcessAdvancedFile
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSFileCommand::ProcessAdvancedFile (

	CWVCSItem&	inItem)
	
	{ // begin ProcessAdvancedFile
		
		return ProcessRegularFile (inItem);
		
	} // end ProcessAdvancedFile
	
// ---------------------------------------------------------------------------
//		¥ ProcessFile
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSFileCommand::ProcessFile (

	CWVCSItem&	inItem)
	
	{ // begin ProcessFile
		
		return mContext.Advanced ()
				? ProcessAdvancedFile (inItem)
				: ProcessRegularFile (inItem);
			
	} // end ProcessFile

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSFileCommand::ProcessRegularFolder (

	CWVCSItem&	inItem)
	
	{ // begin ProcessRegularFolder

		OSErr				e = noErr;

		inItem.eItemStatus = cwItemStatusFailed;
		if (noErr != (e = ::FSpIterateDirectory (&inItem.fsItem, sMaxRecursion, IterateProc, this))) {
			if (e == userCanceledErr) inItem.eItemStatus = cwItemStatusCancelled;
			goto CleanUp;
			} // if

		inItem.eItemStatus = cwItemStatusSucceeded;
	
	CleanUp:
	
		return inItem.eItemStatus;
		
	} // end ProcessRegularFolder
	
// ---------------------------------------------------------------------------
//		¥ ProcessAdvancedFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSFileCommand::ProcessAdvancedFolder (

	CWVCSItem&	inItem)
	
	{ // begin ProcessAdvancedFolder
		
		return ProcessRegularFolder (inItem);
		
	} // end ProcessAdvancedFolder
	
// ---------------------------------------------------------------------------
//		¥ ProcessFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSFileCommand::ProcessFolder (

	CWVCSItem&	inItem)
	
	{ // begin ProcessFolder
		
		return mContext.Advanced ()
				? ProcessAdvancedFolder (inItem)
				: ProcessRegularFolder (inItem);
			
	} // end ProcessFolder
	
#pragma mark -

// ---------------------------------------------------------------------------
//		¥ ProcessItem
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSFileCommand::ProcessItem (

	CWVCSItem&	inItem)
	
	{ // begin ProcessItem
		
		return mContext.Recursive ()
				? ProcessFolder (inItem)
				: ProcessFile (inItem);
			
	} // end ProcessItem
	
// ---------------------------------------------------------------------------
//		¥ DoRequest
// ---------------------------------------------------------------------------

CWVCSCommandStatus 
VCSFileCommand::DoRequest (void)
	
	{ // begin DoRequest
			
		CWVCSCommandStatus	status = cwCommandStatusSucceeded;
		Boolean				advanced (mContext.Advanced ());
		Boolean				recursive (mContext.Recursive ());
		
		unsigned	long	total (mContext.GetItemCount ());
		mContext.CompletionRatio (total, 0);
		for (long i = 0; i < total; ++i) {
			CWVCSItem	item;
			
			mContext.GetItem (item, i);
			//if (!recursive) mContext.PreFileAction (item.fsItem);
			ProcessItem (item);
			//if (!recursive) mContext.PostFileAction (item.fsItem);
			mContext.SetItem (item, i);
			
			switch (item.eItemStatus) {
				case cwItemStatusCancelled:
					return cwCommandStatusCancelled;
					
				case cwItemStatusFailed:
					status = cwCommandStatusPartial;
					break;
				} // switch
			
			mContext.CompletionRatio (total, i + 1);
			if (mContext.YieldTime ()) return cwCommandStatusCancelled;
			} // for
			
		/* return result code */
		return status;
		
	} // end DoRequest

