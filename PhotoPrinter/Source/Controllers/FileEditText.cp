/*
	File:		FileEditText.cp

	Contains:	An adornment class which is used for nametag on-screen functions
				also contains helper Action

	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		06 Aug 2001		drd		259 Added asserts to TryRename
		26 Jul 2001		drd		233 BeTarget scrolls if necessary
		24 Jul 2001		rmgw	Fix rename notification. Bug #219.
		24 Jul 2001		rmgw	Badges need to know about the document. Bug #202.
		19 Jul 2001		drd		194 Compare against current text, not filespec, and send ClearFileSpec
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

//	PhotoPrint
#include "FileNotifier.h"
#include "PhotoPrintAction.h"
#include "PhotoPrintCommands.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintDoc.h"
#include "PhotoExceptionHandler.h"
#include "PhotoPrintModel.h"
#include "PhotoPrintResources.h"

//	Epp
#include "EUserMessage.h"
#include "EUserMessageServer.h"

//	Toolbox++
#include "MPString.h"

//	STL
#include <algorithm>

//	Resources
const ResIDT	TEXT_DuplicateFilename	= 1136;
const ResIDT	TEXT_RenameFailure		= 1137;
const ResIDT	TEXT_FileLocked			= 1138;

//	Types
class	RenameFileAction : public PhotoPrintAction
{
public:
				RenameFileAction(PhotoPrintDoc*	inDoc, PhotoItemRef inItem, ConstStrFileNameParam inNewName, LEditText* inEditText);
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


RenameFileAction::RenameFileAction(

	PhotoPrintDoc*			inDoc, 
	PhotoItemRef			inItem, 
	ConstStrFileNameParam	inNewName, 
	LEditText* 				inEditText) 
	
	: PhotoPrintAction (inDoc, si_RenameFile, kNotAlreadyDone)
	
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
}//end RedoSelf
	

// ---------------------------------------------------------------------------
//	¥ TryRenameFile  
// ---------------------------------------------------------------------------
void
RenameFileAction::TryRenameFile(MPString& newName) {

	HORef<MFileSpec>	mRedoSpec (mItem->GetFileSpec());
	Assert_(mRedoSpec != nil);
	
	try {
		StDisableDebugThrow_();
		
			//	Get the new undo state
		bool				mRedoDirty (GetCurrentDirty ());
		
			//	Swap the filenames
		mRedoSpec->Rename(newName);
		
		PhotoItemList					itemList (1, mItem);
		PhotoPrintModel::MessageRange	range = {itemList.begin(), itemList.end()};
		GetModel ()->BroadcastMessage (PhotoPrintModel::msg_ModelItemsChanged, &range);
		
			//	Restore the dirty flag
		GetDocument ()->SetDirty (mUndoDirty);
		
			//	Swap the state
		mUndoDirty = mRedoDirty;
			
			// Anyone with this name had better do something!
		FileNotifier::Notify (*mRedoSpec);					
	}//end try
	
	catch (LException& e) {
		ResIDT		msgID = TEXT_RenameFailure;
		
		MPString	errorNumber (e.GetErrorCode());
		switch (e.GetErrorCode()) {
			case dupFNErr:
				msgID = TEXT_DuplicateFilename;
				break;
			
			case fLckdErr:
			case permErr:
				msgID = TEXT_FileLocked;
				break;
			}//switch
		
		EUserMessage	renameMessage (msgID, kStopIcon, 
									   ExceptionHandler::GetCurrentHandler ()->GetOperation (), 
									   						//	^0	==	operation
									   mRedoSpec->Name (),	//	^1	==	old name
									   newName,				//	^2	==	new name
									   errorNumber);		//	^3	==	error #
		EUserMessageServer::GetSingleton ()->QueueUserMessage (renameMessage);
		}//catch

}//end TryRenameFile


// ---------------------------------------------------------------------------
//	¥ UndoSelf.  
// ---------------------------------------------------------------------------
void
RenameFileAction::UndoSelf() {
	this->TryRenameFile(mOldName);
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
	
	, mDoc (0)
	, mItem (0)
	
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

	, mDoc (0)
	, mItem (0)
	
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

	// 233 Scroll to the text. We need to get the thing being scrolled (the 'back' view) and
	// scroll it. ??? Currently, we're not too bright about NOT scrolling if it's already
	// visible.
	LView*		theView = mDoc->GetScrolledView();
	Point		pt = {0, 0};
	this->GetSuperView()->LocalToPortPoint(pt);
	theView->PortToLocalPoint(pt);
	theView->ScrollPinnedImageTo(pt.h, pt.v, Refresh_Yes);

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
	
// ---------------------------------------------------------------------------
//	¥ SetItem
// ---------------------------------------------------------------------------
void
FileEditText::SetItem(PhotoPrintDoc* inDoc, PhotoItemRef inItem) {
	mDoc = inDoc;
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
	Assert_(mItem->GetFileSpec() != nil);

	bool				bHappy (false);
	Str255				newName;
	this->GetDescriptor(newName);
	RenameFileAction*	newAction (new RenameFileAction(mDoc, mItem, newName, this));
	Assert_(newAction != nil);
	newAction->Redo();
	if (newAction->IsDone()) {
		this->PostAction(newAction);
		bHappy = true;
		}//endif
	else
		delete newAction;

	return bHappy;
}//end TryRename

