/*
	File:		VCSCommand.cp

	Contains:	Interface to the generic command object.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1998 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    10/14/98    rmgw    first checked in.
*/


#include "VCSCommand.h"

// ---------------------------------------------------------------------------
//		€ VCSContext
// ---------------------------------------------------------------------------

VCSCommand::VCSCommand (

	VCSContext&	inContext,
	Boolean		inHasCommand,
	Boolean		inHasRecursive,
	Boolean		inHasAdvanced,
	Boolean		inHasAdvancedRecursive)
	
	: mContext (inContext)
	
	, mHasCommand (inHasCommand)
	, mHasRecursive (inHasRecursive)
	, mHasAdvanced (inHasAdvanced)
	, mHasAdvancedRecursive (inHasAdvancedRecursive)

	{ // begin VCSCommand
		
	} // end VCSCommand

// ---------------------------------------------------------------------------
//		€ ~VCSCommand
// ---------------------------------------------------------------------------

VCSCommand::~VCSCommand (void)
	
	{ // begin ~VCSCommand
				
	} // end ~VCSCommand

// ---------------------------------------------------------------------------
//		€ RequestIsSupported
// ---------------------------------------------------------------------------

CWVCSCommandStatus 
VCSCommand::RequestIsSupported (void)
	
	{ // begin RequestIsSupported
			
		eVCSCommandStatus	status = cwCommandStatusSucceeded;
		Boolean				supported = false;
		Boolean				recursive (mContext.Recursive ());
		Boolean				advanced (mContext.Advanced ());
		
		if (recursive)
			supported = advanced ? mHasAdvancedRecursive : mHasRecursive;
		else supported = advanced ? mHasAdvanced : mHasCommand;

		return supported ? cwCommandStatusSupported : cwCommandStatusUnsupported;

	} // end RequestIsSupported

// ---------------------------------------------------------------------------
//		€ DoRequest
// ---------------------------------------------------------------------------

CWVCSCommandStatus 
VCSCommand::DoRequest (void)
	
	{ // begin DoRequest
			
		return cwCommandStatusSucceeded;
	
	} // end DoRequest

// ---------------------------------------------------------------------------
//		€ OnRequest
// ---------------------------------------------------------------------------

CWVCSCommandStatus 
VCSCommand::OnRequest (void)
	
	{ // begin OnRequest
			
		if (mContext.CheckIfSupported ()) return RequestIsSupported ();
		
		return DoRequest ();
	
	} // end OnRequest

