/*
	File:		OptionsPanel.cp

	Contains:	Implementation of the CWProjector Options prefs panel.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <4>    11/18/98    rmgw    Handle resource loading failure.
         <3>     11/3/98    rmgw    Export map structure.
        <1+>     11/3/98    rmgw    Implement.
         <1>     11/3/98    rmgw    first checked in.
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
#include <Palettes.h>
#include <Resources.h>
#include <Sound.h>
#include <Scrap.h>
#include <TextUtils.h>

/* compiler headers */
#include <A4Stuff.h>
#include <SetUpA4.h>
#include <DropInPanel.h>

/* project headers */
#include "OptionsPanel.h"

//	===	Constants ===

enum PanelItems {
	kAddTextKeywordsItem = 1,
	kAddBinaryKeywordsItem,
	kAddGroupItem,
	kAddBeginItem = kAddTextKeywordsItem,
	kAddEndItem = kAddGroupItem,
		
	kGetResetTagsItem,
	kGetPruneItem,
	kGetDirectoriesItem,
	kGetGroupItem,
	kGetBeginItem = kGetResetTagsItem,
	kGetEndItem = kGetGroupItem,

	kCheckinNoProgramItem,
	kCheckinGroupItem,
	kCheckinBeginItem = kCheckinNoProgramItem,
	kCheckinEndItem = kCheckinGroupItem,

	kDiffContextualItem,
	kDiffGroupItem,
	kDiffBeginItem = kDiffContextualItem,
	kDiffEndItem = kDiffGroupItem,

	kStatusAllTagsItem,
	kStatusGroupItem,
	kStatusBeginItem = kStatusAllTagsItem,
	kStatusEndItem = kStatusGroupItem,

	kLabelReplaceItem,
	kLabelBranchItem,
	kLabelCheckItem,
	kLabelDeleteItem,
	kLabelGroupItem,
	kLabelFirstItem = kLabelReplaceItem,
	kLabelLastItem = kLabelGroupItem,

	kRemoteLabelReplaceItem,
	kRemoteLabelBranchItem,
	kRemoteLabelDeleteItem,
	kRemoteLabelPurgeItem,
	kRemoteLabelNoProgramItem,
	kRemoteLabelGroupItem,
	kRemoteLabelFirstItem = kRemoteLabelReplaceItem,
	kRemoteLabelLastItem = kRemoteLabelGroupItem,

	kHistoryInfoItem,
	kHistoryHideLabelsItem,
	kHistoryDefaultBranchItem,
	kHistoryGroupItem,
	kHistoryBeginItem = kHistoryInfoItem,
	kHistoryEndItem = kHistoryGroupItem,

	kClientLabelItem,
	kClientItem,

	kItemListID = 129
	};
	
enum {
	kFactoryPrefsID = 128,
	kGroupBoxStringsID = 128,

	kCommentInfoStr = 1,
	kLabelsInfoStr
};

//	===	Macros ===

#define arraycount(a) (sizeof (a) / sizeof ((a)[0]))
#define handlecount(a) (::GetHandleSize (a) / sizeof (**(a)))

//	===	Static Members ===

static DialogItemIndex 
sGroupItems [] =
	{
	kAddGroupItem,
	kGetGroupItem,
	kCheckinGroupItem,
	kDiffGroupItem,
	kStatusGroupItem,
	kLabelGroupItem,
	kRemoteLabelGroupItem,
	kHistoryGroupItem,
	};

static OptionsRec 
sDefaultOptions = {
	kOptionsPrefVersion,
	{
		0x00000003,
		0x00000000,
		0x00000001,
		0x00000000,
		0x00000010,
		0x00000008,
		0x00000000
		},
	0,
	5,
	0,
	'mCVS'
	};

// ---------------------------------------------------------------------------
//		Ä UpdatePref
// ---------------------------------------------------------------------------
//	"upgrade" a pref to the current version

