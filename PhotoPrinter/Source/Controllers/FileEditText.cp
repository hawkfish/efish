/*
	File:		FileEditText.cp

	Contains:	An adornment class which is used for nametag on-screen functions
				also contains helper Action

	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		15 Aug 2001		drd		309 Override ObeyCommand and work around OS X bug
		13 Aug 2001		rmgw	Scroll PhotoPrintView, not the background.  Bug #284.
		08 Aug 2001		rmgw	Make TryRename smarter. Bug #298.
		08 Aug 2001		rmgw	Fix rename failures. Bug #295.
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
						RenameFileAction(PhotoPrintDoc*	inDoc, PhotoItemRef inItem, LCommander* inEditText);
	virtual				~RenameFileAction();

	ConstStr255Param	GetCurrentFileName (void) const;

	virtual bool		TryRenameFile	(ConstStr255Param	inName);

protected:
	PhotoItemRef		mItem;
	LCommander*			mEditText;

	MPString			mUndoName;
	
	// LAction
	virtual	void		RedoSelf();
	virtual	void		UndoSelf();
	
};


// ---------------------------------------------------------------------------
//	¥RenameFileAction
// ---------------------------------------------------------------------------
RenameFileAction::RenameFileAction(

	PhotoPrintDoc*			inDoc, 
	PhotoItemRef			inItem, 
	LCommander* 			inEditText) 
	
	: PhotoPrintAction (inDoc, si_RenameFile, kAlreadyDone)
	
	, mItem (inItem)
	, mEditText (inEditText)

	, mUndoName (GetCurrentFileName ())
{}

// ---------------------------------------------------------------------------
//	¥~RenameFileAction
// ---------------------------------------------------------------------------
RenameFileAction::~RenameFileAction()
{}

// ---------------------------------------------------------------------------
//	¥ GetCurrentFileName										   [protected]
// ---------------------------------------------------------------------------
//	Not virtual because constructor uses it.

ConstStr255Param
RenameFileAction::GetCurrentFileName (void) const

	{ // begin GetCurrentFileName
		
		Assert_(mItem);
		Assert_(mItem->GetFileSpec());

		return mItem->GetFileSpec()->Name ();
		
	} // end GetCurrentFileName

// ---------------------------------------------------------------------------
//	¥RedoSelf
// ---------------------------------------------------------------------------
void
RenameFileAction::RedoSelf() 

{ // begin RedoSelf
	
		//	Get the new undo state
	bool				redoDirty (GetCurrentDirty ());
	MPString			redoName (GetCurrentFileName ());
		
		//	Swap the filename back
	this->TryRenameFile (mUndoName);

		//	Restore the dirty flag
	GetDocument ()->SetDirty (mUndoDirty);
		
		//	Swap the state
	mUndoDirty = redoDirty;
	mUndoName = redoName;
		
		//	Put the user back in the field
	LCommander::SwitchTarget (mEditText);

}//end RedoSelf
	

// ---------------------------------------------------------------------------
//	¥ TryRenameFile  
// ---------------------------------------------------------------------------

bool
RenameFileAction::TryRenameFile (

	ConstStr255Param	inName) 
	
{ // begin TryRenameFile
	
	bool	success = false;
	
	HORef<MFileSpec>	itemSpec (mItem->GetFileSpec());
	Assert_(itemSpec != nil);
	
	try {
		StDisableDebugThrow_();
		
			//	Swap the filenames
		itemSpec->Rename(inName);
			
			//	Tell everyone
		PhotoItemList					itemList (1, mItem);
		PhotoPrintModel::MessageRange	range = {itemList.begin(), itemList.end()};
		GetModel ()->BroadcastMessage (PhotoPrintModel::msg_ModelItemsChanged, &range);
		
			// Anyone with this name had better do something!
		FileNotifier::Notify (*itemSpec);
			
			//	Ahhh!
		success = true;				
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
									   itemSpec->Name (),	//	^1	==	old name
									   inName,				//	^2	==	new name
									   errorNumber);		//	^3	==	error #
		EUserMessageServer::GetSingleton ()->QueueUserMessage (renameMessage);
		}//catch
	
	return success;
	
}//end TryRenameFile


// ---------------------------------------------------------------------------
//	¥ UndoSelf.  
// ---------------------------------------------------------------------------
void
RenameFileAction::UndoSelf() {
	RedoSelf ();
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
	Boolean allowIt (false);
	
	if (TryRename())
		allowIt = LCommander::AllowDontBeTarget(inNewTarget);
	else {
		SetDescriptor(mItem->GetFileSpec()->Name());
		SelectAll();
	}// ii f

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
	LView*		theView = mDoc->GetView();
	
	//	Outer badge's location in the main view
	SPoint32	pt32 = {0, 0};
	this->GetSuperView()->GetFrameLocation (pt32);
	
	//	Convert to image coordinates (without going through the port)
	SPoint32	imageLocation;
	theView->GetImageLocation (imageLocation);
	pt32.h -= imageLocation.h;
	pt32.v -= imageLocation.v;
	
	//	Scroll to that image location
	theView->ScrollPinnedImageTo (pt32.h, pt32.v, Refresh_Yes);

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
ObeyCommand {OVERRIDE}
	We override because LEditText::ObeyCommand(cmd_Copy) calls ::TEToScrap(), which seems
	not to work under OS X [309]
*/
Boolean
FileEditText::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{
	Boolean		cmdHandled = true;

	switch (inCommand) {
		case cmd_Copy: {
			SInt16			selStart = (**mTextEditH).selStart;
			SInt32			textLen = (**mTextEditH).selEnd - selStart;
			StHandleBlock	theText(textLen);
			::BlockMoveData(*((**mTextEditH).hText) + selStart, *theText, textLen);

			::TECopy(mTextEditH);				// Probably don't need internal scrap, but be safe
			UScrap::SetData('TEXT', theText, true);
			break;
		}

		default:
			cmdHandled = inherited::ObeyCommand(inCommand, ioParam);
			break;
	}

	return cmdHandled;
}

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
	bool				bHappy (false);

	Str255				newName;
	this->GetDescriptor(newName);
	
	Assert_(mItem->GetFileSpec());
	if (0 == ::RelString(mItem->GetFileSpec()->Name(), newName, false, true)) return true;

	MemoryExceptionHandler	handler (MPString (str_CommandName, si_RenameFile).AsPascalString ());
	
	RenameFileAction*	renameAction (new RenameFileAction (mDoc, mItem, this));
	Assert_(renameAction);

	if (renameAction->TryRenameFile (newName)) {
		this->PostAction (renameAction);
		bHappy = true;
		}//endif
	else
		delete renameAction;

	return bHappy;
}//end TryRename

