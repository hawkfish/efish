/*
 *  SamplePanel.c - 'Sample Panel' Drop-In Preferences
 *
 *  Copyright © 1995 metrowerks inc.  All rights reserved.
 *
 */

/* prevent access to dialog (for a glimpse of the future...) */
#define CW_STRICT_DIALOGS 1

/* standard headers */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* system headers */
#include <AERegistry.h>
#include <Drag.h>
#include <Lists.h>
#include <Palettes.h>
#include <Resources.h>
#include <Scrap.h>
#include <TextUtils.h>
#include <Sound.h>

/* compiler headers */
#include <A4Stuff.h>
#include <SetUpA4.h>
#include <DropInPanel.h>

/* project headers */
#include "EnvPanel.h"


/* current version number for our prefs data */
#define PSAMPLEPANELVERSION		3


enum {
	kFactoryPrefsID = 128,
	kCW7ItemListID = 128,
	kCW8ItemListID,
	
	kEnvListItem = 1,
	kKeyItem,
	kEqualsItem,
	kValueItem,
	kAddItem,
	kRemoveItem,
	kBoxItem,
	
	kSampleStringsID = 128,
	kSampleInfoStr = 1
};

//	===	Globals ===

static	ListHandle
sEnvListBox = nil; 

static const char 
sEquals = '=';


// ---------------------------------------------------------------------------
//		¥ RedrawItem
// ---------------------------------------------------------------------------

static OSErr
RefreshItem (

	PanelParamBlkPtr	pb,
	long				whichItem)
	
	{ // begin RefreshItem
		
		OSErr	e = noErr;
		
		Rect	itemRect;
		
		GrafPtr	dlogPort;
		GrafPtr	savePort;
		
		GetPort (&savePort);
		
		if (noErr != (e = CWPanlGetMacPort (pb, &dlogPort))) goto CleanUp;
		SetPort (dlogPort);
		
		if (noErr != (e = CWPanlGetItemRect (pb, whichItem, &itemRect))) goto CleanUp;
		EraseRect (&itemRect);
		
		if (noErr != (e = CWPanlInvalItem (pb, whichItem))) goto CleanUp;
		
	CleanUp:
	
		SetPort (savePort);
		
		return e;
		
	} // end RefreshItem
	
// ---------------------------------------------------------------------------
//		¥ PanelDrawListBox
// ---------------------------------------------------------------------------

static void
PanelDrawListBox (
	
	ListHandle	inListBox)
	
	{ // begin PanelDrawListBox
		
		Rect	bounds = (**inListBox).rView;
		
		LUpdate ((**inListBox).port->visRgn, inListBox);
		
		InsetRect (&bounds, -1, -1);
		FrameRect (&bounds);

	} // end PanelDrawListBox
	
// ---------------------------------------------------------------------------
//		¥ PanelHandleListClick
// ---------------------------------------------------------------------------

static void 
PanelHandleListClick (

	PanelParameterBlock*	pb, 
	EventRecord*			event,
	ListHandle				inListBox)
	
	{ // begin PanelHandleListClick
	
		Point	localWhere;
		GrafPtr	panelPort;
		
		Boolean	oldSelected = false;
		Cell	oldCell = {0, 0};
		
		Boolean	newSelected = false;
		Cell	newCell = {0, 0};
		
		GrafPtr	savePort;
		
		GetPort (&savePort);
		
		CWPanlGetMacPort (pb, &panelPort);
		SetPort (panelPort);
		
		localWhere = event->where;
		GlobalToLocal (&localWhere);
		
		oldSelected = LGetSelect (true, &oldCell, sEnvListBox);
		LClick (localWhere, event->modifiers, inListBox);
		newSelected = LGetSelect (true, &newCell, sEnvListBox);
		
		if (oldSelected != newSelected) goto CleanUp;
		if (oldSelected && !EqualPt (oldCell, newCell)) goto CleanUp;
		
		//pb->itemHit = 0;
		
	CleanUp:
	
		SetPort (savePort);
	
	} // end PanelHandleListClick

// ---------------------------------------------------------------------------
//		¥ OnEnvListHit
// ---------------------------------------------------------------------------
//	copy the options data from the handle to the screen

