/*
	File:		CMResultsDialog.cp

	Contains:	Implementation of the CMM Results dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-9 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <6>      5/6/99    rmgw    Use Nav services to anchor the save dialog.
         <5>      4/7/99    rmgw    Support CreateDocument.
         <4>      4/1/99    rmgw    Pass event processing up to superclass.
         <3>     3/30/99    rmgw    Crash recovery merging.
         <2>     3/22/99    rmgw    Grayscale dialogs.
         <1>     3/22/99    rmgw    first checked in.
*/


#include "CMResultsDialog.h"

#include <Fonts.h>
#include <Resources.h>
#include <Script.h>
#include <TextUtils.h>
#include <Navigation.h>

#include <string.h>

// ---------------------------------------------------------------------------
//		¥ DoSaveText
// ---------------------------------------------------------------------------

OSErr 
CMResultsDialog::DoSaveText (

	ConstStringPtr		origName,
	Handle				inData,
	StScrpHandle		inStyle,
	const	FSSpec*		inLocation)
	
	{ // begin DoSaveText
		
		OSErr		e = noErr;
		
		SInt16		refNum = -1;
		SInt16		resFile = -1;

		SignedByte	hState = ::HGetState (inData);
		AEDesc		defaultLocation = {typeNull, nil};
		
		do {
			if (!NavServicesAvailable ()) {
				e = userCanceledErr;
				break;
				} // if
			
			if (inLocation && (noErr != ::AECreateDesc (typeFSS, inLocation, sizeof (*inLocation), &defaultLocation))) inLocation = 0;
				
			//	Ask where to save the document
			NavDialogOptions	dialogOptions;
			if (noErr != (e = ::NavGetDefaultDialogOptions (&dialogOptions))) break;
			dialogOptions.dialogOptionFlags |= kNavNoTypePopup;
			dialogOptions.dialogOptionFlags &= ~kNavAllowMultipleFiles;
			::BlockMoveData (origName + 1, dialogOptions.savedFileName + 1, dialogOptions.savedFileName[0] = origName[0]);
				
			NavReplyRecord		reply;
			if (noErr != (e = ::NavPutFile (inLocation ? &defaultLocation : 0, &reply, &dialogOptions, nil, 'TEXT', 'ttxt', nil))) break;
				
			//	Extract it
			FSSpec				fileSpec;
			AEKeyword			theAEKeyword;
			DescType			typeCode;
			Size				actualSize;
			if (noErr != (e = ::AEGetNthPtr (&reply.selection, 1, typeFSS, &theAEKeyword, &typeCode, &fileSpec, sizeof (fileSpec), &actualSize))) break;

			//	Create the file
			if (reply.replacing) ::FSpDelete (&fileSpec);
			::FSpCreateResFile (&fileSpec, 'ttxt', 'TEXT', reply.keyScript);
			if (noErr != (e = ::ResError ())) break;
			
			//	Open the file
			if (noErr != (e = ::FSpOpenDF (&fileSpec, fsRdWrPerm, &refNum))) break;
			
			//	Write the data
			::HLock (inData);
			Size	inOutCount = ::GetHandleSize (inData);
			if (noErr != (e = ::FSWrite (refNum, &inOutCount, *inData))) break;
			
			//	Check for a style
			if (!inStyle) break;
			
			//	Open the resource fork
			resFile = ::FSpOpenResFile (&fileSpec, fsRdWrPerm);
			if (noErr != (e = ::ResError ())) break;
			
			//	Copy the style
			Handle	styl = (Handle) inStyle;
			if (noErr != (e = ::HandToHand (&styl))) break;
			
			//	Add the style
			::AddResource (styl, 'styl', 128, nil);
			if (noErr != (e = ::ResError ())) break;
			} while (false);
			
		::AEDisposeDesc (&defaultLocation);

		if (resFile != -1) ::CloseResFile (resFile);
		resFile = -1;
		
		if (refNum != -1) ::FSClose (refNum);
		refNum = -1;
		
		::HSetState (inData, hState);
		
		return e;
		
	} // end DoSaveText

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ CMResultsDialog
// ---------------------------------------------------------------------------