static short 
FactoryToData (Handle inSettings);

static short 
UpdatePref (

	Handle	inSettings)

	{ // begin UpdatePref
	
		OSErr				err			= noErr;

		OptionsRecHandle	settings	= (OptionsRecHandle) inSettings;
		switch ((*settings)->version) {
			case kOptionsPrefVersion:
				break;
			
			case 1:
				FactoryToData (inSettings);
				break;
				
			default:
				err = kBadPrefVersion;
				break;
			} // switch

		return (err);
	
	} // end UpdatePref

// ---------------------------------------------------------------------------
//		Ä ComparePrefs
// ---------------------------------------------------------------------------

static Boolean	
ComparePrefs (

	Handle prefsHand1, 
	Handle prefsHand2)
	
	{ // begin ComparePrefs
	
		OptionsRecHandle	prefs1	= (OptionsRecHandle) prefsHand1;
		OptionsRecHandle	prefs2	= (OptionsRecHandle) prefsHand2;
		
		for (UInt16 i = 0; i < OptionsRec::kOptionsCount; ++i) {
			if ((**prefs1).options[i] != (**prefs2).options[i]) return false;
			} // for
			
		if ((**prefs1).textKeyword != (**prefs2).textKeyword) return false;
		if ((**prefs1).binaryKeyword != (**prefs2).binaryKeyword) return false;
		if ((**prefs1).historyInfo != (**prefs2).historyInfo) return false;
		
		if ((**prefs1).clientCreator != (**prefs2).clientCreator) return false;
		
		return true;
				
	} // end ComparePrefs

#pragma mark -

// ---------------------------------------------------------------------------
//		Ä Validate
// ---------------------------------------------------------------------------
//	check if panel's changes require a recompile or relink

static void 
Validate (

	Handle original, 
	Handle current, 
	Boolean *recompile, 
	Boolean *relink, 
	Boolean *reset)
	
	{ // begin Validate
	
#pragma unused(original, current)
		
		*recompile	= false;
		*relink		= false;
		*reset		= false;
		
	} // end Validate

// ---------------------------------------------------------------------------
//		Ä DataToDialog
// ---------------------------------------------------------------------------
//	copy the options data from the handle to the screen

const	ResID	kCheckBoxMapID = 128;

static OSErr 
DataToDialog (

	PanelParameterBlock *pb, 
	Handle 				inSettings)

	{ // begin DataToDialog
		
		OSErr	e = UpdatePref (inSettings);
		if (noErr != e) goto CleanUp;
		
		OptionsRecHandle	settings	= (OptionsRecHandle) inSettings;
		
		//	Checkbox mappings
		OptionBitMapRec**	map = (OptionBitMapRec**) ::Get1Resource ('OBMp', kCheckBoxMapID);
		if ((map == nil) || (*map == nil)) {
			e = ::ResError ();
			if (noErr == e) e = resNotFound;
			goto CleanUp;
			} // if

		for (UInt16	i = 0; i < (**map).count; ++i) {
			UInt32	value = (((**settings).options[(**map).bit[i].index] >> (**map).bit[i].shift) & 0x01);
			::CWPanlSetItemValue (pb, (**map).bit[i].key, value ? 1 : 0);
			} // for
		
		//	Menus
		::CWPanlSetItemValue (pb, kAddTextKeywordsItem, (**settings).textKeyword + 1);
		::CWPanlSetItemValue (pb, kAddBinaryKeywordsItem, (**settings).binaryKeyword + 1);
		::CWPanlSetItemValue (pb, kHistoryInfoItem, (**settings).historyInfo + 1);
		
		//	Text boxes
		Str15	creatorString;
		::BlockMoveData (&(**settings).clientCreator, creatorString + 1, creatorString[0] = sizeof ((**settings).clientCreator));
		::CWPanlSetItemText (pb, kClientItem, creatorString);
		
	CleanUp:
	
		return e;
		
	} // end DataToDialog

