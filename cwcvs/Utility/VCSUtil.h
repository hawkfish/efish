// ===========================================================================
//	VCSUtil.h ©1997-2001 Electric Fish, Inc. All Rights Reserved.
// ===========================================================================

#pragma once

#include "VCSContext.h"

#include <AEDataModel.h>
#include <CFBundle.h>

enum ScanCodes {
	kCmdScan = 0x37,
	kShiftScan = 0x38,
	kAlphaScan = 0x39,
	kOptionScan = 0x3A,
	kControlScan = 0x3B,
	
	kFnordScan
	};

#if !CALL_NOT_IN_CARBON
#define P2CStr p2cstr
#define C2PStr c2pstr

char*
p2cstr (StringPtr	src);

StringPtr
c2pstr (char *	src);
#endif

#if !ACCESSOR_CALLS_ARE_FUNCTIONS
#include <Menus.h>
#include <Controls.h>
#include <Lists.h>
#include <Scrap.h>

/* GetWindowFromPort is needed to ‘cast up’ to a WindowRef from a GrafPort */
inline pascal WindowRef
GetWindowFromPort           (CGrafPtr               port)
	{return (WindowRef) port;};

pascal MenuHandle
GetControlPopupMenuHandle	(ControlRef	c);

pascal CGrafPtr
GetListPort					(ListRef                list);

pascal RgnHandle
GetPortVisibleRegion 		(CGrafPtr				port,
							 RgnHandle				visRgn);
						
pascal OSErr
AEGetDescData               (const AEDesc *         theAEDesc,
							 void *                 dataPtr,
							 Size                   maximumSize);

pascal Size
AEGetDescDataSize			(const AEDesc *         theAEDesc);
#endif

StringPtr
AppendPString (

	StringPtr			s,
	ConstStr255Param	c);

OSErr 
ReplaceString (

	Handle baseText, 
	ConstStr255Param key, 
	ConstStr255Param substitutionText);

OSErr 
ReplaceInIndString (

	StringPtr 				stringOut, 
	short 					strListID, 
	short 					strIndex,
	ConstStr255Param 		param1 = nil, 
	ConstStr255Param 		param2 = nil,
	ConstStr255Param 		param3 = nil, 
	ConstStr255Param 		param4 = nil);

Boolean
FSpEqual (

	const	FSSpec&			a,
	const	FSSpec&			b);
	
Boolean 
CheckScanCode (

	short					inScan);
	
pascal Boolean 
VCSIdleProc		(EventRecord*			theEvent, 
				 long*					sleepTime, 
				 RgnHandle*				mouseRgn);
	
OSErr 
VCSSendOutputCommand (

	VCSContext&				inPB,
	const	AEDescList*		command,
	const	FSSpec* 		cwd,
	Handle*					output);

OSErr 
VCSSendCommand	(

	VCSContext&				inPB,
	const	AEDescList*		command,
	const	FSSpec* 		cwd);

OSStatus 
LoadFrameworkBundle (
	
	CFBundleRef&			outBundle,
	CFStringRef 			inFramework);
