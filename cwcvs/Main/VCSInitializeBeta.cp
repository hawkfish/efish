#include "VCSInitialize.h"

#include <Dialogs.h>

//	=== Constants ===

// ---------------------------------------------------------------------------
//		¥ VCSInitialize
// ---------------------------------------------------------------------------

CWVCSCommandStatus 
VCSInitialize (
	
	const	VCSContext&	)

	{ // begin VCSInitialize
		
		typedef	unsigned	long	t_secs;
		
		CWVCSCommandStatus			result = cwCommandStatusFailed;
		
		t_secs						nowSecs;
		t_secs						expireSecs;
		DateTimeRec					expire;
		
		//	Check the times
		::GetDateTime (&nowSecs);

		expire.year = 1999;
		expire.month = 4;
		expire.day = 1;
		expire.hour = 0;
		expire.minute = 0;
		expire.second = 0;
		::DateToSeconds (&expire, &expireSecs);
		
		if (nowSecs >= expireSecs) {
			::StopAlert (16100, nil);
			
			goto CleanUp;
			} // if
			
		result = cwCommandStatusSucceeded;

	CleanUp:
		
		return result;
		
	} // end VCSInitialize