// ---------------------------------------------------------------------------
//		Ä DialogToData
// ---------------------------------------------------------------------------
//	copy the options data from screen to the handle

static short 
DialogToData (

	PanelParameterBlock*	pb, 
	Handle					inSettings, 
	Boolean 				)
	
	{ // begin DialogToData
		
		OSErr				e = noErr;
		
		OptionsRecHandle	settings	= (OptionsRecHandle) inSettings;
		
		//	Checkbox mappings
		OptionBitMapRec**	map = (OptionBitMapRec**) ::Get1Resource ('OBMp', kCheckBoxMapID);
		if ((map == nil) || (*map == nil)) {
			e = ::ResError ();
			if (noErr == e) e = resNotFound;
			goto CleanUp;
			} // if

		long	value;
		for (UInt16	i = 0; i < (**map).count; ++i) {
			::CWPanlGetItemValue (pb, (**map).bit[i].key, &value);

			UInt32	mask = 0x01;
			mask <<= (**map).bit[i].shift;
			if (value)
				(**settings).options[(**map).bit[i].index] |= mask;
			else (**settings).options[(**map).bit[i].index] &= ~mask;
			} // for
	
		//	Menus
		::CWPanlGetItemValue (pb, kAddTextKeywordsItem, &value);
		(**settings).textKeyword = value - 1;
		
		::CWPanlGetItemValue (pb, kAddBinaryKeywordsItem, &value);
		(**settings).binaryKeyword = value - 1;

		::CWPanlGetItemValue (pb, kHistoryInfoItem, &value);
		(**settings).historyInfo = value - 1;
		
		//	Text boxes
		Str15	creatorString;
		::CWPanlGetItemText (pb, kClientItem, creatorString, sizeof (creatorString));
		while (creatorString[0] < sizeof ((**settings).clientCreator))
			creatorString[++creatorString[0]] = 0;
		::BlockMoveData (creatorString + 1, &(**settings).clientCreator, sizeof ((**settings).clientCreator));
		
	CleanUp:
	
		return (e);
	
	} // end DialogToData

// ---------------------------------------------------------------------------
//		Ä FactoryToData
// ---------------------------------------------------------------------------
//	retrieve factory settings

static short 
FactoryToData (

	Handle inSettings)
	
	{ // begin FactoryToData
	
		OSErr	e = noErr;
		
		Handle factory = ::Get1Resource ('pref', kFactoryPrefsID);
		if ((factory == nil) || (*factory == nil)) {
			e = ::ResError ();
			if (e == noErr) e = resNotFound;
			::SetHandleSize (inSettings, sizeof (sDefaultOptions));
			::BlockMoveData (&sDefaultOptions, *inSettings, ::GetHandleSize (inSettings));
			goto CleanUp;
			} // if
		
		Size	size = ::GetHandleSize (factory);
		::SetHandleSize (inSettings, size);
		if (noErr != (e = ::MemError())) goto CleanUp;
		
		::BlockMoveData (*factory, *inSettings, size);
	
	CleanUp:
	
		return (e);
		
	} // end FactoryToData

#pragma mark -

static DescType
sKeywordsList [] = {
	kKeywordsNone, 
	kKeywordsDefault,
	kKeywordsLocker,
	kKeywordsNames,
	kKeywordsOld,
	kKeywordsBinary,
	kKeywordsValues,
	};

static DescType
sHistoryInfoList [] = {
	kFileInfoFull,
	kFileInfoShort,
	kFileInfoMinimal,
	kFileInfoRSCName,
	};

// ---------------------------------------------------------------------------
//		Ä DataToAE
// ---------------------------------------------------------------------------
//	get a specified Preference setting for an AppleEvent request

const	ResID	kPropertyMapID = 129;

