#include "VCSResult.h"

#include "VCSError.h"
#include "VCSTask.h"
#include "VCSUtil.h"

#include <Devices.h>
#include <Files.h>
#include <TextUtils.h>

// ---------------------------------------------------------------------------
//		´ PBWaitNextEvent
// ---------------------------------------------------------------------------

static OSErr 
PBWaitNextEvent (

	IOParam*	io)
	
	{ // begin PBWaitNextEvent
	
		while (io->ioResult == 1) {
			EventRecord theEvent;
			WaitNextEvent (0, &theEvent, 6, nil);
			} // while
		
		return io->ioResult;
		
	} // end PBWaitNextEvent
	
// ---------------------------------------------------------------------------
//		´ GetNextString
// ---------------------------------------------------------------------------

OSErr
GetNextString (

	Handle*	outLine,
	Handle	inText,
	char	inDelim)
	
	{ // begin GetNextString
		
		OSErr			e = noErr;
		
		long			lOffset = Munger (inText, 0, &inDelim, 1, nil, 0);   /* IM I pg 468 */
		
		if (lOffset < 0) return 1;

		*outLine = NewHandle (lOffset);
		if (noErr != (e = MemError ())) goto CleanUp;
		
		BlockMoveData (*inText, **outLine, lOffset);
		Munger (inText, 0, 0, lOffset + 1, &inDelim, 0);
		if (noErr != (e = MemError ())) goto CleanUp;
	
	CleanUp:
	
		return e;
		
	} // end GetNextString
	
// ---------------------------------------------------------------------------
//		´ GetNextLine
// ---------------------------------------------------------------------------

OSErr
GetNextLine (

	Handle*	outLine,
	Handle	inText)
	
	{ // begin GetNextLine
		
		const	char	lf = 0x0A;
		const	char	cr = 0x0D;
		
		Munger (inText, 0, &lf, sizeof (lf), &cr, sizeof (cr));

		return GetNextString (outLine, inText, cr);
		
	} // end GetNextLine
	
// ---------------------------------------------------------------------------
//		´ ReadFileContents
// ---------------------------------------------------------------------------

OSErr 
ReadFileContents (
	
	Handle*				outData,	
	const	FSSpec*		inSpec)
	
	{ // begin ReadFileContents
	
		OSErr	e = noErr;
		
		short	refNum = -1;
		
		Handle	data = nil;
		Size	dataSize;
		
		IOParam io;
		
		//	Open the file
		if (noErr != (e = FSpOpenDF (inSpec, fsRdPerm, &refNum))) goto CleanUp;
		
		///	Make the datahandle
		if (noErr != (e = GetEOF (refNum, &dataSize))) goto CleanUp;
		data = NewHandle (dataSize);
		if (noErr != (e = MemError ())) goto CleanUp;
		
		//	Read the data
		HLock (data);
		io.ioCompletion = 0;
		io.ioRefNum = refNum;
		io.ioBuffer = *data;
		io.ioReqCount = dataSize;
		io.ioPosMode = fsFromStart;
		io.ioPosOffset = 0;
		PBReadAsync ((ParmBlkPtr) &io);
		e = PBWaitNextEvent (&io);
		HUnlock (data);
		if (noErr != e) goto CleanUp;
		
		*outData = data;
		data = nil;
		
	CleanUp:
		
		if (noErr != e) {
			if (data != nil) DisposeHandle (data);
			data = nil;
			} // if
			
		if (refNum != -1) FSClose (refNum);
		refNum = -1;

		return e;

	} // end ReadFileContents

// ---------------------------------------------------------------------------
//		´ FSpEqual
// ---------------------------------------------------------------------------

Boolean
FSpEqual (

	const	FSSpec*		spec1,
	const	FSSpec*		spec2)

	{ // begin FSpEqual
	
		if (spec1->vRefNum != spec2->vRefNum) return false;
		if (spec1->parID != spec2->parID) return false;
		
		return ::EqualString (spec1->name, spec2->name, false, false);
			
	} // end FSpEqual
	
// ---------------------------------------------------------------------------
//		´ VCSUpdateFileStatus
// ---------------------------------------------------------------------------

