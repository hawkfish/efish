// ===========================================================================
//	VCSUtil.c ©1997-2001 Electric Fish, Inc. All Rights Reserved.
// ===========================================================================

#include "VCSUtil.h"

#include "VCSError.h"
#include "VCSResult.h"
#include "VCSTask.h"

#include "CVSCommand.h"
#include "StHandle.h"

#include "MoreFilesExtras.h"

#include <Errors.h>
#include <Folders.h>
#include <LowMem.h>
#include <TextUtils.h>

//	=== Constants ===

#if !CALL_NOT_IN_CARBON

// ---------------------------------------------------------------------------
//		¥ p2cstr
// ---------------------------------------------------------------------------

char*
p2cstr (
	
	StringPtr	src)
	
	{ // begin p2cstr
		
		char*	dst = (char*) src;
		::p2cstrcpy (dst, src);
		
		return dst;
		
	} // end p2cstr

// ---------------------------------------------------------------------------
//		¥ c2pstr
// ---------------------------------------------------------------------------

StringPtr
c2pstr (
	
	char *	src)
	
	{ // begin c2pstr
		
		StringPtr	dst = (StringPtr) src;
		::c2pstrcpy (dst, src);

		return dst;

	} // end c2pstr

#else
// ---------------------------------------------------------------------------
//		¥ GetCurrentScrap
// ---------------------------------------------------------------------------

pascal OSStatus
GetCurrentScrap (

	ScrapRef*	outScrap)
	
	{ // begin GetCurrentScrap
		
		*outScrap = (ScrapRef) InfoScrap ();
		
		return noErr;
		
	} // end GetCurrentScrap

// ---------------------------------------------------------------------------
//		¥ ClearCurrentScrap
// ---------------------------------------------------------------------------

pascal OSStatus
ClearCurrentScrap (void)
	
	{ // begin ClearCurrentScrap
		
		::ZeroScrap ();
		
		return noErr;
		
	} // end ClearCurrentScrap

// ---------------------------------------------------------------------------
//		¥ PutScrapFlavor
// ---------------------------------------------------------------------------

pascal OSStatus
PutScrapFlavor (
	
	ScrapRef               ,
	ScrapFlavorType        flavorType,
	ScrapFlavorFlags       ,
	Size                   flavorSize,
	const void *           flavorData)

	{ // begin PutScrapFlavor
		
		return ::PutScrap (flavorSize, flavorType, flavorData);

	} // end PutScrapFlavor

// ---------------------------------------------------------------------------
//		¥ GetScrapFlavorSize
// ---------------------------------------------------------------------------

pascal OSStatus
GetScrapFlavorSize (
	
	ScrapRef               ,
	ScrapFlavorType        flavorType,
	Size                   *flavorSize)

	{ // begin GetScrapFlavorSize
		
		SInt32	offset;
		SInt32	byteCount = ::GetScrap (nil, flavorType, &offset);
		if (byteCount < 0) return byteCount;
		
		*flavorSize = byteCount;
		
		return noErr;
		
	} // end GetScrapFlavorSize

// ---------------------------------------------------------------------------
//		¥ GetScrapFlavorData
// ---------------------------------------------------------------------------

pascal OSStatus
GetScrapFlavorData (

	ScrapRef               ,
	ScrapFlavorType        flavorType,
	Size *                 flavorSize,
	void *                 destination)

	{ // begin GetScrapFlavorData
		
		OSStatus	result;
		Handle		data = nil;
		
		do {
			data = NewHandle (0);
			if (noErr != (result = MemError ())) break;
			
			SInt32	offset;
			result = ::GetScrap (data, flavorType, &offset);
			if (result < 0) break;
		
			*flavorSize = result;
			::BlockMoveData (*data, destination, *flavorSize);
			} while (false);
			
		if (data) DisposeHandle (data);
		data = nil;
		
		return result;
		
	} // end GetScrapFlavorData
	