CMResultsDialog::CMResultsDialog (

	const VCSContext&	inContext)
	
	: VCSDialog (inContext, kResourceID)
	
	, mContext (inContext)
	
	, mOKButton (*this, ok)
	, mTextScroll (*this, kScrollItem)
	, mText (*this, kResultsItem, &mTextScroll)
	, mCopyButton (*this, kCopyItem)
	, mSaveButton (*this, kSaveItem)
	
	{ // begin CMResultsDialog
	
		::SetPort (GetDialogPtr ());
		::TextFont (kPlatformDefaultGuiFontID);
		::TextFace (bold);
		::TextSize (10);
		
	} // end CMResultsDialog
	
// ---------------------------------------------------------------------------
//		¥ ~CMResultsDialog
// ---------------------------------------------------------------------------

CMResultsDialog::~CMResultsDialog (void)

	{ // begin ~CMResultsDialog
	
	} // end ~CMResultsDialog

// ---------------------------------------------------------------------------
//		¥ OnSave
// ---------------------------------------------------------------------------

void 
CMResultsDialog::OnSave (void)
	
	{ // begin OnSave
		
		Str63				origName;
		::GetWTitle (GetDialogPtr (), origName);
		
		CWVCSDatabaseConnection	db;
		mContext.GetDatabase (db);
		
		DoSaveText (origName, mText.GetText (), mText.GetStyleScrapHandle (), &db.sProjectRoot);
		
	} // end OnSave
	
// ---------------------------------------------------------------------------
//		¥ OnOK
// ---------------------------------------------------------------------------

Boolean 
CMResultsDialog::OnOK (void)
	
	{ // begin OnOK
	
		return false;
				
	} // end OnOK
	
// ---------------------------------------------------------------------------
//		¥ OnItemHit
// ---------------------------------------------------------------------------

Boolean 
CMResultsDialog::OnItemHit (

	DialogItemIndex	inItemHit)
	
	{ // begin OnItemHit
		
		Size	selStart;
		Size	selEnd;
		
		switch (inItemHit) {
			case ok:
				return OnOK ();
				
			case kCopyItem:
				mText.GetSelect (selStart, selEnd);
				mText.SetSelect ();
				mText.Copy ();
				mText.SetSelect (selStart, selEnd);
				break;
				
			case kSaveItem:
				OnSave ();
				break;
			} // switch
		
		return VCSDialog::OnItemHit (inItemHit);
		
	} // end OnItemHit
	
// ---------------------------------------------------------------------------
//		¥ PreModalDialog
// ---------------------------------------------------------------------------

Boolean
CMResultsDialog::PreModalDialog (void)
	
	{ // begin PreModalDialog
		
		//	Initialize the default buttons
		SetDialogDefaultItem (GetDialogPtr (), ok);
		SetDialogCancelItem (GetDialogPtr (), ok);
		SetDialogTracksCursor (GetDialogPtr (), true);
		
		::BringToFront (GetDialogPtr ());
		
		return VCSDialog::PreModalDialog ();
		
	} // end true
	
// ---------------------------------------------------------------------------
//		¥ OnFilterEvent
// ---------------------------------------------------------------------------

Boolean 
CMResultsDialog::OnFilterEvent (

	EventRecord&		theEvent,
	DialogItemIndex&	itemHit)
	
	{ // begin OnFilterEvent
		
		//	Set up the port
		SavePort	savePort;
		SetPort (GetDialogPtr ());
					
		// is it a mouse click in the persistent answer checkbox?
		switch (theEvent.what) {
			case keyDown:
				if (!theEvent.modifiers & cmdKey) break;
				if ((theEvent.message & charCodeMask) != 'c') break;
				
				mText.Copy ();
				itemHit = mText.GetIndex ();
				return true;
			} // switch
	
		return VCSDialog::OnFilterEvent (theEvent, itemHit);
	
	} // end OnFilterEvent

