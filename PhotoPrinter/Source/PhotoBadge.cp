/*
	File:		PhotoBadge.cp

	Contains:	The implementation of a badge manager view.

	Written by:	Dav Lion
	
	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		16 Aug 2001		drd		326 Set pane IDs for badge and FileEditText. This was supposed to
								help in debugging, but it seems to have worked around the bug.
		25 Jul 2001		rmgw	Change badge pane id. Bug #227.
		24 Jul 2001		rmgw	Badges need to know about the document. Bug #202.
		24 feb 2001		dml		nametag tool working
		24 feb 2001		dml		added support for NameTag tool
*/

#include "PhotoBadge.h"

#include <LEditText.h>

const ResIDT	View_Badge = 3000;
const PaneIDT	etxt_name = 'btxt';	

//-----------------------------------------------
// PhotoBadge default constructor
//-----------------------------------------------
PhotoBadge::PhotoBadge()
	: LView ()
	, mNameTag (nil)
	, mDoc (0)
	, mItem (nil)
{
}

//-----------------------------------------------
// PhotoBadge copy constructor
//-----------------------------------------------
PhotoBadge::PhotoBadge(	const PhotoBadge		&inOriginal)
	: LView(inOriginal)
	, mNameTag (nil)
	, mDoc (0)
	, mItem (nil)
{
}

//-----------------------------------------------
// PhotoBadge SPaneInfo constructor
//-----------------------------------------------
PhotoBadge::PhotoBadge(	const SPaneInfo		&inPaneInfo,
								const SViewInfo		&inViewInfo)
	: LView(inPaneInfo,
			inViewInfo)
	, mNameTag (nil)
	, mDoc (0)
	, mItem (nil)
{
}

//-----------------------------------------------
// PhotoBadge Stream constructor
//-----------------------------------------------
PhotoBadge::PhotoBadge(	LStream			*inStream)
	: LView (inStream)
	, mNameTag (nil)
	, mDoc (0)
	, mItem (nil)
{
	// For ease in debugging, give us a unique pane ID
	if (this->GetPaneID() == 0) {
		this->SetPaneID('B000' + this->GetSuperView()->GetSubPanes().GetCount());
	}
}

//-----------------------------------------------
// ~PhotoBadge 
//-----------------------------------------------
PhotoBadge::~PhotoBadge()
{
}// ~PhotoBadge

//-----------------------------------------------
// FinishCreateSelf.  Find all those handy panes which ought to be setup by now
//-----------------------------------------------
void
PhotoBadge::FinishCreateSelf()
{
	mNameTag = (FileEditText*)FindPaneByID(etxt_name);
	mNameTag->SetKeyFilter(FileNameField);

	// Give the FileEditText a unique id. This helps us debug.
	// Richard had set it to 0, to avoid confusion of them all having 
	mNameTag->SetPaneID('N\0\0\0' | (this->GetPaneID() & 0xFFFFFFL));	//	For ease of debugging.
} // FinishCreateSelf

#pragma mark -

//-----------------------------------------------------
//	ClickSelf
//-----------------------------------------------------
void		
PhotoBadge::ClickSelf(const SMouseDownEvent &/*inMouseDown*/) {
	LCommander::SwitchTarget(GetNameTag());
}//end


//-----------------------------------------------------
//	FileNameField.  a keyfilter based off UKeyFilters.cp's PrintingCharFilter
//-----------------------------------------------------
EKeyStatus	
PhotoBadge::FileNameField(TEHandle		/*inMacTEH*/,
						UInt16			inKeyCode,
						UInt16			&ioCharCode,
						EventModifiers	/*inModifiers*/) {

	EKeyStatus	theKeyStatus = keyStatus_PassUp;

	if (UKeyFilters::IsTEDeleteKey(inKeyCode)) {
		theKeyStatus = keyStatus_TEDelete;

	} else if (UKeyFilters::IsTECursorKey(inKeyCode)) {
		theKeyStatus = keyStatus_TECursor;

	} else if (UKeyFilters::IsExtraEditKey(inKeyCode)) {
		theKeyStatus = keyStatus_ExtraEdit;

	} else if ((ioCharCode >= char_FirstPrinting) &&
			   (ioCharCode != char_FwdDelete) &&
			   (ioCharCode != ':')) {
		theKeyStatus = keyStatus_Input;
	} else {
		theKeyStatus = keyStatus_Reject;
	 	}//else unhappy so beep

	return theKeyStatus;
}//end FileNameField


//-----------------------------------------------------
//	SetItem
//-----------------------------------------------------
void
PhotoBadge::SetItem(PhotoPrintDoc*	inDoc, PhotoItemRef inItem) {
	mDoc = inDoc;
	mItem = inItem;
	mNameTag->SetItem(mDoc, mItem);
}//SetItem