// ---------------------------------------------------------------------------
//		¥ CheckEventQueueForUserCancel
// ---------------------------------------------------------------------------

pascal Boolean 
CheckEventQueueForUserCancel (void)

	{ // begin CheckEventQueueForUserCancel
		
		Boolean	foundEvent = false;
		
		//	Scan event queue
		QHdrPtr		eventQHdr = LMGetEventQueue ();
		
		for (EvQElPtr eventQPtr = (EvQElPtr) eventQHdr->qHead; eventQPtr; eventQPtr = (EvQElPtr) eventQPtr->qLink) {
			if ((eventQPtr->evtQWhat != keyDown) && (eventQPtr->evtQWhat != autoKey)) continue;
			if (0 == (eventQPtr->evtQModifiers & cmdKey)) continue;
			
			EventRecord		qEvent = {
										eventQPtr->evtQWhat,
										eventQPtr->evtQMessage,
										eventQPtr->evtQWhen,
										eventQPtr->evtQWhere,
										eventQPtr->evtQModifiers
										};
			if (!::IsCmdChar (&qEvent, 0x2E)) continue;
			
			foundEvent = true;
			break;
			} // for
		
		return foundEvent;

	} // end CheckEventQueueForUserCancel
#endif

#if !ACCESSOR_CALLS_ARE_FUNCTIONS

#include <ControlDefinitions.h>

// ---------------------------------------------------------------------------
//		¥ GetControlPopupMenuHandle
// ---------------------------------------------------------------------------

pascal MenuHandle
GetControlPopupMenuHandle (

	ControlRef	c)
	
	{ // begin GetControlPopupMenuHandle
		
		return (**((PopupPrivateDataHandle) (**c).contrlData)).mHandle;
	
	} // end GetControlPopupMenuHandle

// ---------------------------------------------------------------------------
//		¥ GetPortVisibleRegion
// ---------------------------------------------------------------------------

pascal RgnHandle
GetPortVisibleRegion (

	CGrafPtr               port,
	RgnHandle              visRgn)
	
	{ // begin GetPortVisibleRegion
		
		CopyRgn (port->visRgn, visRgn);
		
		return visRgn;
	
	} // end GetPortVisibleRegion

// ---------------------------------------------------------------------------
//		¥ GetListPort
// ---------------------------------------------------------------------------

pascal CGrafPtr
GetListPort (

	ListRef	inList)
	
	{ // begin GetListPort
		
		return (CGrafPtr) (**inList).port;
	
	} // end GetListPort
	
// ---------------------------------------------------------------------------
//		¥ AEGetDescDataSize
// ---------------------------------------------------------------------------

pascal Size
AEGetDescDataSize (

	const AEDesc *         theAEDesc)
	
	{ // begin AEGetDescDataSize
	
		return GetHandleSize (theAEDesc->dataHandle);
		
	} // end AEGetDescDataSize
	
// ---------------------------------------------------------------------------
//		¥ AEGetDescData
// ---------------------------------------------------------------------------

pascal OSErr
AEGetDescData (

	const AEDesc *         theAEDesc,
	void *                 dataPtr,
	Size                   maximumSize)
	
	{ // begin AEGetDescData
		
		Size	copySize = AEGetDescDataSize (theAEDesc);
		if (copySize > maximumSize) copySize = maximumSize;
		
		::BlockMoveData (*theAEDesc->dataHandle, dataPtr, copySize);
		
		return noErr;
		
	} // end AEGetDescData
	
#endif

// ---------------------------------------------------------------------------
//		¥ AppendPString
// ---------------------------------------------------------------------------

StringPtr
AppendPString (

	StringPtr			s,
	ConstStr255Param	c)
	
	{ // begin AppendPString
		
		BlockMoveData (c + 1, s + s[0] + 1, c[0]);
		s[0] += c[0];

		return s;
		
	} // end AppendPString
	
// ---------------------------------------------------------------------------
//		¥ ReplaceString
// ---------------------------------------------------------------------------