static void 
OnEnvListHit (

	PanelParamBlkPtr	pb)
	
	{ // begin OnEnvListHit
		
		const	unsigned	char	null = 0;
		
		Cell						theCell = {0, 0};
		short						offset;
		short						len;
		DataHandle					dh;			/* defined in Lists.h */
		DataPtr						kp;
		DataPtr						vp;
		
		Str255						key;
		Str255						value;
		
		//	Clear the edit boxes
		CWPanlEnableItem (pb, kRemoveItem, false);
		
		//	Get the selection
		if (!sEnvListBox) return;
		if (!LGetSelect (true, &theCell, sEnvListBox)) return;
		
		//	Get the data
		LGetCellDataLocation (&offset, &len, theCell, sEnvListBox);
		if (offset == -1) return;
		
		dh = (*sEnvListBox)->cells;	
		kp = *dh + offset;
		vp = (DataPtr) memchr (kp, sEquals, len);
		if (!vp) return;
		
		//	Extract the key and value
		BlockMoveData (kp, key + 1, key[0] = vp - kp);
		vp += sizeof (sEquals);
		BlockMoveData (vp, value + 1, value[0] = len - (vp - kp));
		
		//	Set the edit boxes
		RefreshItem (pb, kKeyItem);
		RefreshItem (pb, kValueItem);
		
		CWPanlSetItemText (pb, kKeyItem, key);
		CWPanlSetItemText (pb, kValueItem, value);
		
		CWPanlEnableItem (pb, kRemoveItem, true);

	} // end OnEnvListHit

// ---------------------------------------------------------------------------
//		¥ EnvListKeySearchProc
// ---------------------------------------------------------------------------

static short
EnvListKeySearchProc (

	Ptr 	cellPtr, 
	Ptr 	testPtr, 
	short	cellLen, 
	short	testLen)
	
	{ // begin EnvListKeySearchProc
		
		Ptr		equalsPtr = (Ptr) memchr (cellPtr, sEquals, cellLen);
		if (!equalsPtr) return 1;
		
		return IUMagIDString (cellPtr, testPtr, equalsPtr - cellPtr, testLen);
		
	} // end EnvListKeySearchProc
	
// ---------------------------------------------------------------------------
//		¥ EnvListKeySearch
// ---------------------------------------------------------------------------
//	copy the options data from the handle to the screen

static Boolean 
EnvListKeySearch (
	
	Cell*				ioCell,
	ConstStr255Param	inKey,
	ListHandle			inList)

	{ // begin EnvListKeySearch
		
		ListSearchUPP	upp = NewListSearchProc(EnvListKeySearchProc);
		Boolean			result = LSearch (inKey + 1, inKey[0], upp, ioCell, inList);
		DisposeRoutineDescriptor (upp);
		
		return result;
		
	} // end EnvListKeySearch
	
// ---------------------------------------------------------------------------
//		¥ OnAddHit
// ---------------------------------------------------------------------------
//	copy the options data from the handle to the screen

static void 
OnAddHit (

	PanelParamBlkPtr	pb)
	
	{ // begin OnAddHit
		
		Cell	theCell = {0, 0};
		Str255	key;
		Str255	value;
		
		//	Set the edit boxes
		CWPanlGetItemText (pb, kKeyItem, key, sizeof (key));
		CWPanlGetItemText (pb, kValueItem, value, sizeof (key));
		
		if (!EnvListKeySearch (&theCell, key, sEnvListBox)) 
			theCell.v = LAddRow (1, (**sEnvListBox).dataBounds.bottom + 1, sEnvListBox);
		
		LSetCell (key + 1, key[0], theCell, sEnvListBox);
		LAddToCell (&sEquals, sizeof (sEquals), theCell, sEnvListBox);
		LAddToCell (value + 1, value[0], theCell, sEnvListBox);
		
	} // end OnAddHit
	
// ---------------------------------------------------------------------------
//		¥ OnRemoveHit
// ---------------------------------------------------------------------------
//	copy the options data from the handle to the screen

static void 
OnRemoveHit (

	PanelParamBlkPtr	pb)
	
	{ // begin OnRemoveHit
		
		Cell	theCell = {0, 0};
		
		if (!LGetSelect (true, &theCell, sEnvListBox)) return;
		LDelRow (1, theCell.v, sEnvListBox);
		CWPanlEnableItem (pb, kRemoveItem, false);
	
	} // end OnRemoveHit
	
/*
 *	PanelDrawBox
 *
 */
 
static void PanelDrawBox(PanelParameterBlock *pb, long whichItem, short strIndex)
{
	Str255	str;
	
	GetIndString(str, kSampleStringsID, strIndex);
	CWPanlDrawPanelBox(pb, whichItem, str);
}


// ---------------------------------------------------------------------------
//		¥ PanelDrawBoxCB
// ---------------------------------------------------------------------------
 
