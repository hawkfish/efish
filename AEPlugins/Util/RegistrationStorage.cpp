/*
	File:		RegistrationStorage.cp

	Contains:	Implementation of a Serial Number RegistrationFile file.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    11/6/01		rmgw    Created from old ERegistrationFile.
*/

#include "RegistrationStorage.h"

#include "adm_suites.h"

//	===	Class Variables ===

const char
RegistrationStorage::sTrialVersion [] = "Last Version";

const char
RegistrationStorage::sTrialDate [] = "Diagonal Size";

const char
RegistrationStorage::sRegKey [] = "Default Title";

const char
RegistrationStorage::sHostKey [] = "Host";

// ---------------------------------------------------------------------------
//		¥ RegistrationStorage
// ---------------------------------------------------------------------------

RegistrationStorage::RegistrationStorage (

	const	std::string&	inSection,
	const	ASInt32&		inCurrentVersion)
	
	: mPersist (inSection)
	, mCurrentVersion (inCurrentVersion)
	
	{ // begin RegistrationStorage
		
	} // end RegistrationStorage
	
// ---------------------------------------------------------------------------
//		¥ IsCorrectHost
// ---------------------------------------------------------------------------

bool
RegistrationStorage::IsCorrectHost (void) const
	
	{ // begin IsCorrectHost
	
		//	Get the info
		PF_AppPersonalTextInfo	info;
		PFAppSuite2*			app = (PFAppSuite2*) ADM::Suites::Acquire (kPFAppSuite, kPFAppSuiteVersion2);
			app->PF_GetPersonalInfo (&info);
		ADM::Suites::Release (kPFAppSuite, kPFAppSuiteVersion2);
		
		//	Get our version
		char		serial_str[PF_APP_MAX_PERS_LEN + 1];
		mPersist.GetString (sHostKey, sizeof (serial_str), serial_str, info.serial_str);
		
		return (0 == strcmp (serial_str, info.serial_str));
		
	} // end IsCorrectHost
	
// ---------------------------------------------------------------------------
//		¥ DeleteHost
// ---------------------------------------------------------------------------

void
RegistrationStorage::DeleteHost (void) 
	
	{ // begin DeleteHost
	
		mPersist.DeleteEntry (sHostKey);
		
	} // end DeleteHost
	
// ---------------------------------------------------------------------------
//		¥ GetTrialVersion
// ---------------------------------------------------------------------------

ASInt32
RegistrationStorage::GetTrialVersion (void) const
	
	{ // begin GetTrialVersion
	
		//	Get the version
		ASInt32				stampVersion;
		mPersist.GetLong (sTrialVersion, stampVersion, mCurrentVersion);
		
		return stampVersion;
		
	} // end GetTrialVersion
	
// ---------------------------------------------------------------------------
//		¥ SetTrialVersion
// ---------------------------------------------------------------------------

void
RegistrationStorage::SetTrialVersion (

	ASInt32	inTrialVersion)
	
	{ // begin SetTrialVersion
		
		mPersist.SetLong (sTrialVersion, inTrialVersion);
			
	} // end SetTrialVersion
	
// ---------------------------------------------------------------------------
//		¥ DeleteTrialVersion
// ---------------------------------------------------------------------------

void
RegistrationStorage::DeleteTrialVersion (void)
	
	{ // begin DeleteTrialVersion
		
		mPersist.DeleteEntry (sTrialVersion);
			
	} // end DeleteTrialVersion
	
// ---------------------------------------------------------------------------
//		¥ GetRegTime
// ---------------------------------------------------------------------------

time_t
RegistrationStorage::GetRegTime (void) const
	
	{ // begin GetRegTime
	
		//	Get the time stamp
		time_t				nowSecs = time (0);
		time_t				stampSecs;
		mPersist.GetLong (sTrialDate, stampSecs, nowSecs);
		
		//	Don't let them muck with the clock
		if (stampSecs > nowSecs) stampSecs = nowSecs;
		
		return stampSecs;
		
	} // end GetRegTime
	
// ---------------------------------------------------------------------------
//		¥ SetRegTime
// ---------------------------------------------------------------------------

void
RegistrationStorage::SetRegTime (

	time_t	inRegTime)
	
	{ // begin SetRegTime
		
		mPersist.SetLong (sTrialDate, inRegTime);
			
	} // end SetRegTime
	
// ---------------------------------------------------------------------------
//		¥ DeleteRegTime
// ---------------------------------------------------------------------------

void
RegistrationStorage::DeleteRegTime (void)
	
	{ // begin DeleteRegTime
		
		mPersist.DeleteEntry (sTrialDate);
			
	} // end DeleteRegTime
	
// ---------------------------------------------------------------------------
//		¥ GetRegString
// ---------------------------------------------------------------------------

bool
RegistrationStorage::GetRegString (

	A_u_long				inBufSize,
	char*					outValue) const
	
	{ // begin GetRegString
		
		//	Throw if it is missing
		if (!mPersist.DoesKeyExist (sRegKey)) return false;
		
		//	Return it
		mPersist.GetString (sRegKey, inBufSize, outValue);
		
		return true;
		
	} // end GetRegString
	
// ---------------------------------------------------------------------------
//		¥ SetRegString
// ---------------------------------------------------------------------------

void
RegistrationStorage::SetRegString (

	const	std::string&	inRegString)
	
	{ // begin SetRegString
		
		mPersist.SetString (sRegKey, inRegString.c_str ());
		
	} // end SetRegString
	
// ---------------------------------------------------------------------------
//		¥ DeleteRegString
// ---------------------------------------------------------------------------

void
RegistrationStorage::DeleteRegString (void)
	
	{ // begin DeleteRegString
		
		mPersist.DeleteEntry (sRegKey);
			
	} // end DeleteRegString