OSErr 
ReplaceString (

	Handle 				baseText, 
	ConstStr255Param	key, 
	ConstStr255Param	substitutionText)

	{ // begin ReplaceString

		OSErr	e = noErr;
		Handle 	h = nil;
		
		if (noErr != (e = PtrToHand(substitutionText + 1, &h, substitutionText[0]))) goto CleanUp;
		if (noErr > (e = ReplaceText (baseText, h, (StringPtr) key))) goto CleanUp;
		
		e = noErr;
		
	CleanUp:
	
		if (h) DisposeHandle (h);
		h = nil;
		
		return e;
		
	} // end ReplaceString

// ---------------------------------------------------------------------------
//		¥ ReplaceInIndString
// ---------------------------------------------------------------------------

OSErr 
ReplaceInIndString (

	StringPtr			stringOut, 
	short 				strListID, 
	short 				strIndex,
	ConstStr255Param	param1, 
	ConstStr255Param	param2,
	ConstStr255Param	param3, 
	ConstStr255Param	param4)
	
	{ // begin ReplaceInIndString
	
		OSErr e = noErr;

		Handle h = nil;
		
		GetIndString (stringOut, strListID, strIndex);
		if (noErr != (e = PtrToHand (stringOut + 1, &h, stringOut[0]))) goto CleanUp;
		
		if (param1 == nil) param1 = "\p";
		if (noErr > (e = ReplaceString (h, "\p<1>", param1))) goto CleanUp;

		if (param2 == nil) param2 = "\p";
		if (noErr > (e = ReplaceString (h, "\p<2>", param2))) goto CleanUp;

		if (param3 == nil) param3 = "\p";
		if (noErr > (e = ReplaceString (h, "\p<3>", param3))) goto CleanUp;

		if (param4 == nil) param4 = "\p";
		if (noErr > (e = ReplaceString (h, "\p<4>", param4))) goto CleanUp;

		BlockMoveData (*h, stringOut + 1, stringOut[0] = GetHandleSize(h));
		
	CleanUp:
	
		if (h) DisposeHandle (h);
		h = nil;
		
		return e;
	
	} // end ReplaceInIndString

// ---------------------------------------------------------------------------
//		¥ FSpEqual
// ---------------------------------------------------------------------------

Boolean
FSpEqual (

	const	FSSpec&	a,
	const	FSSpec&	b)
	
	{ // begin FSpEqual
		
		if (a.vRefNum != b.vRefNum) return false;
		if (a.parID != b.parID) return false;
		
		return ::EqualString (a.name, b.name, false, true);
		
	} // end FSpEqual
	
// ---------------------------------------------------------------------------
//		¥ CheckScanCode
// ---------------------------------------------------------------------------

Boolean 
CheckScanCode (

	short	inScan)

	{ // begin CheckScanCode
	
		KeyMapByteArray		keys;
		::GetKeys (*(KeyMap*) &keys);
		
		return ((keys[inScan >> 3] >> (inScan & 0x07)) & 0x01) ? true : false;
		
	} // end CheckScanCode
	
// ---------------------------------------------------------------------------
//		¥ VCSIdleProc
// ---------------------------------------------------------------------------

pascal Boolean 
VCSIdleProc (

	EventRecord*	theEvent, 
	long *			sleepTime, 
	RgnHandle*		mouseRgn)
	
	{ // begin VCSIdleProc
		
		if (theEvent->what == nullEvent) {
			*mouseRgn = nil;
			*sleepTime = 6;
			} // if
		
		return false;
	
	} // end VCSIdleProc

// ---------------------------------------------------------------------------
//		¥ VCSSendOutputCommand
// ---------------------------------------------------------------------------

