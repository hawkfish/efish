/*
	File:		FileEditText.cp

	Contains:	An adornment class which is used for nametag on-screen functions
				also contains helper Action

	Written by:	Dav Lion
	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	26 feb 2001		dml			cleanup AllowDontBeTarget handling, refactored RenameFileAction
	23 feb 2001		dml			created

*/

#include "FileEditText.h"
#include <UKeyFilters.h>
#include "MPString.h"
#include <LAction.h>
#include "PhotoPrintResources.h"
#include <UDebugging.h>
#include <algorithm>

const ResIDT	str_RenameFileAction = 23;
const ResIDT	alrt_DuplicateFilename = 136;
const ResIDT	alrt_RenameFailure = 137;

class	RenameFileAction : public LAction
{
public:
				RenameFileAction(HORef<MFileSpec>& ioSpec, ConstStrFileNameParam inNewName);
				~RenameFileAction();

	virtual	void	Redo();

protected:
	HORef<MFileSpec>&	mSpec;
	MPString	oldName;
	MPString	newName;

	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();
	
	virtual void		TryRenameFile(MPString& newName);
};


RenameFileAction::RenameFileAction(HORef<MFileSpec>& inSpec, ConstStrFileNameParam inNewName) 
	: LAction(str_Redo, str_RenameFileAction)
	, mSpec (inSpec)
	, oldName (inSpec->Name())
	, newName (inNewName)
{}

RenameFileAction::~RenameFileAction()
{}



// ---------------------------------------------------------------------------
//	¥Redo
// ---------------------------------------------------------------------------
void
RenameFileAction::Redo()
{
	if (CanRedo()) 
		RedoSelf();
}//end Redo



// ---------------------------------------------------------------------------
//	¥RedoSelf
// ---------------------------------------------------------------------------
void
RenameFileAction::RedoSelf() {
	TryRenameFile(newName);
	}//end RedoSelf
	

// ---------------------------------------------------------------------------
//	¥ TryRenameFile  
// ---------------------------------------------------------------------------
void
RenameFileAction::TryRenameFile(MPString& newName) {
	try {
		StDisableDebugThrow_() ;
		mSpec->Rename(newName);
		mIsDone = true;
		}//end try
	catch (LException e) {
		if (e.GetErrorCode() == dupFNErr)
			::StopAlert(alrt_DuplicateFilename, NULL);
		else {
			MPString errorNumber (e.GetErrorCode());
			::ParamText(errorNumber, NULL, NULL, NULL);
			::StopAlert(alrt_RenameFailure, NULL);
			}//else
		mIsDone = false;
		}//catch
}//end TryRenameFile


// ---------------------------------------------------------------------------
//	¥ UndoSelf.  
// ---------------------------------------------------------------------------
void
RenameFileAction::UndoSelf() {
	TryRenameFile(oldName);
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
//	¥ HandleKeyPress
// ---------------------------------------------------------------------------
Boolean
FileEditText::HandleKeyPress(const EventRecord&	inKeyEvent) {
	UInt16		theChar		 = (UInt16) (inKeyEvent.message & charCodeMask);
	
	if (UKeyFilters::IsActionKey(theChar) ||
	(inKeyEvent.modifiers & controlKey && UKeyFilters::IsNavigationKey(theChar))) {
		TryRename();
		return LCommander::HandleKeyPress(inKeyEvent); // pass up in case we are inside tab group
		}//endif special keys get sent up
	else {
		return LEditText::HandleKeyPress(inKeyEvent);
		}//else it's actionable, so try to action it

	}//end HandleKeyPress
	
	

// ---------------------------------------------------------------------------
//	¥ SetItem
// ---------------------------------------------------------------------------
void
FileEditText::SetItem(PhotoItemRef inItem) {
	mItem = inItem;
	SetDescriptor(mItem->GetFileSpec()->Name());
	}//end
		

	
// ---------------------------------------------------------------------------
//	¥ TryRename
// ---------------------------------------------------------------------------
bool
FileEditText::TryRename(void) {

bool bHappy (false);
	Str255 newName;
	GetDescriptor(newName);
	RenameFileAction*	newAction (new RenameFileAction(mItem->GetFileSpec(), newName));
	newAction->Redo();
	if (newAction->IsDone()) {
		PostAction(newAction);
		bHappy = true;
		}//endif
	else
		delete (newAction);

return bHappy;
}//end TryRename
	
	
