#include "VCSDatabaseConnect.h"

#include "VCSTask.h"
#include "CVSCommand.h"

//	=== Constants ===

// ---------------------------------------------------------------------------
//		¥ VCSDatabaseConnect
// ---------------------------------------------------------------------------

VCSDatabaseConnect::VCSDatabaseConnect (

	VCSContext&	inContext)
	
	: VCSCommand (inContext, true)
	
	{ // begin VCSDatabaseConnect
		
	} // end VCSDatabaseConnect

// ---------------------------------------------------------------------------
//		¥ ~VCSDatabaseConnect
// ---------------------------------------------------------------------------

VCSDatabaseConnect::~VCSDatabaseConnect (void)
	
	{ // begin ~VCSDatabaseConnect
				
	} // end ~VCSDatabaseConnect

// ---------------------------------------------------------------------------
//		¥ DoRequest
// ---------------------------------------------------------------------------

CWVCSCommandStatus 
VCSDatabaseConnect::DoRequest (void)
	
	{ // begin DoRequest
					
		CWVCSCommandStatus		result = cwCommandStatusFailed;
		
		//	Prepare
		CWVCSDatabaseConnection	db;
		mContext.GetDatabase (db);
		
		VCSTask (mContext, kTaskStrings, kConnectTask, db.sDatabasePath.name);
		
		//	Launch the app
		ProcessSerialNumber	psn;
		if (noErr != CVSGetProcess (&psn)) goto CleanUp;

		result = cwCommandStatusSucceeded;

	CleanUp:
		
		return result;
		
	} // end DoRequest