OSErr 
VCSSendOutputCommand (

	VCSContext&				inPB,
	const	AEDescList*		command,
	const	FSSpec*			cwd,
	Handle*					output)
	
	{ // begin VCSSendOutputCommand
	
		OSErr				e = noErr;
		
		ProcessSerialNumber	psn;
				
		*output = nil;
		
		{
			// find MacCVS
			VCSTask 	task(inPB, kTaskStrings, kFindingClientTask);
			if (noErr != (e = CVSGetProcess (inPB, &psn))) return VCSRaiseOSErr (inPB, e);
			}
			
		// send the command to SourceServer
		switch (e = CVSSendCommand (inPB, output, &psn, command, cwd)) {
			case noErr:
			case userCanceledErr:
				break;
				
			default:
				//	Report any errors			
				VCSRaiseOSErr (inPB, e);
				break;
			} // switch
		
		return e;
		
	} // end VCSSendOutputCommand

// ---------------------------------------------------------------------------
//		¥ VCSSendCommand
// ---------------------------------------------------------------------------

OSErr 
VCSSendCommand (

	VCSContext&				inPB,
	const	AEDescList*		command,
	const	FSSpec*			cwd)
	
	{ // begin VCSSendCommand
	
		OSErr				e = noErr;
		
		//	Stuff to clean up
		StHandle			output;
		
		if (noErr != (e = VCSSendOutputCommand (inPB, command, cwd, &output.mH))) return e;
		
		if ((nil != output) && (0 != GetHandleSize (output))) {
			long				theCount;
			long				i;
			
			// Pull the command line from the event
			if (noErr != (e = AECountItems (command, &theCount))) return e;
			for (i = 0; i < theCount; ++i) {
				AEKeyword	key = typeNull;
				DescType	typeCode;
				Size		actualSize = 0;
				Str255		cmdName;
				
				if (noErr != (e = AEGetNthPtr (command, i + 1, typeChar, &key, &typeCode, cmdName + 1, sizeof (cmdName) - 1, &actualSize))) return e;
				cmdName[0] = actualSize;
				if (cmdName[1] == '-') continue;
				
				if (noErr != (e = VCSCheckCmdOutput (inPB, cmdName, output))) return e;
				break;
				} // for
			} // if
			
		return e;
		
	} // end VCSSendCommand

// ---------------------------------------------------------------------------
//		¥ LoadFrameworkBundle
// ---------------------------------------------------------------------------

OSStatus 
LoadFrameworkBundle (
	
	CFBundleRef&	outBundle,
	CFStringRef 	inFramework)

	{ // begin LoadFrameworkBundle
	
		OSStatus    e;

		FSRef       fwRef;
		CFURLRef    baseURL = nil;
		CFURLRef    bundleURL = nil;

		outBundle = nil;

		do {
			if (noErr != (e = ::FSFindFolder (kOnAppropriateDisk, kFrameworksFolderType, true, &fwRef))) break;

		 	baseURL = ::CFURLCreateFromFSRef (kCFAllocatorDefault, &fwRef);
		 	if (baseURL == nil) {
				e = coreFoundationUnknownErr;
				break;
				} // if
				
			bundleURL = ::CFURLCreateCopyAppendingPathComponent (kCFAllocatorDefault, baseURL, inFramework, false);
			if (bundleURL == nil) {
				e = coreFoundationUnknownErr;
				break;
				} // if

			outBundle = ::CFBundleCreate (kCFAllocatorDefault, bundleURL);
			if (outBundle == nil) {
				e = coreFoundationUnknownErr;
				break;
				} // if
			
			if (!::CFBundleLoadExecutable (outBundle)) {
				e = coreFoundationUnknownErr;
				break;
				} // if
			} while (false);
			
		if ((noErr != e) && (nil != outBundle)) {
			::CFRelease (outBundle);
			outBundle = nil;
			} // if
			
		if (bundleURL != nil) {
			::CFRelease(bundleURL);
			bundleURL = nil;
			} // if
			
		if (baseURL != nil) {
			::CFRelease(baseURL);
			baseURL = nil;
			} // if

		return e;
	
	} // end LoadFrameworkBundle

