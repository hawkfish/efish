/*
	File:		VCSPrompt.c

	Contains:	User prompting utilities.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-8 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <7>     11/5/98    rmgw    Save/restore entire scrap.
         <6>     10/8/98    rmgw    Convert to v7 API.
         <5>     9/22/98    rmgw    Convert to CWP3.1.
         <4>     2/22/98    rmgw    Convert to C++.
         <3>     2/11/98    rmgw    Automatic project creation.
         <2>    11/14/97    rmgw    Add VCSPromptString.
         <1>    11/13/97    rmgw    first checked in.
*/


#include "VCSPrompt.h"

#include "VCSDialogPrep.h"
#include "VCSUtil.h"

#include <Dialogs.h>
#include <Scrap.h>
#include <TextUtils.h>

//	=== Constants ===

enum PromptYesNoCancelItems {
	k3ConfirmYesItem = 1,
	k3ConfirmCancelItem,
	k3ConfirmMessageItem,
	k3StopIconItem,
	k3ConfirmNoItem,
	k3ConfirmSameCheckItem,
	
	k3StateConfirmAlert = 16010
	};
	
enum PromptYesNoItems {
	k2ConfirmYesItem = 1,
	k2ConfirmNoItem,
	k2ConfirmMessageItem,
	k2StopIconItem,
	k2ConfirmSameCheckItem,
	
	k2StateConfirmAlert = 16011
	};

//	===	Types ===

typedef struct PromptResponseRecord
{
	struct PromptResponseRecord*	next;
	short 							strnID;
	short 							strIndex;
	PromptResponse 					response;
} PromptResponseRecord;

//	=== Statics ===

static PromptResponseRecord*
sPromptResponseList = 0;

// ---------------------------------------------------------------------------
//		¥ SetPromptResponse
// ---------------------------------------------------------------------------

static void 
SetPromptResponse (

	short 			strnID, 
	short 			strIndex, 
	PromptResponse	response)
	
	{ // begin SetPromptResponse
	
		/* search the list for this entry */
		PromptResponseRecord*	rec;
		for (rec = sPromptResponseList; rec; rec = rec->next) {
			if ((rec->strnID != strnID) || (rec->strIndex != strIndex)) continue;
			
			/* found it */
			rec->response = response;
			return;
			} // for
		
		/* didn't find it, add one */
		rec = (PromptResponseRecord*) NewPtrClear (sizeof (PromptResponseRecord));
		if (!rec) return;
		
		rec->strnID = strnID;
		rec->strIndex = strIndex;
		rec->response = response;
		
		rec->next = sPromptResponseList;
		sPromptResponseList = rec;
	
	} // end SetPromptResponse

// ---------------------------------------------------------------------------
//		¥ GetPromptResponse
// ---------------------------------------------------------------------------

static Boolean 
GetPromptResponse (

	short 			strnID, 
	short 			strIndex, 
	PromptResponse*	response)

	{ // begin GetPromptResponse
	
		/* search the list for this entry */
		const	PromptResponseRecord*	rec;
		for (rec = sPromptResponseList; rec; rec = rec->next) {
			if ((rec->strnID != strnID) || (rec->strIndex != strIndex)) continue;
			
			/* found it */
			*response = rec->response;
			return true;
			} // for

		return false;
	
	} // end GetPromptResponse
	
// ---------------------------------------------------------------------------
//		¥ ClearPromptResponses
// ---------------------------------------------------------------------------

void
ClearPromptResponses (void)

	{ // begin ClearPromptResponses
	
		/* search the list for this entry */
		PromptResponseRecord*	rec;
		for (rec = sPromptResponseList; rec; ) {
			PromptResponseRecord*	kill = rec;
			rec = rec->next;
			
			DisposePtr ((Ptr) kill);
			kill = nil;
			} // for
		
		sPromptResponseList = 0;
		
	} // end ClearPromptResponses

// ---------------------------------------------------------------------------
//		¥ PromptAlertFilter
// ---------------------------------------------------------------------------

static Boolean 
pPersistentFlag = false;