static short 
DataToAE (

	AEKeyword	keyword, 
	AEDesc*		prefsDesc, 
	Handle 		inSettings)

	{ // begin DataToAE
		
		OSErr				e = errAECantHandleClass;
		
		OptionsRecHandle	settings	= (OptionsRecHandle) inSettings;

		switch (keyword) {
			case pAddTextKeyword:
				e = ::AECreateDesc (typeEnumerated, &sKeywordsList[(**settings).textKeyword], sizeof (sKeywordsList[(**settings).textKeyword]), prefsDesc);
				goto CleanUp;

			case pAddBinaryKeyword:
				e = ::AECreateDesc (typeEnumerated, &sKeywordsList[(**settings).binaryKeyword], sizeof (sKeywordsList[(**settings).binaryKeyword]), prefsDesc);
				goto CleanUp;
				
			case pHistoryFileInfo:
				e = ::AECreateDesc (typeEnumerated, &sHistoryInfoList[(**settings).historyInfo], sizeof (sHistoryInfoList[(**settings).historyInfo]), prefsDesc);
				goto CleanUp;
				
			case pClientCreator:
				e = ::AECreateDesc (typeType, &(**settings).clientCreator, sizeof ((**settings).clientCreator), prefsDesc);
				goto CleanUp;
			} // switch

		OptionBitMapRec**	map = (OptionBitMapRec**) ::Get1Resource ('OBMp', kPropertyMapID);
		if ((map == nil) || (*map == nil)) {
			e = ::ResError ();
			if (noErr == e) e = resNotFound;
			goto CleanUp;
			} // if
			
		for (UInt16	i = 0; i < (**map).count; ++i) {
			if ((**map).bit[i].key != keyword) continue;
			
			UInt32	value = (((**settings).options[(**map).bit[i].index] >> (**map).bit[i].shift) & 0x01);
			Boolean	aBoolean = value ? true : false;
			e = ::AECreateDesc (typeBoolean, &aBoolean, sizeof (aBoolean), prefsDesc);
			goto CleanUp;
			} // for

	CleanUp:
		
		return e;
				
	} // end DataToAE

// ---------------------------------------------------------------------------
//		Ä AEToData
// ---------------------------------------------------------------------------
//	set a specified Preference setting from an AppleEvent request

static short 
AEToData (

	AEKeyword 		keyword, 
	const AEDesc*	prefsDesc, 
	Handle 			inSettings)
	
	{ // begin AEToData
		
		OSErr				e = noErr;
		
		OptionsRecHandle	settings	= (OptionsRecHandle) inSettings;
		AEDesc				toDesc	= {typeNull, nil};
		
		Handle				dataHand = nil;
		DescType			enumValue;
		switch (keyword) {
			case pAddTextKeyword:
			case pAddBinaryKeyword:
				if (prefsDesc->descriptorType == typeEnumerated)
					dataHand = prefsDesc->dataHandle;
				
				else {
					if (noErr != (e = ::AECoerceDesc (prefsDesc, typeEnumerated, &toDesc))) goto CleanUp;
					dataHand = toDesc.dataHandle;
					} // else
				
				enumValue = **((DescType**) dataHand);
				for (short i = 0; i < arraycount(sKeywordsList); ++i) {
					if (enumValue != sKeywordsList[i]) break;
					
					switch (keyword) {
						case pAddTextKeyword:
							(**settings).textKeyword = i;
							break;
							
						case pAddBinaryKeyword:
							(**settings).binaryKeyword = i;
							break;
						} // switch
					break;
					} // for
				goto CleanUp;
				
			case pHistoryFileInfo:
				if (prefsDesc->descriptorType == typeEnumerated)
					dataHand = prefsDesc->dataHandle;
				
				else {
					if (noErr != (e = ::AECoerceDesc (prefsDesc, typeEnumerated, &toDesc))) goto CleanUp;
					dataHand = toDesc.dataHandle;
					} // else

				enumValue = **((DescType**) dataHand);
				for (short i = 0; i < arraycount(sHistoryInfoList); ++i) {
					if (enumValue != sHistoryInfoList[i]) break;
					
					(**settings).historyInfo = i;
					break;
					} // for
				goto CleanUp;
				
			case pClientCreator:
				if (prefsDesc->descriptorType == typeType)
					dataHand = prefsDesc->dataHandle;
				
				else {
					if (noErr != (e = ::AECoerceDesc (prefsDesc, typeType, &toDesc))) goto CleanUp;
					dataHand = toDesc.dataHandle;
					} // else
				
				(**settings).clientCreator = **((OSType**) dataHand);
				goto CleanUp;
			} // switch

		OptionBitMapRec**	map = (OptionBitMapRec**) ::Get1Resource ('OBMp', kPropertyMapID);
		if ((map == nil) || (*map == nil)) {
			e = ::ResError ();
			if (noErr == e) e = resNotFound;
			goto CleanUp;
			} // if

		for (UInt16	i = 0; i < (**map).count; ++i) {
			if ((**map).bit[i].key != keyword) continue;

			UInt32	mask = 0x01;
			mask <<= (**map).bit[i].shift;
			
			Handle			dataHand = nil;
			if (prefsDesc->descriptorType == typeBoolean)
				dataHand = prefsDesc->dataHandle;
			
			else {
				if (noErr != (e = ::AECoerceDesc (prefsDesc, typeBoolean, &toDesc))) goto CleanUp;
				dataHand = toDesc.dataHandle;
				} // else

			if (**((Boolean**) dataHand))
				(**settings).options[(**map).bit[i].index] |= mask;
			else (**settings).options[(**map).bit[i].index] &= ~mask;
			goto CleanUp;
			} // for
	
	CleanUp:
	
		::AEDisposeDesc(&toDesc);
		
		return (e);


	} // end AEToData

