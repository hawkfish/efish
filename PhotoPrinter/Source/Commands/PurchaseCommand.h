/*
	File:		PurchaseCommand.h

	Contains:	Interface to the Purchase command handler.
	
	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		14 Nov 2001		rmgw	Hacked from About Box.
*/

#pragma once

#include "PhotoAppCommandAttachment.h"

class PurchaseCommand : public PhotoAppCommandAttachment
{
public:
						PurchaseCommand		(const CommandT inCommand, 
											 PhotoPrintApp* inApp);
						~PurchaseCommand();

	// ECommandAttachment
	virtual	void		ExecuteCommand		(void*				inCommandData);
	virtual	void		FindCommandStatus	(SCommandStatus*	inStatus);
};
	