static pascal void 
PanelDrawBoxCB (

	DialogPtr	dp, 
	short		item)
	
	{ // begin PanelDrawBoxCB
	
		Str255	str;
		
		EnterCallback();
		
		GetIndString (str, kSampleStringsID, kSampleInfoStr);
		CWPanlDrawUserItemBox (dp, item, str);
		
		ExitCallback ();
		
	} // end PanelDrawBoxCB

// ---------------------------------------------------------------------------
//		¥ InitDialog
// ---------------------------------------------------------------------------
//	initialize Dialog Box items for this panel

static short 
InitDialog (

	PanelParamBlkPtr ppb)

	{ // begin InitDialog
	
		short	ditlID;
		OSErr	err;
		
		// The library function will call the IDE to append the dialog items 
		// if possible;  else it will call AppendDITL itself.  This way, you 
		// don't have to worry about it.
		
		if (ppb->version < DROPINPANELAPIVERSION_2)
			ditlID = kCW7ItemListID;
		else
			ditlID = kCW8ItemListID;
		
		err = CWPanlAppendItems(ppb, ditlID);
		if (err != noErr)
			return (err);
		
		if (ppb->version < DROPINPANELAPIVERSION_2) {
			// CW/7 API -- we have to do install user item procs ourselves
			
			CWPanlInstallUserItem(ppb, kBoxItem, PanelDrawBoxCB);
			} // if
		
		//	Create the list box
		{
			Rect	rView;
			Rect	rDataBnds = {0, 0, 0, 1};
			Point	cellSize = {0, 0};
			GrafPtr	theWindow = nil;
			
			CWPanlGetMacPort (ppb, &theWindow);
			CWPanlGetItemRect (ppb, kEnvListItem, &rView);
			rView.right -= 15;
			sEnvListBox = LNew (&rView, &rDataBnds, cellSize, 0, theWindow, false, false, false, true);
			}
		if (sEnvListBox == nil) return memFullErr;
		
		LSetDrawingMode (true, sEnvListBox);

		return (err);
		
	} // end InitDialog

// ---------------------------------------------------------------------------
//		¥ TermDialog
// ---------------------------------------------------------------------------
//	destroy Dialog Box items for this panel

static void 
TermDialog (

	PanelParameterBlock *pb)
	
	{ // begin TermDialog
		
		if (sEnvListBox) LDispose (sEnvListBox);
		sEnvListBox = nil;
		
		if (pb->version < DROPINPANELAPIVERSION_2)
		{
			// CW/7 API -- we have to release the memory we allocated in InitDialog
			
			CWPanlRemoveUserItem(pb, kBoxItem);
		}
		
	} // end TermDialog

// ---------------------------------------------------------------------------
//		¥ ComparePrefs
// ---------------------------------------------------------------------------

static Boolean	
ComparePrefs (

	Handle	prefsHand1, 
	Handle	prefsHand2)

	{ // begin ComparePrefs
	
		EnvironmentListHandle	prefs1	= (EnvironmentListHandle) prefsHand1;
		EnvironmentListHandle	prefs2	= (EnvironmentListHandle) prefsHand2;
		
		return EnvListEqual (prefs1, prefs2);
	
	} // end ComparePrefs

// ---------------------------------------------------------------------------
//		¥ GetPref
// ---------------------------------------------------------------------------
//	get a specified Preference setting for an AppleEvent request

static OSErr 
GetPref (

	AEKeyword	keyword, 
	AEDesc*		prefsDesc, 
	Handle 		settings)
	
	{ // begin GetPref
		
		OSErr					e = errAECantHandleClass;
		
		EnvironmentListHandle	prefs	= (EnvironmentListHandle) settings;
		
		switch (keyword) {
			case prefsPR_EnvList:
				if (noErr != (e = AECreateList (nil, 0, false, prefsDesc))) goto CleanUp;
				if (noErr != (e = EnvListToAEDescList (prefs, prefsDesc))) goto CleanUp;
				break;
			} // switch
	
	CleanUp:
	
		return (e);
	
	} // end GetPref

// ---------------------------------------------------------------------------
//		¥ SetPref
// ---------------------------------------------------------------------------
//	set a specified Preference setting from an AppleEvent request

static OSErr 
SetPref (

	AEKeyword 		keyword, 
	const AEDesc*	prefsDesc, 
	Handle 			settings)
	
	{ // begin SetPref
		
		OSErr					e = errAECantHandleClass;
		
		EnvironmentListHandle	prefs	= (EnvironmentListHandle) settings;
		AEDescList				listDesc = {typeNull, nil};
		
		
		switch (keyword) {
			case prefsPR_EnvList:
				if (noErr != (e = AECoerceDesc (prefsDesc, typeAEList, &listDesc))) goto CleanUp;
				EnvListClear (prefs);
				if (noErr != (e = EnvListFromAEDescList (prefs, &listDesc))) goto CleanUp;
				break;
			} // switch

	CleanUp:
	
		AEDisposeDesc (&listDesc);

		return (e);
	
	} // end SetPref