#pragma mark -

// ---------------------------------------------------------------------------
//		Ä RedrawItem
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
//		Ä PanelDrawBoxCB
// ---------------------------------------------------------------------------

static pascal void 
PanelDrawBoxCB (

	DialogPtr 	dp, 
	short 		inItem)
	
	{ // begin PanelDrawBoxCB
	
		EnterCallback();
		
		for (UInt16 index = 0; index <  arraycount (sGroupItems); ++index) {
			if (sGroupItems[index] != inItem) continue;
			
			Str255	str;
			::GetIndString (str, kGroupBoxStringsID, index + 1);
			::CWPanlDrawUserItemBox (dp, inItem, str);
			} // for
			
		ExitCallback();
	
	} // end PanelDrawBoxCB

// ---------------------------------------------------------------------------
//		Ä PanelDrawBox
// ---------------------------------------------------------------------------

static void 
PanelDrawBox (

	PanelParameterBlock*	pb, 
	DialogItemIndex 		inItem)

	{ // begin PanelDrawBox
	
		for (UInt16 index = 0; index <  arraycount (sGroupItems); ++index) {
			if (sGroupItems[index] != inItem) continue;
			
			Str255	str;
			::GetIndString (str, kGroupBoxStringsID, index + 1);
			::CWPanlDrawPanelBox (pb, inItem, str);
			} // for

	} // end PanelDrawBox

// ---------------------------------------------------------------------------
//		Ä Filter
// ---------------------------------------------------------------------------

static short 
Filter (

	PanelParameterBlock*, 
	EventRecord *		, 
	short *				)
	
	{ // begin Filter
		
		return (noErr);
		
	} // end Filter

// ---------------------------------------------------------------------------
//		Ä ItemHit
// ---------------------------------------------------------------------------
//	handle an itemHit in a Preferences panel

