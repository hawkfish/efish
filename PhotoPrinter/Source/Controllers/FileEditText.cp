/*
	File:		FileEditText.cp

	Contains:	An adornment class which is used for nametag on-screen functions
				also contains helper Action

	Written by:	Dav Lion
	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	18 Jul 2001		drd		Removed unnecessary RenameFileAction::Redo; 194 talk to FileNotifier
	26 Jun 2001		drd		Call UCursor::SetArrow() before displaying alert
	15 Jun 2001		rmgw	Make BeTarget smarter and less obtrusive.  Bug 66.
	14 Jun 2001		rmgw	Make new HORef with GetFileSpec result.  Bug #56.
	25 Apr 2001		drd		BeTarget, SetItem don't crash for placeholder item; renamed instance data
	26 feb 2001		dml		fix handling of locked files (which should be handled way upstream anyway)
	26 feb 2001 	dml		fix updating of view + edit box on Redo/Undo
	26 feb 2001		dml		cleanup AllowDontBeTarget handling, refactored RenameFileAction
	23 feb 2001		dml		created

*/

#include "FileEditText.h"
#include "FileNotifier.h"
#include "MPString.h"
#include <LAction.h>
#include "PhotoPrintCommands.h"
#include "PhotoPrintResources.h"
#include <algorithm>

const ResIDT	str_RenameFileAction = 23;
const ResIDT	alrt_DuplicateFilename = 136;
const ResIDT	alrt_RenameFailure = 137;
const ResIDT	alrt_FileLocked = 138;

class	RenameFileAction : public LAction
{
public:
				RenameFileAction(PhotoItemRef inItem, ConstStrFileNameParam inNewName, LEditText* inEditText);
				~RenameFileAction();

protected:
	PhotoItemRef	mItem;
	MPString		mOldName;
	MPString		mNewName;
	LEditText*		mEditText;
	
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();
	
	virtual void		TryRenameFile(MPString& newName);
};


RenameFileAction::RenameFileAction(PhotoItemRef inItem, ConstStrFileNameParam inNewName, LEditText* inEditText) 
	: LAction(str_Redo, str_RenameFileAction)
	, mItem (inItem)
	, mOldName (mItem->GetFileSpec()->Name())
	, mNewName (inNewName)
	, mEditText (inEditText)
{}

RenameFileAction::~RenameFileAction()
{}

// ---------------------------------------------------------------------------
//	¥RedoSelf
// ---------------------------------------------------------------------------
void
RenameFileAction::RedoSelf() {
	this->TryRenameFile(mNewName);
	FileNotifier::Notify(mOldName);					// Anyone with this name had better do something!
}//end RedoSelf
	

// ---------------------------------------------------------------------------
//	¥ TryRenameFile  
// ---------------------------------------------------------------------------
void
RenameFileAction::TryRenameFile(MPString& newName) {
	Assert_(mItem->GetFileSpec() != nil);
	try {
		StDisableDebugThrow_();
		mItem->GetFileSpec()->Rename(newName);
		mIsDone = true;
		mEditText->SetDescriptor(mItem->GetFileSpec()->Name());
		mEditText->InvalPortRect(&(mItem->GetImageRect()));
	}//end try
	catch (LException e) {
		// !!! Should use new spiffy error reporting
		UCursor::SetArrow();
		switch (e.GetErrorCode()) {
			case dupFNErr:
				::StopAlert(alrt_DuplicateFilename, NULL);
				break;
			case fLckdErr:
			case permErr:
				::StopAlert(alrt_FileLocked, NULL);
				break;
			default : {
				MPString errorNumber (e.GetErrorCode());
				::ParamText(errorNumber, NULL, NULL, NULL);
				::StopAlert(alrt_RenameFailure, NULL);
				}//default case
			}//switch
		mEditText->SetDescriptor(mOldName); // failed, so roll back the edit state
		mIsDone = false;
		}//catch
}//end TryRenameFile


// ---------------------------------------------------------------------------
//	¥ UndoSelf.  
// ---------------------------------------------------------------------------
void
RenameFileAction::UndoSelf() {
	this->TryRenameFile(mOldName);
	FileNotifier::Notify(mNewName);					// Anyone with this name had better do something!
	if (mIsDone)
		LCommander::SwitchTarget(mEditText);
}//end UndoSelf


#pragma mark -

// ---------------------------------------------------------------------------
//	¥ FileEditText
// ---------------------------------------------------------------------------

FileEditText::FileEditText(
	LStream*	inStream,
	ClassIDT	inImpID)

	: LEditText(inStream, inImpID)
{
	FileNotifier::Listen(this);
}//tiny ct


