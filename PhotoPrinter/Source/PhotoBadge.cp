#include "PhotoBadge.h"
#include <LEditText.h>
const ResIDT	View_Badge = 3000;
const PaneIDT	etxt_name = 'name';	

//-----------------------------------------------
// PhotoBadge default constructor
//-----------------------------------------------
PhotoBadge::PhotoBadge()
	: LView ()
	, mNameTag (nil)
	, mItem (nil)
	
{
}

//-----------------------------------------------
// PhotoBadge copy constructor
//-----------------------------------------------
PhotoBadge::PhotoBadge(	const PhotoBadge		&inOriginal)
	: LView(inOriginal)
	, mNameTag (nil)
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
	, mItem (nil)
{
}

//-----------------------------------------------
// PhotoBadge Stream constructor
//-----------------------------------------------
PhotoBadge::PhotoBadge(	LStream			*inStream)
	: LView (inStream)
	, mNameTag (nil)
	, mItem (nil)
{
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
	mNameTag = (LEditText*)FindPaneByID(etxt_name);
	mNameTag->SetKeyFilter(FileNameField);
		
} // FinishCreateSelf

#pragma mark -

//-----------------------------------------------------
//	ClickSelf
//-----------------------------------------------------
void		
PhotoBadge::ClickSelf(const SMouseDownEvent &inMouseDown) {
	LCommander::SwitchTarget(GetNameTag());
}//end


//-----------------------------------------------------
//	SetName
//-----------------------------------------------------
void
PhotoBadge::SetName(ConstStrFileNameParam inName) {
	if (ValidName(inName)) {
		mNameTag->SetDescriptor(inName);
		}//endif legalName
	}//end SetName


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
	}
	 else {
		theKeyStatus = keyStatus_Reject;
	 	}//else unhappy so beep

	return theKeyStatus;
}//end FileNameField


//-----------------------------------------------------
//	SetItem
//-----------------------------------------------------
void
PhotoBadge::SetItem(PhotoItemRef inItem) {
	mItem = inItem;
	SetName(mItem->GetFileSpec()->Name());
	}//SetItem



//-----------------------------------------------------
//	ValidName.  make sure it's printable and doesn't contain ':'
//-----------------------------------------------------
bool
PhotoBadge::ValidName(ConstStrFileNameParam inName) {
	for (int i = 1; i < inName[0] + 1; ++i) {
		if ((inName[i] < char_FirstPrinting) ||
			(inName[i] == ':'))
			return false;
		}//for all chars

	return true;
	}//end ValidName