#include "VCSPrefs.h"

#include "VCSContext.h"

#include <string.h>

#include <TextUtils.h>

//	=== Constants ===

enum {
	kIllegalPanel = 0,
	
	kEnvironmentPanel,
	
	kPanelSTRN = 16000
	};

static const	unsigned	char
sCVSLogNameKey[] = "\pLOGNAME";

static const	unsigned	char
sCVSUserKey[] = "\pUSER";

static const	unsigned	char
sCVSPassKey[] = "\pCVS_PASSWORD";


// ---------------------------------------------------------------------------
//		¥ GetCommentPrefs
// ---------------------------------------------------------------------------

static CWMemHandle
GetEnvPrefs (

	const VCSContext&	inPB)
	
	{ // begin GetEnvPrefs
		
		Str255		panelName;
		GetIndString (panelName, kPanelSTRN, kEnvironmentPanel);
		
		return inPB.GetNamedPreferences (p2cstr (panelName));
		
	} // end GetEnvPrefs

// ---------------------------------------------------------------------------
//		¥ VCSGetUserName
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
//		¥ VCSPrefsGetEnv
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
//		¥ VCSPrefsMakeEnvDescList
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
				
			//	LOGNAME = inLogin
			if (noErr != (e = AEPutPtr (outList, 0, typeChar, sCVSLogNameKey + 1, sCVSLogNameKey[0]))) goto CleanUp;
			if (noErr != (e = AEPutPtr (outList, 0, typeChar, db.pUsername, strlen (db.pUsername)))) goto CleanUp;

			//	USER = inLogin
			if (noErr != (e = AEPutPtr (outList, 0, typeChar, sCVSUserKey + 1, sCVSUserKey[0]))) goto CleanUp;
			if (noErr != (e = AEPutPtr (outList, 0, typeChar, db.pUsername, strlen (db.pUsername)))) goto CleanUp;

			//	CVS_PASSWORD = inPB->pPassword
			if (noErr != (e = AEPutPtr (outList, 0, typeChar, sCVSPassKey + 1, sCVSPassKey[0]))) goto CleanUp;
			if (noErr != (e = AEPutPtr (outList, 0, typeChar, db.pPassword, strlen (db.pPassword)))) goto CleanUp;
			
			//	Other variables from the list
			try {
				CWMemHandle			h = GetEnvPrefs (inPB);
				EnvironmentListPtr	p = (EnvironmentListPtr) inPB.LockMemHandle (h);
				e = EnvListToAEDescList (p, outList);
				inPB.UnlockMemHandle (h);
				} // try
				
			catch (...) {
				} // catch
			if (noErr != (e)) goto CleanUp;
			
		CleanUp:
		
			return e;
			
	} // end VCSPrefsMakeEnvDescList
