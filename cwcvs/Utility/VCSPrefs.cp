#include "VCSPrefs.h"

#include "VCSContext.h"
#include "CVSCommand.h"

#include <string.h>

#include <Errors.h>
#include <TextUtils.h>

//	=== Constants ===

enum {
	kIllegalPanel = 0,
	
	kEnvironmentPanel,
	kOptionsPanel,
	kCommentsPanel,
	
	kPanelStrings = 16000
	};

static const	unsigned	char
sCVSLogNameKey[] = "\pLOGNAME";

static const	unsigned	char
sCVSUserKey[] = "\pUSER";

static const	unsigned	char
sCVSPassKey[] = "\pCVS_PASSWORD";

static const	unsigned	char
sCVSDirtyProjectKey[] = "\pDIRTY_MCP";

static const	unsigned	char
sCVSYesValue[] = "\pyes";

static const	unsigned	char
sCVSNoValue[] = "\pno";

// ---------------------------------------------------------------------------
//		€ GetEnvPrefs
// ---------------------------------------------------------------------------

static CWMemHandle
GetEnvPrefs (

	const VCSContext&	inPB)
	
	{ // begin GetEnvPrefs
		
		Str255		panelName;
		GetIndString (panelName, kPanelStrings, kEnvironmentPanel);
		
		return inPB.GetNamedPreferences (p2cstr (panelName));
		
	} // end GetEnvPrefs

// ---------------------------------------------------------------------------
//		€ VCSGetUserName
// ---------------------------------------------------------------------------

void 
VCSGetUserName (
	
	const VCSContext&		inPB,
	Str63					userName)

	{ // begin VCSGetUserName
		
		CWVCSDatabaseConnection	db;
		inPB.GetDatabase (db);

		::BlockMoveData (db.pUsername, userName + 1, userName[0] = ::strlen (db.pUsername));
		
	} // end VCSGetUserName

// ---------------------------------------------------------------------------
//		€ VCSPrefsGetEnv
// ---------------------------------------------------------------------------

Boolean 
VCSPrefsGetEnv (
	
	const VCSContext&		inPB,
	ConstStr255Param		inKey,
	Str255					outValue,
	ConstStr255Param		inDefault)

	{ // begin VCSPrefsGetEnv
		
		Boolean	result = false;
			
		try {
			CWMemHandle			h = GetEnvPrefs (inPB);
			EnvironmentListPtr	p = (EnvironmentListPtr) inPB.LockMemHandle (h);
			result = EnvListGet (p, inKey, outValue, inDefault);
			inPB.UnlockMemHandle (h);
			} // try
			
		catch (...) {
			} // catch
		
		return result;
		
	} // end VCSPrefsGetEnv
	
// ---------------------------------------------------------------------------
//		€ VCSPrefsMakeEnvDescList
// ---------------------------------------------------------------------------

OSErr 
VCSPrefsMakeEnvDescList (
	
	const VCSContext&		inPB,
	AEDescList*				outList)

	{ // begin VCSPrefsMakeEnvDescList
		
		OSErr					e = noErr;
		
		//	Get the username
		CWVCSDatabaseConnection	db;
		inPB.GetDatabase (db);
		
		//	Build the environment list
		if (noErr != (e = AECreateList (nil, 0, false, outList))) goto CleanUp;
				
			//	Other variables from the list
			//	Put these first so they will override the ones we add.
			try {
				CWMemHandle			h = GetEnvPrefs (inPB);
				EnvironmentListPtr	p = (EnvironmentListPtr) inPB.LockMemHandle (h);
				e = EnvListToAEDescList (p, outList);
				inPB.UnlockMemHandle (h);
				} // try
				
			catch (...) {
				} // catch
			if (noErr != (e)) goto CleanUp;
			
			//	LOGNAME = inLogin
			if (noErr != (e = AEPutPtr (outList, 0, typeChar, sCVSLogNameKey + 1, sCVSLogNameKey[0]))) goto CleanUp;
			if (noErr != (e = AEPutPtr (outList, 0, typeChar, db.pUsername, strlen (db.pUsername)))) goto CleanUp;

			//	USER = inLogin
			if (noErr != (e = AEPutPtr (outList, 0, typeChar, sCVSUserKey + 1, sCVSUserKey[0]))) goto CleanUp;
			if (noErr != (e = AEPutPtr (outList, 0, typeChar, db.pUsername, strlen (db.pUsername)))) goto CleanUp;

			//	DIRTY_MCP = inLogin
			if (noErr != (e = AEPutPtr (outList, 0, typeChar, sCVSDirtyProjectKey + 1, sCVSDirtyProjectKey[0]))) goto CleanUp;
			ConstStr255Param	dirtyValue = sCVSNoValue;
			CWIDEInfo			info;
			if ((cwNoErr == inPB.GetIDEVersion (info)) && (info.majorVersion == 2))
				dirtyValue = sCVSYesValue;
			if (noErr != (e = AEPutPtr (outList, 0, typeChar, dirtyValue + 1, dirtyValue[0]))) goto CleanUp;
				
		CleanUp:
		
			return e;
			
	} // end VCSPrefsMakeEnvDescList

#pragma mark -

// ---------------------------------------------------------------------------
//		€ VCSOptionsPrefs
// ---------------------------------------------------------------------------

VCSOptionsPrefs::VCSOptionsPrefs (

	const VCSContext&	inContext)
	
	: mContext (inContext)
	
	{ // begin VCSOptionsPrefs
		
		Str255		panelName;
		::GetIndString (panelName, kPanelStrings, kOptionsPanel);
		mHandle = mContext.GetNamedPreferences (p2cstr (panelName));
		
		mOptions = (OptionsRec*) mContext.LockMemHandle (mHandle);
		
	} // end VCSOptionsPrefs