// ---------------------------------------------------------------------------
//		¥ GetFactory
// ---------------------------------------------------------------------------
//	retrieve factory settings

static short 
GetFactory ( 
	
	Handle settings)

	{ // begin GetFactory
	
		OSErr	e = noErr;

		Handle	factory;
		Size	size;
		
		factory = Get1Resource ('pref', kFactoryPrefsID);
		if (factory == nil) {
			if (noErr == (e = ResError())) e = resNotFound;
			
			goto CleanUp;
			} // if
		
		size = GetHandleSize(factory);
		SetHandleSize(settings, size);
		if (noErr != (e = MemError ())) goto CleanUp;
		
		BlockMoveData (*factory, *settings, size);
	
	CleanUp:
	
		return (e);
		
	} // end GetFactory

// ---------------------------------------------------------------------------
//		¥ UpdatePref
// ---------------------------------------------------------------------------
//	"upgrade" a pref to the current version

static short 
UpdatePref (

	Handle settings)
	
	{ // begin UpdatePref
	
		#pragma unused(settings)

		return (noErr);
		
	} // end UpdatePref

// ---------------------------------------------------------------------------
//		¥ PutData
// ---------------------------------------------------------------------------
//	copy the options data from the handle to the screen

static void 
PutData (

	PanelParamBlkPtr	pb, 
	Handle 				options)
	
	{ // begin PutData
		
		EnvironmentListHandle		prefs	= (EnvironmentListHandle) options;
		const	unsigned	char	null = 0;
		
		if (!sEnvListBox) return;
		
		//	Turn off drawing
		LSetDrawingMode (false, sEnvListBox);
		
		LDelRow (0, 0, sEnvListBox);
		LDelColumn (0, 0, sEnvListBox);
		LAddColumn (1, 1, sEnvListBox);
		
		//	Copy the list to the list box
		EnvListToListBox (prefs, sEnvListBox, sEquals);
		
		//	Refresh the list
		LSetDrawingMode (true, sEnvListBox);
		RefreshItem (pb, kEnvListItem);
		
		OnEnvListHit (pb);
		
	} // end PutData

// ---------------------------------------------------------------------------
//		¥ GetData
// ---------------------------------------------------------------------------
//	copy the options data from screen to the handle

static short 
GetData (

	PanelParameterBlock *, 
	Handle 				options,
	Boolean 			noisy)
	
	{ // begin GetData
		
		OSErr					e = noErr;
		
		EnvironmentListHandle	prefs = (EnvironmentListHandle) options;
		
		EnvListClear (prefs);
		if (noErr != (e = EnvListFromListBox (prefs, sEnvListBox, sEquals))) goto CleanUp;
		
	CleanUp:
		
		if ((noErr != e) && noisy) SysBeep (10);
		
		return (e);
		
	} // end GetData

// ---------------------------------------------------------------------------
//		¥ Filter
// ---------------------------------------------------------------------------
//	filter an event for the Preferences panel

static short 
Filter (

	PanelParameterBlock*	pb, 
	EventRecord*			event, 
	short*					itemHit)
	
	{ // begin Filter
		
		#pragma unused (pb,event,itemHit)
		return (noErr);
	
	} // end Filter

// ---------------------------------------------------------------------------
//		¥ ItemHit
// ---------------------------------------------------------------------------
//	handle an itemHit in a Preferences panel

static void 
ItemHit (
	
	PanelParameterBlock *pb)

	{ // begin ItemHit
		
		Str255		key;
		
		switch (pb->itemHit - pb->baseItems) {
			case kEnvListItem:
				OnEnvListHit (pb);
				break;
				
			case kAddItem:
				OnAddHit (pb);
				break;
				
			case kRemoveItem:
				OnRemoveHit (pb);
				break;
			} // switch
		
		CWPanlGetItemText (pb, kKeyItem, key, sizeof (key));
		CWPanlEnableItem (pb, kAddItem, key[0] ? true : false);

		GetData (pb, pb->currentPrefs, false);
		
		pb->canRevert	= !ComparePrefs (pb->originalPrefs, pb->currentPrefs);
		pb->canFactory	= !ComparePrefs (pb->factoryPrefs,  pb->currentPrefs);
		
	} // end ItemHit

