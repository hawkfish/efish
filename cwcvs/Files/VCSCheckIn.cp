#include "VCSCheckIn.h"

#include "VCSError.h"
#include "VCSPrefs.h"
#include "VCSPrompt.h"
#include "VCSResult.h"
#include "VCSTask.h"
#include "VCSUtil.h"
#include "VCSVersion.h"

#include "CVSCommand.h"

#include <TextUtils.h>

//	=== Constants ===

static const unsigned char
sCheckinTag [] = "\pChecking in ";

static const unsigned char
sArrowTag [] = "\p<--";

static const unsigned char
sRevisionTag [] = "\prevision: ";

static const unsigned char
sDoneTag [] = "\pdone";

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ VCSCheckIn
// ---------------------------------------------------------------------------

VCSCheckIn::VCSCheckIn (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, true)
		
	{ // begin VCSCheckIn
		
	} // end VCSCheckIn

// ---------------------------------------------------------------------------
//		¥ ~VCSCheckIn
// ---------------------------------------------------------------------------

VCSCheckIn::~VCSCheckIn (void)
	
	{ // begin ~VCSCheckIn
		
	} // end ~VCSCheckIn

// ---------------------------------------------------------------------------
//		¥ ParseResult
// ---------------------------------------------------------------------------

OSErr
VCSCheckIn::ParseResult (

	const	FSSpec&			root,
	Handle					output,
	Handle					file)
	
	{ // begin ParseResult
		
		OSErr				e = noErr;
		
		Handle				line = nil;

		Str255				name;
		CWFileSpec			item;
		CWVCSVersion		version;

		const	char		slash = '/';
		const	char		colon = ':';
		
		if (0 == GetHandleSize (output)) goto CleanUp;
		
		if (noErr != (e = GetNextLine (&line, output))) goto CleanUp;
		if (0 > Munger (line, 0, sArrowTag + 1, sArrowTag[0], nil, 0)) goto CleanUp;
		DisposeHandle (line);
		line = nil;
		
		if (noErr != (e = GetNextLine (&line, output))) goto CleanUp;
		if (0 > Munger (line, 0, sRevisionTag + 1, sRevisionTag[0], nil, 0)) goto CleanUp;
		DisposeHandle (line);
		line = nil;
		
		if (noErr != (e = GetNextLine (&line, output))) goto CleanUp;
		if (0 > Munger (line, 0, sDoneTag + 1, sDoneTag[0], nil, 0)) goto CleanUp;
		DisposeHandle (line);
		line = nil;

		//	Build the FSSpec
		Munger (file, 0, 0, 0, &colon, 1);
		Munger (file, 0, 0, 0, root.name + 1, root.name[0]);
		Munger (file, 0, 0, 0, &colon, 1);
		while (0 <= Munger (file, 0, &slash, sizeof (slash), &colon, sizeof (colon)))
			continue;
		
		BlockMoveData (*file, name + 1, name[0] = GetHandleSize (file));
		if (noErr != (e = FSMakeFSSpec (root.vRefNum, root.parID, name, &item))) goto CleanUp;
		
		//	Build the version
		version.eVersionForm = cwVersionFormNone;
		
		//	Update the IDE
		mContext.UpdateCheckoutState (item, cwCheckoutStateNotCheckedOut, version);

	CleanUp:
	
		if (line) DisposeHandle (line);
		line = nil;
		
		return e;
		
	} // end ParseResult
	
// ---------------------------------------------------------------------------
//		¥ ParseOutput
// ---------------------------------------------------------------------------

CWVCSItemStatus
VCSCheckIn::ParseOutput (

	const	FSSpec&			root,
	Handle					output)
	
	{ // begin ParseOutput
		
		CWVCSItemStatus	result = cwItemStatusSucceeded;
		
		const	char	semi = ';';
		
		while (0 != GetHandleSize (output)) {
			Handle	line = nil;

			if (noErr != VCSRaiseOSErr (mContext, GetNextLine (&line, output))) {
				result = cwItemStatusFailed;
				goto CleanUp;
				} // if
				
			if (noErr != VCSCheckCmdOutputLine (mContext, "\pcommit", line)) 
				result = cwItemStatusFailed;
			
			else {
				long	tagPos = Munger (line, 0, sCheckinTag + 1, sCheckinTag[0], nil, 0);
				if (0 <= tagPos) {
					Munger (line, 0, nil, tagPos + sCheckinTag[0], &semi, 0);
					Munger (line, 0, &semi, sizeof (semi), &semi, 0);
					ParseResult (root, output, line);
					} // if
				} // else
				
			DisposeHandle (line);
			line = nil;
			} // while
		
		CleanUp:
		
			return result;
			
	} // end ParseOutput

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFolder
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSCheckIn::ProcessRegularFolder (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFolder

		FSSpec		cwd = inItem.fsItem;
		Str255		comment;
		
		//	Stuff to clean up
		AEDesc 		command = {typeNull, nil};
		Handle		output = nil;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kCheckInTask, inItem.fsItem.name);
		
		// Get a comment from the user
		comment[0] = 0;
		if (!VCSPromptComment (mContext, kCheckinCommentPrompt, inItem.fsItem.name, comment)) {
			inItem.eItemStatus = cwItemStatusCancelled;
			goto CleanUp;
			} // if
			
		//	cvs -r commit -R -F <comment>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-r"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "commit"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "-R"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCommentArg (&command, comment))) goto CleanUp;
		if (noErr != (VCSSendOutputCommand (mContext, &command, &cwd, &output))) goto CleanUp;
		
		//	Update status
		CWVCSDatabaseConnection	db;
		mContext.GetDatabase (db);
		inItem.eItemStatus = ParseOutput (db.sProjectRoot, output);
		
	CleanUp:
		
		if (output != nil) DisposeHandle ((Handle) output);
		output = nil;

		AEDisposeDesc (&command);

		return inItem.eItemStatus;
		
	} // end ProcessRegularFolder

// ---------------------------------------------------------------------------
//		¥ ProcessRegularFile
// ---------------------------------------------------------------------------

CWVCSItemStatus 
VCSCheckIn::ProcessRegularFile (
	
	CWVCSItem& 		inItem)

	{ // begin ProcessRegularFile
		
		FSSpec		cwd = inItem.fsItem;
		Str255		comment;
		
		//	Stuff to clean up
		AEDesc 		command = {typeNull, nil};
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kCheckInTask, inItem.fsItem.name);
		
		//	Get the cwd for update
		if (noErr != VCSRaiseOSErr (mContext, ::FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) goto CleanUp;

		//	Get the checkin comment
		comment[0] = 0;
		if (!VCSPromptComment (mContext, kCheckinCommentPrompt, inItem.fsItem.name, comment)) {
			inItem.eItemStatus = cwItemStatusCancelled;
			goto CleanUp;
			} // if
			
		//	cvs commit -m <comment> <file>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-r"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "commit"))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCommentArg (&command, comment))) goto CleanUp;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) goto CleanUp;
		
		// send the command to MacCVS
		if (noErr != (VCSSendCommand (mContext, &command, &cwd))) goto CleanUp;
		
		//	Update status
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
			
	CleanUp:
		
		AEDisposeDesc (&command);

		return inItem.eItemStatus;
		
	} // end ProcessRegularFile

