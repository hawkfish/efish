#include "VCSCheckIn.h"

#include "VCSAdvancedOptionsDialog.h"
#include "VCSDialogCheckboxItem.h"
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

//	=== Types ===

class CVSCheckinOptionsDialog : public VCSAdvancedOptionsDialog 

	{
	
	protected:
		
		VCSDialogTextItem		mRevisionItem;
		VCSDialogCheckboxItem	mNoRunItem;
		
	public:
		
		enum {
			kPromptItem = cancel + 1,
			kRevisionLabelItem,
			kRevisionItem,
			kNoRunItem,
			
			kResourceID = 16200
			};
	
						CVSCheckinOptionsDialog		(const VCSContext&		mContext,
													 short					inDLOGid);
		virtual			~CVSCheckinOptionsDialog	(void);
		
		static	OSErr	DoDialog					(const 	VCSContext&		inContext,
													 const	FSSpec&			inFile,
													 const	AEDescList&		inDefaults,
													 AEDescList&			outOptions);
		static	OSErr 	GetOptions					(const 	VCSContext&		inContext,
													 const	FSSpec&			inFile,
													 AEDescList&			outOptions);
	};

//	===	Class Members ===

// ---------------------------------------------------------------------------
//		¥ CVSCheckinOptionsDialog
// ---------------------------------------------------------------------------

CVSCheckinOptionsDialog::CVSCheckinOptionsDialog (

	const VCSContext&	mContext,
	short				inDLOGid)
	
	: VCSAdvancedOptionsDialog (mContext, inDLOGid)
	
	, mRevisionItem (*this, kRevisionItem)
	, mNoRunItem (*this, kNoRunItem)
	
	{ // begin CVSCheckinOptionsDialog
	
	} // end CVSCheckinOptionsDialog
	
// ---------------------------------------------------------------------------
//		¥ ~CVSCheckinOptionsDialog
// ---------------------------------------------------------------------------

CVSCheckinOptionsDialog::~CVSCheckinOptionsDialog (void)

	{ // begin ~CVSCheckinOptionsDialog
	
	} // end ~CVSCheckinOptionsDialog

// ---------------------------------------------------------------------------
//		¥ DoDialog
// ---------------------------------------------------------------------------

