/*
	File:		PurchaseCommand.cp

	Contains:	Implementation of the Purchase command handler.
	
	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		14 Nov 2001		rmgw	Hacked from About Box.
*/

#include "PurchaseCommand.h"

#include "Registration.h"

#include "PhotoPrintApp.h"

// ---------------------------------------------------------------------------
//	¥	PurchaseCommand												  [public]
// ---------------------------------------------------------------------------

PurchaseCommand::PurchaseCommand (

	const CommandT inCommand,
	PhotoPrintApp* inApp)
	
	: PhotoAppCommandAttachment(inCommand, inApp)

	{ // begin PurchaseCommand

	} // end PurchaseCommand

// ---------------------------------------------------------------------------
//	¥	~PurchaseCommand											  [public]
// ---------------------------------------------------------------------------

PurchaseCommand::~PurchaseCommand()

	{ // begin ~PurchaseCommand

	} // end ~PurchaseCommand
	
// ---------------------------------------------------------------------------
//	¥	FindCommandStatus											  [public]
// ---------------------------------------------------------------------------

void
PurchaseCommand::FindCommandStatus (

	SCommandStatus*	inStatus)

	{ // begin FindCommandStatus

		*(inStatus->enabled) = !Registration::IsRegistered ();

	} // end FindCommandStatus
	
// ---------------------------------------------------------------------------
//	¥	ExecuteCommand												  [public]
// ---------------------------------------------------------------------------

void
PurchaseCommand::ExecuteCommand (

	void*			/*inCommandData*/)
	
	{ // begin ExecuteCommand
		
		Registration::DoPurchaseDialog (mApp);

	} // end ExecuteCommand
