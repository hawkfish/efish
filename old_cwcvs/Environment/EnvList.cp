#include "EnvList.h"

#include <Memory.h>
#include <TextUtils.h>

#include <string.h>

//	===	Constants	===

// ---------------------------------------------------------------------------
//		´ EnvListClear
// ---------------------------------------------------------------------------

void 
EnvListClear (

	EnvironmentListHandle	list)
	
	{ // begin EnvListClear
		
		SetHandleSize ((Handle) list, sizeof (**list));
		
		(**list).mVersion = kEnvListCurrentVersion;
		(**list).mVarCount = 0;
		
	} // end EnvListClear
	
// ---------------------------------------------------------------------------
//		´ EnvListEqual
// ---------------------------------------------------------------------------

Boolean	
EnvListEqual (

	EnvironmentListHandle	prefs1, 
	EnvironmentListHandle	prefs2)

	{ // begin EnvListEqual
	
		ConstStr255Param		s1 = (ConstStr255Param) (*prefs1 + 1);
		ConstStr255Param		s2 = (ConstStr255Param) (*prefs2 + 1);
		
		short					i;
		
		if ((**prefs1).mVarCount != (**prefs2).mVarCount) return false;
		
		for (i = 2 * (**prefs1).mVarCount; i > 0; --i) {
			if (!EqualString (s1, s2, true, true)) return false;
			
			s1 += *s1 + 1;
			s2 += *s2 + 1;
			} // for
			
		return true;
	
	} // end EnvListEqual

// ---------------------------------------------------------------------------
//		´ EnvListGet
// ---------------------------------------------------------------------------

Boolean
EnvListGet (

	EnvironmentListPtr		list,
	ConstStr255Param		inKey,
	Str255					outValue,
	ConstStr255Param		inDefault)
	
	{ // begin VCSGetEnv
		
		Boolean				result = false;
		
		//	Loop variables
		ConstStr255Param	key;
		short 				i;
		
		//	Initialize
		outValue[0] = 0;
		if (inDefault) BlockMoveData (inDefault, outValue, inDefault[0] + 1);
		
		//	Search
		key = (ConstStr255Param) (list + 1);
		for (i = (*list).mVarCount; i > 0; --i) {
			ConstStr255Param	value = key + *key + 1;
			if (EqualString (key, inKey, true, true)) {
				BlockMoveData (value, outValue, 1 + value[0]);
				result = true;
				goto CleanUp;
				} // if

			key =  value + *value + 1;
			} // for
	
	CleanUp:
	
		return result;
		
	} // end EnvListGet
	
// ---------------------------------------------------------------------------
//		´ EnvListGet
// ---------------------------------------------------------------------------

Boolean
EnvListGet (

	EnvironmentListHandle	list,
	ConstStr255Param		inKey,
	Str255					outValue,
	ConstStr255Param		inDefault)
	
	{ // begin VCSGetEnv
		
		//	Stuff to clean up
		Handle				h = (Handle) list;
		SignedByte			listState = HGetState (h);

		HLock (h);
		Boolean				result = EnvListGet (*list, inKey, outValue, inDefault);
		
		HSetState (h, listState);
		
		return result;
		
	} // end EnvListGet
	
// ---------------------------------------------------------------------------
//		´ EnvListSet
// ---------------------------------------------------------------------------

OSErr
EnvListSet (

	EnvironmentListHandle	list,
	ConstStr255Param		inKey,
	ConstStr255Param		inValue)
	
	{ // begin EnvListSet
		
		OSErr				e = noErr;
		
		//	Loop variables
		ConstStr255Param	key;
		short 				i;
		
		//	Stuff to clean up
		Handle				h = (Handle) list;
		SignedByte			listState = HGetState (h);
		
		//	Replace the old value
		HLock (h);
		key = (ConstStr255Param) (*list + 1);
		
		for (i = (**list).mVarCount; i > 0; --i) {
			ConstStr255Param	value = key + *key + 1;
			if (EqualString (key, inKey, true, true)) {
				Size	offset = ((Ptr) value) - *h;
				
				HUnlock (h);
				Munger (h, offset, nil, value[0] + 1, inValue, inValue[0] + 1);
				e = MemError ();
				goto CleanUp;
				} // if
			
			key =  value + *value + 1;
			} // for
		
		//	Add a new value
		HUnlock (h);
		if (noErr != (e = PtrAndHand (inKey, h, inKey[0] + 1))) goto CleanUp;
		if (noErr != (e = PtrAndHand (inValue, h, inValue[0] + 1))) goto CleanUp;
		(**list).mVarCount++;
	
	CleanUp:
	
		HSetState (h, listState);
		
		return (e);
		
	} // end EnvListSet
	
// ---------------------------------------------------------------------------
//		´ EnvListToListBox
// ---------------------------------------------------------------------------