/*
 *	Validate	-	check if panel's changes require a recompile or relink
 *
 */

static void Validate(Handle original, Handle current, Boolean *recompile, Boolean *relink, Boolean *reset)
{
#pragma unused(original, current)
	
	*recompile	= false;
	*relink		= false;
	*reset		= false;
}

/*
 *	FindStatus
 *
 */
static void FindStatus(PanelParameterBlock *pb)
{
	short	command	= pb->itemHit;
	Boolean	enabled	= false;
	
	pb->itemHit = enabled;
}

/*
 *	ObeyCommand
 *
 */

static void ObeyCommand(PanelParameterBlock *pb)
{
	short		command			= pb->itemHit;
	short		simulatedItem	= 0;
	
	switch (command)
	{
	case menu_Copy:
		break;
	}
	
	pb->itemHit = simulatedItem /* ????? + pb->baseItems */;
}

// ---------------------------------------------------------------------------
//		¥ main
// ---------------------------------------------------------------------------
//	entry-point for Drop-In Preferences Panel
 
pascal short 
main (

	PanelParameterBlock *pb)
	
	{ // begin main
	
		short	result, theItem;
		
		EnterCodeResource();
		
		PrepareCallback();
		
		result = noErr;
				
		switch (pb->request) {
			case reqInitPanel:
				/* panel has just been loaded into memory */
				break;

			case reqTermPanel:
				/* panel is about to be unloaded from memory */
				break;

			case reqInitDialog:
				/* hook our dialog item list into the preferences dialog */
				result = InitDialog(pb);
				break;
				
			case reqTermDialog:
				/* unhook our dialog item list from the preferences dialog */
				TermDialog(pb);
				break;
			
			case reqPutData:
				/* put the data in the given handle into our dialog items */
				PutData(pb, pb->currentPrefs);
				break;

			case reqGetData:
				/* fill in the given handle with our dialog items */
				result = GetData(pb, pb->currentPrefs, true);
				break;

			case reqFilter:
				/* filter an event in the dialog */
				result = Filter(pb, pb->event, &pb->itemHit);
				break;
				
			case reqItemHit:
				/* handle a hit on one of our dialog items */
				ItemHit (pb);
				break;
				
			case reqDrawCustomItem:
				/* handle a request to draw one of our user items (CW/8 and later) */
				theItem = pb->itemHit - pb->baseItems;
				switch (theItem) {
					case kBoxItem:
						PanelDrawBox(pb, theItem, kSampleInfoStr);
						break;
						
					case kEnvListItem:
						PanelDrawListBox (sEnvListBox);
						break;
					} // switch
				break;
				
			case reqActivateItem:
				theItem = pb->itemHit - pb->baseItems;
				break;
				
			case reqDeactivateItem:
				theItem = pb->itemHit - pb->baseItems;
				break;
				
			case reqHandleKey:
				theItem = pb->itemHit - pb->baseItems;
				break;
				
			case reqHandleClick:
				switch (theItem = pb->itemHit - pb->baseItems) {
					case kEnvListItem:
						PanelHandleListClick (pb, pb->event, sEnvListBox);
						break;
					} // switch
				break;
				
			case reqFindStatus:
				FindStatus(pb);
				break;
				
			case reqObeyCommand:
				ObeyCommand(pb);
				break;
				
			case reqAEGetPref:
				/* return one item in the given handle as an Apple Event descriptor */
				result = GetPref(pb->prefsKeyword, &pb->prefsDesc, pb->currentPrefs);
				break;

			case reqAESetPref:
				/* change one item in the given handle according to the given Apple Event descriptor */
				result = SetPref(pb->prefsKeyword, &pb->prefsDesc, pb->currentPrefs);
				break;

			case reqValidate:
				/* determine if we need to reset paths, recompile, or relink */
				Validate(pb->originalPrefs, pb->currentPrefs, &pb->recompile, &pb->relink, &pb->reset);
				break;

			case reqGetFactory:
				/* return our factory settings */
				result = GetFactory(pb->factoryPrefs);
				break;

			case reqUpdatePref:
				/* update the given handle to use the current format for our prefs data */
				result = UpdatePref(pb->currentPrefs);
				break;
				
			case reqDragEnter:
				/* determine if we can accept the drag and, if so, start tracking */
				break;
			
			case reqDragWithin:
				/* continue tracking */
				break;
			
			case reqDragExit:
				/* stop tracking */
				break;
			
			case reqDragDrop:
				/* the user has dropped in our panel */
				break;
			
			default:
				result = paramErr;
				break;
			} // switch
		
		ExitCodeResource();
		
		return (result);
	
	} // end main
