#include "VCSDatabaseVariables.h"

#include "VCSError.h"
#include "VCSTask.h"
#include "VCSUtil.h"

#include "CVSCommand.h"

#include <TextUtils.h>

// ---------------------------------------------------------------------------
//		€ VCSDatabaseVariables
// ---------------------------------------------------------------------------

VCSDatabaseVariables::VCSDatabaseVariables (

	VCSContext&	inContext)
	
	: VCSCommand (inContext, true)
	
	{ // begin VCSDatabaseVariables
		
	} // end VCSDatabaseVariables

// ---------------------------------------------------------------------------
//		€ ~VCSDatabaseVariables
// ---------------------------------------------------------------------------

VCSDatabaseVariables::~VCSDatabaseVariables (void)
	
	{ // begin ~VCSDatabaseVariables
				
	} // end ~VCSDatabaseVariables

// ---------------------------------------------------------------------------
//		€ DoRequest
// ---------------------------------------------------------------------------

CWVCSCommandStatus 
VCSDatabaseVariables::DoRequest (void)
	
	{ // begin DoRequest
					
		CWVCSCommandStatus	result = cwCommandStatusFailed;
		
		OSErr				e = noErr;
		Str255				title;
		
		//	Stuff to clean up
		AEDesc 				command = {typeNull, nil};
		Handle				output = nil;
		
		//	Prepare
		CWVCSDatabaseConnection	db;
		mContext.GetDatabase (db);
		VCSTask (mContext, kTaskStrings, kVariablesTask, db.sProjectRoot.name);

		/* Get the project info
		 * status
		 */
		if (noErr != VCSRaiseOSErr (mContext, CVSCreateCommand (&command, "status"))) goto CleanUp;
					
		// send the command to SourceServer
		if (noErr != (e = VCSSendOutputCommand (mContext, &command, &db.sProjectRoot, &output))) goto CleanUp;
			
		//	Display the results
		title[0] = 0;
		AppendPString (AppendPString (title, db.sProjectRoot.name), "\p.variables");
		
		mContext.CreateDocument (p2cstr (title), output);
		result = cwCommandStatusSucceeded;
		
	CleanUp:
	
		if (output != nil) DisposeHandle (output);
		output  = nil;

		AEDisposeDesc (&command);
				
		return result;
	
	} // end DoRequest