static void 
ItemHit (

	PanelParameterBlock *pb)
	
	{ // begin ItemHit
	
		short	theItem	= pb->itemHit - pb->baseItems;
		
		switch (theItem) {
			case kAddGroupItem:
			case kGetGroupItem:
			case kCheckinGroupItem:
			case kDiffGroupItem:
			case kStatusGroupItem:
			case kLabelGroupItem:
			case kRemoteLabelGroupItem:
			case kHistoryGroupItem:
				break;
			
			case kAddTextKeywordsItem:
			case kAddBinaryKeywordsItem:
			case kHistoryInfoItem:
			case kClientItem:
				break;
				
			default:
				long	value;

				::CWPanlGetItemValue (pb, theItem, &value);
				::CWPanlSetItemValue (pb, theItem, value ? 0 : 1);
				break;
			} // switch
			
		DialogToData (pb, pb->currentPrefs, false);
		pb->canRevert	= !ComparePrefs (pb->originalPrefs, pb->currentPrefs);
		pb->canFactory	= !ComparePrefs (pb->factoryPrefs,  pb->currentPrefs);
	
	} // end ItemHit

// ---------------------------------------------------------------------------
//		Ä ObeyCommand
// ---------------------------------------------------------------------------

static void 
ObeyCommand (

	PanelParameterBlock *pb)
	
	{ // begin ObeyCommand
	
		short		command			= pb->itemHit;
		short		simulatedItem	= 0;
		
		switch (command) {
			case menu_Copy:
				break;
			} //  switch
		
		pb->itemHit = simulatedItem /* ????? + pb->baseItems */;
		
	} // end ObeyCommand

#pragma mark -

// ---------------------------------------------------------------------------
//		Ä InitDialog
// ---------------------------------------------------------------------------

static short 
InitDialog (

	PanelParameterBlock *pb)
	
	{ // begin InitDialog
		
		OSErr	e = noErr;
		
		// The library function will call the IDE to append the dialog items 
		// if possible;  else it will call AppendDITL itself.  This way, you 
		// don't have to worry about it.
		
		if (noErr != (e = CWPanlAppendItems (pb, kItemListID))) goto CleanUp;
		
	CleanUp:
	
		return (e);
		
	} // end InitDialog

// ---------------------------------------------------------------------------
//		Ä TermDialog
// ---------------------------------------------------------------------------

static void 
TermDialog (

	PanelParameterBlock *)

	{ // begin TermDialog
		
	} // end TermDialog

// ---------------------------------------------------------------------------
//		Ä main
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
				
		switch (pb->request)
		{
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
			result = DataToDialog(pb, pb->currentPrefs);
			break;

		case reqGetData:
			/* fill in the given handle with our dialog items */
			result = DialogToData(pb, pb->currentPrefs, true);
			break;

		case reqFilter:
			/* filter an event in the dialog */
			result = Filter(pb, pb->event, &pb->itemHit);
			break;
			
		case reqItemHit:
			/* handle a hit on one of our dialog items */
			ItemHit(pb);
			break;
			
		case reqDrawCustomItem:
			/* handle a request to draw one of our user items (CW/8 and later) */
			theItem = pb->itemHit - pb->baseItems;
			PanelDrawBox (pb, theItem);
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
			theItem = pb->itemHit - pb->baseItems;
			break;
			
		case reqFindStatus:
			break;
			
		case reqObeyCommand:
			ObeyCommand(pb);
			break;
			
		case reqAEGetPref:
			/* return one item in the given handle as an Apple Event descriptor */
			result = DataToAE(pb->prefsKeyword, &pb->prefsDesc, pb->currentPrefs);
			break;

		case reqAESetPref:
			/* change one item in the given handle according to the given Apple Event descriptor */
			result = AEToData(pb->prefsKeyword, &pb->prefsDesc, pb->currentPrefs);
			break;

		case reqValidate:
			/* determine if we need to reset paths, recompile, or relink */
			Validate(pb->originalPrefs, pb->currentPrefs, &pb->recompile, &pb->relink, &pb->reset);
			break;

		case reqGetFactory:
			/* return our factory settings */
			result = FactoryToData(pb->factoryPrefs);
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
		}
		
		ExitCodeResource();
		
		return (result);
}