void 
EnvListToListBox (

	EnvironmentListHandle	list,
	ListHandle 				listBox,
	const	char			sep)
	
	{ // begin EnvListToListBox
		
		//	Loop variables
		ConstStr255Param	key;
		short 				i;
		
		//	Stuff to clean up
		Handle				h = (Handle) list;
		SignedByte			listState = HGetState (h);
		
		//	Walk the list
		HLock (h);
		key = (ConstStr255Param) (*list + 1);
		
		for (i = (**list).mVarCount; i > 0; --i) {
			ConstStr255Param	value = key + *key + 1;
			Cell				theCell = {0, 0};
			
			theCell.v = LAddRow (1, (**listBox).dataBounds.bottom + 1, listBox);
			LAddToCell (key + 1, key[0], theCell, listBox);
			LAddToCell (&sep, sizeof (sep), theCell, listBox);
			LAddToCell (value + 1, value[0], theCell, listBox);
			
			key =  value + *value + 1;
			} // for
		
	CleanUp:
	
		HSetState (h, listState);
		
	} // end EnvListToListBox

// ---------------------------------------------------------------------------
//		´ EnvListFromListBox
// ---------------------------------------------------------------------------

OSErr 
EnvListFromListBox (

	EnvironmentListHandle	list,
	ListHandle 				listBox,
	const	char			sep)
	
	{ // begin EnvListFromListBox
		
		OSErr				e = noErr;
		
		Cell				theCell;
		DataHandle			dh = (*listBox)->cells;

		//	Loop through the cells
		theCell.h = (**listBox).dataBounds.left;
		theCell.v = (**listBox).dataBounds.top;
		if (!PtInRect (theCell, &(**listBox).dataBounds)) goto CleanUp;
		
		do {
			short	offset;
			short	len;
			DataPtr	keyData;
			DataPtr	valueData;
			Str255	key;
			Str255	value;
			
			//	Get the cell data
			LGetCellDataLocation (&offset, &len, theCell, listBox);
			keyData = *dh + offset;
			valueData = (DataPtr) memchr (keyData, sep, len);
			if (!valueData) continue;
		
			//	Extract the key and value
			BlockMoveData (keyData, key + 1, key[0] = valueData - keyData);
			valueData += sizeof (sep);
			BlockMoveData (valueData, value + 1, value[0] = len - (valueData - keyData));
			
			//	Add them
			if (noErr != (e = EnvListSet (list, key, value))) goto CleanUp;
			} while (LNextCell (true, true, &theCell, listBox));
		
		CleanUp:
		
			return (e);
			
	} // end EnvListFromListBox

// ---------------------------------------------------------------------------
//		´ EnvListToAEDescList
// ---------------------------------------------------------------------------

OSErr 
EnvListToAEDescList (

	EnvironmentListPtr		list,
	AEDescList* 			aeList)
	
	{ // begin EnvListToAEDescList
		
		OSErr				e = noErr;
		
		//	Loop variables
		ConstStr255Param	key;
		short 				i;
		
		//	Walk the list
		key = (ConstStr255Param) (list + 1);
		
		for (i = (*list).mVarCount; i > 0; --i) {
			ConstStr255Param	value = key + *key + 1;
			
			if (noErr != (e = AEPutPtr (aeList, 0, typeChar, key + 1, key[0]))) goto CleanUp;
			if (noErr != (e = AEPutPtr (aeList, 0, typeChar, value + 1, value[0]))) goto CleanUp;
			
			key =  value + *value + 1;
			} // for
		
	CleanUp:
	
		return e;
		
	} // end EnvListToAEDescList

// ---------------------------------------------------------------------------
//		´ EnvListToAEDescList
// ---------------------------------------------------------------------------

OSErr 
EnvListToAEDescList (

	EnvironmentListHandle	list,
	AEDescList* 			aeList)
	
	{ // begin EnvListToAEDescList
		
		//	Stuff to clean up
		Handle				h = (Handle) list;
		SignedByte			listState = HGetState (h);
		
		//	Walk the list
		HLock (h);
		OSErr				e = EnvListToAEDescList (*list, aeList);
		
		//	Restore the state
		HSetState (h, listState);
		
		return e;
		
	} // end EnvListToAEDescList

// ---------------------------------------------------------------------------
//		´ EnvListFromAEDescList
// ---------------------------------------------------------------------------

OSErr 
EnvListFromAEDescList (

	EnvironmentListHandle	list,
	const	AEDescList* 	aeList)
	
	{ // begin EnvListFromAEDescList
		
		OSErr				e = noErr;
		
		long				theCount = 0;
		long				index;
		
		if (noErr != (e = AECountItems (aeList, &theCount))) goto CleanUp;
		
		for (index = 0; index < theCount - 1; ) {
			Str255		key;
			Str255		value;
			AEKeyword	keyword;
			DescType	typeCode;
			Size		actualSize;
			
			if (noErr != (e = AEGetNthPtr (aeList, ++index, typeChar, &keyword, &typeCode, key + 1, sizeof (key) - 1, &actualSize)))
				goto CleanUp;
			key[0] = actualSize;
			
			if (noErr != (e = AEGetNthPtr (aeList, ++index, typeChar, &keyword, &typeCode, value + 1, sizeof (value) - 1, &actualSize)))
				goto CleanUp;
			value[0] = actualSize;
			
			if (noErr != (e = EnvListSet (list, key, value))) goto CleanUp;
			} // for
		
		CleanUp:
		
			return (e);
			
	} // end EnvListFromAEDescList