// ---------------------------------------------------------------------------
//	¥ 
// ---------------------------------------------------------------------------
FileEditText::FileEditText(
	const SPaneInfo&	inPaneInfo,
	LCommander*			inSuperCommander,
	ConstStringPtr		inInitialText,
	ResIDT				inTextTraitsID,
	MessageT			inMessage,
	SInt16				inMaxChars,
	UInt8				inAttributes,
	TEKeyFilterFunc		inKeyFilter,
	bool				inPasswordField,
	ClassIDT			inImpID)
	
	:LEditText(inPaneInfo,
				inSuperCommander,
				inInitialText,
				inTextTraitsID,
				inMessage,
				inMaxChars,
				inAttributes,
				inKeyFilter,
				inPasswordField,
				inImpID)
{
	FileNotifier::Listen(this);
}//big ct


// ---------------------------------------------------------------------------
//	¥ ~FileEditText
// ---------------------------------------------------------------------------
FileEditText::~FileEditText()
{}//dt


// ---------------------------------------------------------------------------
//	¥ AllowDontBeTarget
// ---------------------------------------------------------------------------
Boolean		
FileEditText::AllowDontBeTarget(LCommander* inNewTarget) {
	Boolean allowIt (true);
	Str255 newName;
	GetDescriptor(newName);

	Assert_(mItem->GetFileSpec() != nil);

	if (::RelString(mItem->GetFileSpec()->Name(), newName, true, true) != 0) {
		if (TryRename())
			allowIt = LCommander::AllowDontBeTarget(inNewTarget);
		else {
			SetDescriptor(mItem->GetFileSpec()->Name());
			SelectAll();
			allowIt = false;
		}//
	}//if the name has changed, validate change ok before switching target

	return allowIt;
}//end AllowDontBeTarget


// ---------------------------------------------------------------------------
//	¥ BeTarget
// ---------------------------------------------------------------------------
void
FileEditText::BeTarget() {
	
	LEditText::BeTarget();
	
	HORef<MFileSpec>	spec(mItem->GetFileSpec());
	if (spec == nil) return;	// ??? this seems backward
	
	MStr<Str63>	newText (spec->Name());
	MPString	curText;
	this->GetDescriptor (curText.AsPascalString ());
	if (0 == curText.Compare (newText)) return;
	
	this->SetDescriptor(newText);
}//end BeTarget


// ---------------------------------------------------------------------------
//	¥ HandleKeyPress
// ---------------------------------------------------------------------------
Boolean
FileEditText::HandleKeyPress(const EventRecord&	inKeyEvent) {
	UInt16		theChar		 = (UInt16) (inKeyEvent.message & charCodeMask);
	
	if (UKeyFilters::IsActionKey(theChar) ||
		(inKeyEvent.modifiers & controlKey && UKeyFilters::IsNavigationKey(theChar))) {
			return LCommander::HandleKeyPress(inKeyEvent); // pass up in case we are inside tab group
		}//endif special keys get sent up
	else {
		return LEditText::HandleKeyPress(inKeyEvent);
		}//else it's actionable, so try to action it
}//end HandleKeyPress
	
/*
ListenToMessage {OVERRIDE}
*/
void
FileEditText::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{
	if (inMessage != msg_FilenameChanged)
		return;

	MPString	oldName(static_cast<ConstStr255Param>(ioParam));

	// Note that GetFileSpec forces resolution of the alias
	HORef<MFileSpec>	spec(mItem->GetFileSpec());
	if (spec != nil) {
		MPString	curText;
		this->GetDescriptor(curText.AsPascalString ());
		if (curText == oldName) {
			// I don't think this is enough, but may be for duplicated
			this->SetDescriptor(spec->Name());
		}
	}
}

// ---------------------------------------------------------------------------
//	¥ SetItem
// ---------------------------------------------------------------------------
void
FileEditText::SetItem(PhotoItemRef inItem) {
	mItem = inItem;

	HORef<MFileSpec>	spec(mItem->GetFileSpec());
	if (spec != nil)
		this->SetDescriptor(spec->Name());
}//end

	
// ---------------------------------------------------------------------------
//	¥ TryRename
// ---------------------------------------------------------------------------
bool
FileEditText::TryRename(void)
{
	bool				bHappy (false);
	Str255				newName;
	GetDescriptor(newName);
	RenameFileAction*	newAction (new RenameFileAction(mItem, newName, this));
	newAction->Redo();
	if (newAction->IsDone()) {
		PostAction(newAction);
		bHappy = true;
		}//endif
	else
		delete newAction;

	return bHappy;
}//end TryRename

