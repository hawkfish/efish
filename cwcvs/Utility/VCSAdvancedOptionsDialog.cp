#include "VCSAdvancedOptionsDialog.h"

#include "VCSUtil.h"
#include "CVSCommand.h"

#include <TextUtils.h>

// ---------------------------------------------------------------------------
//		¥ VCSAdvancedOptionsDialog
// ---------------------------------------------------------------------------

VCSAdvancedOptionsDialog::VCSAdvancedOptionsDialog (

	const VCSContext&		inPB,
	short					inDLOGid)
	
	: VCSStdDialog (inPB, inDLOGid)
	
	{ // begin VCSAdvancedOptionsDialog
	
	} // end VCSAdvancedOptionsDialog
	
// ---------------------------------------------------------------------------
//		¥ ~VCSAdvancedOptionsDialog
// ---------------------------------------------------------------------------

VCSAdvancedOptionsDialog::~VCSAdvancedOptionsDialog (void)

	{ // begin ~VCSAdvancedOptionsDialog
	
	} // end ~VCSAdvancedOptionsDialog

// ---------------------------------------------------------------------------
//		¥ GetOptionsList
// ---------------------------------------------------------------------------

OSErr 
VCSAdvancedOptionsDialog::GetOptionsList (

	AEDescList&		outOptions,
	ResID			inStrnID) const

	{ // begin GetOptionsList
		
		OSErr					e = noErr;
		
		const	DialogItemIndex	nItems = CountDITL (GetDialogPtr ());
		short					index = 1;
		for (DialogItemIndex itemNo = kVCSAdvancedFirstFreeItem; itemNo <= nItems; ++itemNo) {
			short		iType;
			Handle		iHandle;
			Rect		iRect;
			Str255		option;
			Str255		value;
			
			::GetDialogItem (GetDialogPtr (), itemNo, &iType, &iHandle, &iRect);
			short		baseType (iType & ~kItemDisableBit);
			if ((kStaticTextDialogItem | kIconDialogItem | kPictureDialogItem) & iType) continue;
			if (baseType == kUserDialogItem) continue;
			if (baseType == kButtonDialogItem) continue;
			
			if (baseType & kEditTextDialogItem) {
				::GetIndString (option, inStrnID, index);
				::GetDialogItemText (iHandle, value);
				++index;
				if ((iType & kItemDisableBit) || (option[0] == 0) || (value[0] == 0)) continue;
				
				if (option[option[0]] == ' ') {
					--option[0];
					::AppendPString (option, value);
					if (noErr != (e = ::CVSAddPStringArg (&outOptions, option))) goto CleanUp;
					continue;
					} // if
					
				if (noErr != (e = ::CVSAddPStringArg (&outOptions, option))) goto CleanUp;
				if (noErr != (e = ::CVSAddPStringArg (&outOptions, value))) goto CleanUp;
				continue;
				} // if
			
			if (iType & kControlDialogItem) {
				ControlHandle	c = ControlHandle (iHandle);
				SInt16			min = ::GetControlMinimum (c);
				::GetIndString (option, inStrnID, index + ::GetControlValue (c) - min);
				index += ::GetControlMaximum (c) - min + 1;
				if ((iType & kItemDisableBit) || (option[0] == 0)) continue;
				
				if (noErr != (e = ::CVSAddPStringArg (&outOptions, option))) goto CleanUp;
				continue;
				} // if
			} // for
	
	CleanUp:
	
		return e;
	
	} // end GetOptionsList

// ---------------------------------------------------------------------------
//		¥ SetOptionsList
// ---------------------------------------------------------------------------

#define DEBUG_ADVANCED 0
#if DEBUG_ADVANCED
Str255	sDebugStr;
Str31	sItemNoStr;
Str31	sIndexStr;
Str31	sIndexEndStr;

enum {
	kDebugEditIndex = 1,
	kDebugControlIndex,
	kDebugStringID = 17999
	};
#endif

OSErr 
VCSAdvancedOptionsDialog::SetOptionsList (

	const	AEDescList&		inOptions,
	ResID					inStrnID)

	{ // begin SetOptionsList
		
		OSErr					e = noErr;
		
		long					optionCount;
		if (noErr != (e = ::AECountItems (&inOptions, &optionCount))) goto CleanUp;
		
		const	DialogItemIndex	nItems = CountDITL (GetDialogPtr ());
		short					index = 1;
		for (DialogItemIndex itemNo = kVCSAdvancedFirstFreeItem; itemNo <= nItems; ++itemNo) {
			short		iType;
			Handle		iHandle;
			Rect		iRect;
			Str255		option;
			Str255		value;
			
			::GetDialogItem (GetDialogPtr (), itemNo, &iType, &iHandle, &iRect);
			short		baseType (iType & ~kItemDisableBit);
			if ((kStaticTextDialogItem | kIconDialogItem | kPictureDialogItem) & iType) continue;
			if (baseType == kUserDialogItem) continue;
			if (baseType == kButtonDialogItem) continue;
			
			if (baseType & kEditTextDialogItem) {
				::GetIndString (option, inStrnID, index);
				
				Boolean	found = false;
				for (long i = 1; i <= optionCount; ++i) {
					AEKeyword	theAEKeyword;
					DescType	typeCode;
					Size		actualSize;
					if (noErr != (e = ::AEGetNthPtr	(&inOptions, i, typeChar, &theAEKeyword, &typeCode, value + 1, sizeof (value) - 1, &actualSize))) goto CleanUp;
					value[0] = actualSize;
					
					if (found) {
						::SetDialogItemText (iHandle, value);
						::SelectDialogItemText (GetDialogPtr (), itemNo, 0, 0x7FFF);
						break;
						} // if
						
					found = ::EqualString (option, value, true, true);
					} // for

				++index;
				continue;
				} // if
			
			if (iType & kControlDialogItem) {
				ControlHandle	c = ControlHandle (iHandle);
				SInt16			min = ::GetControlMinimum (c);
				SInt16			range = ::GetControlMaximum (c) - min + 1;
				
#if DEBUG_ADVANCED
				::NumToString (itemNo, sItemNoStr);
				::GetControlTitle (c, option);
				::NumToString (index, sIndexStr);
				::NumToString (index + range - 1, sIndexEndStr);
				::ReplaceInIndString (sDebugStr, kDebugStringID, kDebugControlIndex, sItemNoStr, option, sIndexStr, sIndexEndStr);
				::DebugStr (sDebugStr);
#endif
				for (long i = 1; i <= optionCount; ++i) {
					AEKeyword	theAEKeyword;
					DescType	typeCode;
					Size		actualSize;
					if (noErr != (e = ::AEGetNthPtr	(&inOptions, i, typeChar, &theAEKeyword, &typeCode, value + 1, sizeof (value) - 1, &actualSize))) goto CleanUp;
					value[0] = actualSize;
					
					for (SInt16 r = 0; r < range; ++r) {
						::GetIndString (option, inStrnID, index + r + min);
						if (!::EqualString (option, value, true, true)) continue;
						
						::SetControlValue (c, r + min);
						break;
						} // for
					} // for

				index += range;
				continue;
				} // if
			} // for
	
	CleanUp:
	
		return e;
	
	} // end SetOptionsList