// ---------------------------------------------------------------------------
//		€ ~VCSOptionsPrefs
// ---------------------------------------------------------------------------

VCSOptionsPrefs::~VCSOptionsPrefs (void)
		
	{ // begin ~VCSOptionsPrefs
		
		mOptions = 0;
		mContext.UnlockMemHandle (mHandle);
		
	} // end ~VCSOptionsPrefs

// ---------------------------------------------------------------------------
//		€ VCSGetDefaultOptions
// ---------------------------------------------------------------------------

UInt32 
VCSGetDefaultOptions (
	
	const VCSContext&			inPB,
	OptionsRec::OptionsIndex	inIndex)

	{ // begin VCSGetDefaultOptions
		
		try {
			VCSOptionsPrefs	(inPB)->options[inIndex];
			} // try
			
		catch (...) {
			} // catch
	
		return 0;
		
	} // end VCSGetDefaultOptions

// ---------------------------------------------------------------------------
//		€ VCSGetDefaultOptionsList
// ---------------------------------------------------------------------------

OSErr 
VCSGetDefaultOptionsList (
	
	const VCSContext&			inPB,
	ResID						inStrnID,
	ResID						inMapID,
	AEDescList&					outList)

	{ // begin VCSGetDefaultOptionsList
		
		OSErr				e = noErr;
		VCSOptionsPrefs		settings (inPB);

		OptionBitMapRec**	map = (OptionBitMapRec**) ::GetResource ('OBMp', inMapID);
		if ((map == nil) || (*map == nil)) {
			e = ::ResError ();
			if (noErr == e) e = resNotFound;
			goto CleanUp;
			} // if

		for (UInt16	i = 0; i < (**map).count; ++i) {
			UInt32	value = ((settings->options[(**map).bit[i].index] >> (**map).bit[i].shift) & 0x01);
			if (!value) continue;
			
			//	Bit was set
			Str255	option;
			::GetIndString (option, inStrnID, (**map).bit[i].key);
			if (!option[0]) continue;
			
			if (noErr != (e = ::CVSAddPStringArg (&outList, option))) goto CleanUp;
			} // for
	
	CleanUp:
	
		return e;
		
	} // end VCSGetDefaultOptionsList

// ---------------------------------------------------------------------------
//		€ VCSGetTextKeywordDefault
// ---------------------------------------------------------------------------

UInt16 
VCSGetTextKeywordDefault (
	
	const VCSContext&			inPB)

	{ // begin VCSGetTextKeywordDefault
		
		try {
			return VCSOptionsPrefs (inPB)->textKeyword;
			} // try
			
		catch (...) {
			} // catch
	
		return 0;
		
	} // end VCSGetTextKeywordDefault

// ---------------------------------------------------------------------------
//		€ VCSGetBinaryKeywordDefault
// ---------------------------------------------------------------------------

UInt16 
VCSGetBinaryKeywordDefault (
	
	const VCSContext&			inPB)

	{ // begin VCSGetBinaryKeywordDefault
		
		try {
			return VCSOptionsPrefs (inPB)->binaryKeyword;
			} // try
			
		catch (...) {
			} // catch
	
		return 5;
		
	} // end VCSGetBinaryKeywordDefault

// ---------------------------------------------------------------------------
//		€ VCSGetHistoryInfoDefault
// ---------------------------------------------------------------------------

UInt16 
VCSGetHistoryInfoDefault (
	
	const VCSContext&			inPB)

	{ // begin VCSGetHistoryInfoDefault
		
		try {
			return VCSOptionsPrefs (inPB)->historyInfo;
			} // try
			
		catch (...) {
			} // catch
	
		return 0;
		
	} // end VCSGetHistoryInfoDefault

// ---------------------------------------------------------------------------
//		€ VCSGetClientCreator
// ---------------------------------------------------------------------------

OSType 
VCSGetClientCreator (
	
	const VCSContext&			inPB)

	{ // begin VCSGetClientCreator
		
		try {
			return VCSOptionsPrefs (inPB)->clientCreator;
			} // try
			
		catch (...) {
			} // catch
	
		return 'mCVS';
		
	} // end VCSGetClientCreator

#pragma mark -

// ---------------------------------------------------------------------------
//		€ GetCommentPrefs
// ---------------------------------------------------------------------------

static CWMemHandle
GetCommentPrefs (

	const VCSContext&	inPB)
	
	{ // begin GetCommentPrefs
		
		Str255		panelName;
		::GetIndString (panelName, kPanelStrings, kCommentsPanel);
		
		return inPB.GetNamedPreferences (p2cstr (panelName));
		
	} // end GetCommentPrefs

// ---------------------------------------------------------------------------
//		€ VCSGetFinderLabel
// ---------------------------------------------------------------------------

Boolean 
VCSGetFinderLabel (
	
	const VCSContext&	inPB,
	FinderLabelIndex	inIndex,
	short*				outLabel)

	{ // begin VCSGetFinderLabel
		
		Boolean	result = false;
		
		try {
			CWMemHandle		h = GetCommentPrefs (inPB);
			VCSCommentPref*	p = (VCSCommentPref*) inPB.LockMemHandle (h);
			result = p->useFinderLabels;
			*outLabel = p->finderLabels[inIndex];
			inPB.UnlockMemHandle (h);
			} // try
			
		catch (...) {
			} // catch
			
		return result;

	} // end VCSGetFinderLabel