// ---------------------------------------------------------------------------
//		¥ MapEOL
// ---------------------------------------------------------------------------

void
CMResultsDialog::MapEOL (void)

	{ // begin MapEOL

		//	Fix EOLs
		static	char	cr = 0x0D;
		static	char	nl = 0x0A;
		Handle			text (mText.GetText ());
		Boolean			changed = false;

		while (0 <= ::Munger (text, 0, &nl, sizeof (nl), &cr, sizeof (cr))) 
			changed = true;
		
		if (changed) mText.CalText ();
		
	} // end MapEOL
	
// ---------------------------------------------------------------------------
//		¥ MessageOutput
// ---------------------------------------------------------------------------

void
CMResultsDialog::MessageOutput (

	short 			errorlevel, 
	const char*		inLine1, 
	const char*		inLine2, 
	long 			/*inErrorNumber*/)
	
	{ // begin MessageOutput
		
		const	RGBColor	errColor = {0xFFFF, 0, 0};
		const	RGBColor	warnColor = {0, 0xFFFF, 0};
		const	RGBColor	infoColor = {0, 0, 0xFFFF};
		const	RGBColor	textColor = {0, 0, 0};
		const	char		cr = 0x0D;
		
		TextStyle style;
		style.tsFont = 1;
		style.tsFace = normal;
		style.tsSize = 9;
		
		switch (errorlevel) {
			case messagetypeError:
				style.tsColor = errColor;
				break;
				
			case messagetypeWarning:
				style.tsColor = warnColor;
				break;
			
			case messagetypeInfo:
				style.tsColor = infoColor;
				break;

			default:
				style.tsColor = textColor;
				break;
			} // switch
		
		//	Line 1
		Size	oldLength (mText.GetLength ());
		Size	delta (::strlen (inLine1));
		mText.SetSelect (oldLength, oldLength);
		mText.Insert (inLine1, delta);
		mText.Insert (&cr, sizeof (cr));
		mText.SetSelect (oldLength, oldLength + delta);
		mText.SetStyle (style);
		
		//	Line 2
		oldLength = mText.GetLength ();
		delta = ::strlen (inLine2);
		style.tsColor = textColor;
		mText.SetSelect (oldLength, oldLength);
		mText.Insert (inLine2, delta);
		mText.Insert (&cr, sizeof (cr));
		mText.SetSelect (oldLength, oldLength + delta);
		mText.SetStyle (style);
		
		//	Select everything
		mText.SetSelect ();
		
		//	Fix EOL
		MapEOL ();
		
	} // end MessageOutput

// ---------------------------------------------------------------------------
//		¥ CreateDocument
// ---------------------------------------------------------------------------

void
CMResultsDialog::CreateDocument (

	const	char*	inName,
	Handle			inData,
	Boolean			/*inDirty*/)
		
	{ // begin CreateDocument
		
		//	Get the window title
		Str255		wTitle;
		::BlockMoveData (inName, wTitle + 1, wTitle[0] = ::strlen (inName));
		
		//	Get the data size
		Size		dataSize (::GetHandleSize (inData));
		if (dataSize > 32000) {
			DoSaveText (wTitle, inData, nil);
			return;
			} // if
		
		//	Display it in a dialog
		SignedByte	hState = ::HGetState (inData);
		do {
			::MoveHHi (inData);
			::HLock (inData);
			
			mText.Insert (*inData, ::GetHandleSize (inData));
			MapEOL ();
			
			::SetWTitle (GetDialogPtr (), wTitle);
			DoModalDialog ();
			} while (false);
			
		::HSetState (inData, hState);
		
	} // end CreateDocument