static pascal Boolean 
PromptAlertFilter (

	DialogPtr 		theDialog, 
	EventRecord*	theEvent,
	short*			itemHit)
	
	{ // begin PromptAlertFilter

		SetDialogDefaultItem (theDialog, ok);
		SetDialogCancelItem (theDialog, cancel);
	
		// is it a mouse click in the persistent answer checkbox?
		if (theEvent->what == mouseDown) {
			GrafPtr	savePort;
			Point	pt = theEvent->where;
			short	theItem;
		
			GetPort (&savePort);
			SetPort (theDialog);
		
			GlobalToLocal (&pt);
			theItem = 1 + FindDialogItem (theDialog, pt);
			if (theItem > 0) {
				Handle	h;
				Rect	r;
				short 	type;
			
				GetDialogItem (theDialog, theItem, &type, &h, &r);
				if (type == ctrlItem + chkCtrl) {
					ControlHandle ch = (ControlHandle) h;
					if (TrackControl (ch, pt, nil) != 0) {
						pPersistentFlag = GetControlValue(ch) ? false : true;
						SetControlValue (ch, pPersistentFlag);

						theEvent->what = nullEvent; /* handled it */
						SetPort (savePort);
						
						return false;
						} // if
					} // if
				} // if
		
			SetPort (savePort);
			} // if
	
		return StdFilterProc (theDialog, theEvent, itemHit);
	
	} // end PromptAlertFilter

// ---------------------------------------------------------------------------
//		¥ VCSPrompt
// ---------------------------------------------------------------------------

static short 
VCSPrompt (
	
	const VCSContext&		inPB,
	short					alrtID)
	
	{ // begin VCSPrompt
	
		ModalFilterUPP	filterUPP = NewModalFilterProc (PromptAlertFilter);	
		short			result = 0;
		
		VCSDialogPrep	dlog (inPB);
		result = ::Alert (alrtID, filterUPP);

		DisposeRoutineDescriptor (filterUPP);
		
		return result;
	
	} // end VCSPrompt

// ---------------------------------------------------------------------------
//		¥ VCSPromptYesNoCancel
// ---------------------------------------------------------------------------

PromptResponse 
VCSPromptYesNoCancel (

	const VCSContext&	inPB,
	short 					strnID, 
	short 					strIndex,
	ConstStr255Param 		param1, 
	ConstStr255Param 		param2,
	ConstStr255Param 		param3, 
	ConstStr255Param 		param4)
	
	{ // begin VCSPromptYesNoCancel
	
		PromptResponse 	response;

		Str255 			message;
	
		if (GetPromptResponse (strnID, strIndex, &response))
			return response;
	
		ReplaceInIndString (message, strnID, strIndex, param1, param2, param3, param4);		
		ParamText (message, nil, nil, nil);
		
		pPersistentFlag = false;
		switch (VCSPrompt (inPB, k3StateConfirmAlert)) {
			case k3ConfirmYesItem:
				response = kPromptYes;
				break;
			
			case k3ConfirmCancelItem:
			default:
				response = kPromptCancel;
				break;
			
			case k3ConfirmNoItem:
				response = kPromptNo;
				break;
			} // switch
		
		if (pPersistentFlag)
			SetPromptResponse (strnID, strIndex, response);
	
		return response;
	
	} // end VCSPromptYesNoCancel

// ---------------------------------------------------------------------------
//		¥ VCSPromptYesNo
// ---------------------------------------------------------------------------

PromptResponse 
VCSPromptYesNo (

	const VCSContext&	inPB,
	short 					strnID, 
	short 					strIndex,
	ConstStr255Param 		param1, 
	ConstStr255Param 		param2,
	ConstStr255Param 		param3, 
	ConstStr255Param 		param4)

	{ // begin VCSPromptYesNo
	
		PromptResponse 	response;

		Str255 			message;
	
		if (GetPromptResponse (strnID, strIndex, &response))
			return response;
	
		ReplaceInIndString (message, strnID, strIndex, param1, param2, param3, param4);		
		ParamText (message, nil, nil, nil);
		
		pPersistentFlag = false;
		switch (VCSPrompt (inPB, k2StateConfirmAlert)) {
			case k2ConfirmYesItem:
				response = kPromptYes;
				break;
			
			case k2ConfirmNoItem:
			default:
				response = kPromptNo;
				break;
			} // switch
		
		if (pPersistentFlag)
			SetPromptResponse (strnID, strIndex, response);
	
		return response;
	
	} // end VCSPromptYesNo