OSErr 
CVSCheckinOptionsDialog::DoDialog (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	const	AEDescList&		inDefaults,
	AEDescList&				outOptions)
	
	{ // begin DoDialog
		
		OSErr	e = noErr;

		VCSDialog::SetParamText (inFile.name);
		CVSCheckinOptionsDialog	d (inPB, kResourceID);
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
CVSCheckinOptionsDialog::GetOptions (
	
	const 	VCSContext&		inPB,
	const	FSSpec&			inFile,
	AEDescList&				outOptions)
	
	{ // begin GetOptions
		
		OSErr				e = noErr;
		
		//	Get the defaults
		AEDescList			defaultList = {typeNull, nil};
		if (noErr != (e = ::AECreateList (nil, 0 , false, &defaultList))) return e;
			
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

//	=== Constants ===

static const unsigned char
sCheckinTag [] = "\pChecking in ";

static const unsigned char
sArrowTag [] = "\p<--";

static const unsigned char
sRevisionTag [] = "\prevision: ";

static const unsigned char
sDoneTag [] = "\pdone";

// ---------------------------------------------------------------------------
//		¥ VCSCheckIn
// ---------------------------------------------------------------------------

VCSCheckIn::VCSCheckIn (

	VCSContext&	inContext)
	
	: VCSFileCommand (inContext, true, true, true, true)
		
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
	const	FSSpec&			project,
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
		
		//	Check for project locking
		if (::FSpEqual (&item, &project))
			::FSpSetFLock (&project);
			
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
	const	FSSpec&			project,
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
					ParseResult (root, project, output, line);
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
		StAEDesc 	command;
		StAEDesc 	options;
		StHandle	output;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kCheckInTask, inItem.fsItem.name);
		
		// Get a comment from the user
		comment[0] = 0;
		if (!VCSPromptComment (mContext, kCheckinCommentPrompt, inItem.fsItem.name, comment))
			return inItem.eItemStatus = cwItemStatusCancelled;
			
		//	Get the options.
		OSErr			e = noErr;
		if (noErr != (e = VCSRaiseOSErr (mContext, ::AECreateList (nil, 0 , false, &options)))) return inItem.eItemStatus;
		switch (e = CVSCheckinOptionsDialog::GetOptions (mContext, inItem.fsItem, options)) {
			case userCanceledErr:
				return inItem.eItemStatus = cwItemStatusCancelled;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) return inItem.eItemStatus;
			} // switch

		//	cvs -r commit <options> -R -m <comment>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-r"))) return inItem.eItemStatus;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "commit"))) return inItem.eItemStatus;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddListArgs (&command, &options))) return inItem.eItemStatus;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "-R"))) return inItem.eItemStatus;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCommentArg (&command, comment))) return inItem.eItemStatus;

		// send the command to MacCVS
		if (noErr != VCSSendOutputCommand (mContext, &command, &cwd, &output.mH)) return inItem.eItemStatus;
		
		//	Display the output
		VCSDisplayResult (mContext, messagetypeInfo, kErrorStrings, kCvsInfo, output);

		//	Update status
		FSSpec		projectFile;
		mContext.GetProjectFile (projectFile);
		CWVCSDatabaseConnection	db;
		mContext.GetDatabase (db);
		inItem.eItemStatus = ParseOutput (db.sProjectRoot, projectFile, output);
		
		//	Handle project locking
		if (fLckdErr != ::FSpCheckObjectLock (&projectFile)) {
			//	Commit project file
			VCSTask 	projectTask (mContext, kTaskStrings, kCheckInTask, projectFile.name);
			
			//	Clean up old stuff
			StAEDesc	projectCommand;
			inItem.eItemStatus = cwItemStatusFailed;

			//	Get the cwd for update
			cwd = projectFile;
			if (noErr != VCSRaiseOSErr (mContext, ::FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) return inItem.eItemStatus;

			//	cvs -r commit <options> -m <comment> <project>
			if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&projectCommand, "-r"))) return inItem.eItemStatus;
			if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&projectCommand, "commit"))) return inItem.eItemStatus;
			if (noErr != VCSRaiseOSErr (mContext, CVSAddListArgs (&projectCommand, &options))) return inItem.eItemStatus;
			if (noErr != VCSRaiseOSErr (mContext, CVSAddCommentArg (&projectCommand, comment))) return inItem.eItemStatus;
			if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&projectCommand, projectFile.name))) return inItem.eItemStatus;

			// send the command to MacCVS
			if (noErr != (VCSSendCommand (mContext, &projectCommand, &cwd))) return inItem.eItemStatus;
			
			//	Lock the damn thing in case MacCVS unlocked it...
			::FSpSetFLock (&inItem.fsItem);

			//	Update status
			inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
			} // if

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
		StAEDesc 	command;
		
		//	Prepare
		inItem.eItemStatus = cwItemStatusFailed;
		VCSTask 	task (mContext, kTaskStrings, kCheckInTask, inItem.fsItem.name);
		
		//	Get the cwd for update
		if (noErr != VCSRaiseOSErr (mContext, ::FSMakeFSSpec (cwd.vRefNum, cwd.parID, nil, &cwd))) return inItem.eItemStatus;

		//	Get the checkin comment
		comment[0] = 0;
		if (!VCSPromptComment (mContext, kCheckinCommentPrompt, inItem.fsItem.name, comment)) 
			return inItem.eItemStatus = cwItemStatusCancelled;
			
		//	cvs -r commit <options> -m <comment> <file>
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "-r"))) return inItem.eItemStatus;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddCStringArg (&command, "commit"))) return inItem.eItemStatus;

		//	Get the options.
		OSErr			e = noErr;
		switch (e = CVSCheckinOptionsDialog::GetOptions (mContext, inItem.fsItem, command)) {
			case userCanceledErr:
				return inItem.eItemStatus = cwItemStatusCancelled;
			
			default:
				if (noErr != VCSRaiseOSErr (mContext, e)) return inItem.eItemStatus;
			} // switch

		if (noErr != VCSRaiseOSErr (mContext, CVSAddCommentArg (&command, comment))) return inItem.eItemStatus;
		if (noErr != VCSRaiseOSErr (mContext, CVSAddPStringArg (&command, inItem.fsItem.name))) return inItem.eItemStatus;
		
		// send the command to MacCVS
		if (noErr != (VCSSendCommand (mContext, &command, &cwd))) return inItem.eItemStatus;
		
		//	Lock the damn thing in case MacCVS unlocked it...
		::FSpSetFLock (&inItem.fsItem);

		//	Update status
		inItem.eItemStatus = VCSVersion (mContext).ProcessRegularFile (inItem);
			
		return inItem.eItemStatus;
		
	} // end ProcessRegularFile