OSErr
VCSUpdateFileStatus (

	const VCSContext&		inPB,
	const	FSSpec*			root,
	const	FSSpec*			project,
	Handle					line)
	
	{ // begin VCSUpdateFileStatus
		
		OSErr				e = noErr;
		
		const	char		slash = '/';
		const	char		colon = ':';
		
		Str255				name;
		FSSpec				item;
		CWVCSVersion		version;
		CWVCSCheckoutState	state;
		
		//	Make sure there is data
		if (GetHandleSize (line) == 0) return noErr;
		
		//	Determine the status
		switch (**line) {
			case '?':
			case 'R':
				state = cwCheckoutStateNotInDatabase;
				break;
				
			case 'U':
			case 'A':
			case 'P':
				state = cwCheckoutStateNotCheckedOut;
				break;

			case 'M':
			case 'C':
				state = cwCheckoutStateCheckedOut;
				break;
				
			default:
				return noErr;
			} // switch
				
		//	Build the FSSpec
		Munger (line, 0, 0, 2, &colon, 1);
		while (0 <= Munger (line, 0, &slash, sizeof (slash), &colon, sizeof (colon)))
			continue;
		
		BlockMoveData (*line, name + 1, name[0] = GetHandleSize (line));
		if (noErr != (e = FSMakeFSSpec (root->vRefNum, root->parID, name, &item))) goto CleanUp;
		SetHandleSize (line, 0);
		
		//	Build the version
		version.eVersionForm = cwVersionFormNone;
		
		//	Check for project locking
		if ((state == cwCheckoutStateNotCheckedOut) && ::FSpEqual (&item, project))
			::FSpSetFLock (project);
			
		//	Update the IDE
		inPB.UpdateCheckoutState (item, state, version);
	
	CleanUp:
	
		return e;
		
	} // end VCSUpdateFileStatus

// ---------------------------------------------------------------------------
//		´ VCSDisplayResult
// ---------------------------------------------------------------------------

void 
VCSDisplayResult (

	const VCSContext&		inPB,
	short					inSeverity,
	short					inStringID,
	short					inStringIndex,
	Handle					message)
	
	{ // begin VCSDisplayResult
			
		Str255		label;
		SignedByte	saveState = HGetState (message);
		char*		pLineOne = 0;
		char*		pLineTwo = *message;
		char		null = 0;
		
		GetIndString (label, inStringID, inStringIndex);
		pLineOne = p2cstr ((StringPtr) label);
		
		PtrAndHand (&null, message, sizeof (null));
		MoveHHi (message);
		HLock (message);
		pLineTwo = *message;
		
		inPB.MessageOutput (inSeverity, pLineOne, pLineTwo);

		HSetState (message, saveState);
		c2pstr (pLineOne);
	
	} // end VCSDisplayResult
	
// ---------------------------------------------------------------------------
//		´ VCSDisplayError
// ---------------------------------------------------------------------------

void 
VCSDisplayError (

	const VCSContext&		inPB,
	short					inSeverity,
	short					inStringID,
	short					inStringIndex,
	Handle					message)
	
	{ // begin VCSDisplayError
		
		Str255			taskName;
		Str255			label;
		
		char*			pLineTwo = 0;
		const	char	null = 0;
		
		char*			pLineOne = 0;
		SignedByte		saveState = HGetState (message);

		ReplaceInIndString (label, inStringID, inStringIndex, VCSTask::GetTaskName (taskName));	
		pLineOne = p2cstr ((StringPtr) label);
		
		PtrAndHand (&null, message, sizeof (null));
		MoveHHi (message);
		HLock (message);
		pLineTwo = *message;
		
		inPB.MessageOutput (inSeverity, pLineOne, pLineTwo);

		HSetState (message, saveState);
		c2pstr (pLineOne);
	
	} // end VCSDisplayError
	
// ---------------------------------------------------------------------------
//		¥ VCSCheckCmdOutputLine
// ---------------------------------------------------------------------------

OSErr
VCSCheckCmdOutputLine (

	const VCSContext&		inPB,
	ConstStr255Param		inCmd,
	Handle					line)
	
	{ // begin VCSCheckCmdOutputLine
		
		Str255	aborted;
		
		//	cvs [<cmd> aborted]: <message>
		ReplaceInIndString (aborted, kErrorStrings, kAbortedErr, inCmd, nil, nil, nil);
		if (0 > Munger (line, 0, aborted + 1, aborted[0], nil, 0)) return noErr;
		
		VCSDisplayError (inPB, messagetypeError, kErrorStrings, kShortTaskErr, line);
		
		return 1;
		
	} // end VCSCheckCmdOutputLine
	
// ---------------------------------------------------------------------------
//		¥ VCSCheckCmdOutput
// ---------------------------------------------------------------------------

OSErr
VCSCheckCmdOutput (

	const VCSContext&		inPB,
	ConstStr255Param		inCmd,
	Handle					output)
	
	{ // begin VCSCheckCmdOutput
		
		OSErr	e = noErr;
		
		Boolean	found = false;
		Handle	copy = output;
		
		if (noErr != (e = VCSRaiseOSErr (inPB, HandToHand (&copy)))) {
			copy = nil;
			goto CleanUp;
			} // if
			
		while (GetHandleSize (copy)) {
			Handle	line = nil;
			if (noErr != (e = VCSRaiseOSErr (inPB, GetNextLine (&line, copy)))) goto CleanUp;
			
			if (VCSCheckCmdOutputLine (inPB, inCmd, line)) found = true;
				
			DisposeHandle (line);
			line = nil;
			} // while
		
		if (found) e = 1;
		
	CleanUp:
		
		if (copy) DisposeHandle (copy);
		copy = nil;
		
		return e;
		
	} // end VCSCheckCmdOutput
	