// ---------------------------------------------------------------------------
//		¥ VCSPromptComment
// ---------------------------------------------------------------------------

Boolean 
VCSPromptComment (

	VCSContext&				inPB,
	short					inPromptIndex, 
	ConstStr255Param		fileName, 
	Str255 					comment)
	
	{ // begin VCSPromptComment
	
		Boolean		result = false;
		
		OSErr 		e = noErr;
		
		Str255		s;
		Handle		prompt = nil;
		char*		cComment;
		char		null = 0;
		
		GetIndString (s, kPromptStringsID, inPromptIndex);
		if (noErr != (e = PtrToHand (s + 1, &prompt, s[0]))) goto CleanUp;
		if (noErr > (e = ReplaceString (prompt, "\p<1>", fileName))) goto CleanUp;
		if (noErr != (e = PtrAndHand (&null, prompt, sizeof (null)))) goto CleanUp;
		
		
		HLock (prompt);
		cComment = p2cstr (comment);
		result = (cwNoErr == inPB.GetComment (*prompt, cComment, 256)) ? true : false;
		c2pstr (cComment);
		
	CleanUp:
	
		if (prompt) DisposeHandle (prompt);
		prompt = nil;
				
		return result;
	
	} // end VCSPromptComment

#include "VCSStdDialog.h"
#include "VCSDialogTextItem.h"

class VCSPromptStringDialog : public VCSStdDialog

	{
		
		VCSDialogTextItem		mStringText;
		
		StringPtr				mString;
		Size					mSize;
		
	protected:
		
		//	Modal dialog handling
		virtual	Boolean OnItemHit 				(DialogItemIndex		inItemHit);
		virtual	Boolean	PreModalDialog			(void);

	public:
	
		enum PromptStringItems {
			kPromptStringCancelItem = cancel,
			kPromptStringPromptItem,
			kPromptStringTextItem,
			
			kPromptStringFnordItem
			};
	
						VCSPromptStringDialog	(const VCSContext&	inPB,
												 short					inDLOGid,
												 StringPtr 				outString,
												 Size					outSize);
	};
	
	
// ---------------------------------------------------------------------------
//		¥ VCSPromptStringDialog
// ---------------------------------------------------------------------------

VCSPromptStringDialog::VCSPromptStringDialog (

	
	const VCSContext&	inPB,
	short					inDLOGid,
	StringPtr 				outString,
	Size					outSize)
	
	: VCSStdDialog (inPB, inDLOGid)
	
	, mStringText (*this, kPromptStringTextItem)
	
	, mString (outString)
	, mSize (outSize)
	
	{ // begin VCSPromptStringDialog
	
	} // end VCSPromptStringDialog
	
// ---------------------------------------------------------------------------
//		¥ PreModalDialog
// ---------------------------------------------------------------------------

Boolean
VCSPromptStringDialog::PreModalDialog (void)
	
	{ // begin PreModalDialog
		
		//	Initialize the text
		SetItemText (kPromptStringTextItem, mString);
		SelectDialogItemText (GetDialogPtr (), kPromptStringTextItem, 0, 0x7FFF);
		
		return VCSStdDialog::PreModalDialog ();
		
	} // end true

// ---------------------------------------------------------------------------
//		¥ OnItemHit
// ---------------------------------------------------------------------------

Boolean 
VCSPromptStringDialog::OnItemHit (

	short	inItemHit)
	
	{ // begin OnItemHit
		
		Str255		text;

		switch (inItemHit) {
			case ok:
				GetItemText (kPromptStringTextItem, mString);
				break;
				
			case kPromptStringTextItem:
				GetItemText (kPromptStringTextItem, text);
				SetItemEnable (ok, text[0] < mSize);
				break;
			} // switch
		
		return VCSStdDialog::OnItemHit (inItemHit);
		
	} // end OnItemHit
	
// ---------------------------------------------------------------------------
//		¥ VCSPromptString
// ---------------------------------------------------------------------------

short 
VCSPromptString (
	
	const VCSContext&	inPB,
	short					inDLOGid,
	StringPtr 				outString,
	Size					outSize)
	
	{ // begin VCSPromptString
		
		return VCSPromptStringDialog (inPB, inDLOGid, outString, outSize).DoModalDialog ();

	} // end VCSPromptString

