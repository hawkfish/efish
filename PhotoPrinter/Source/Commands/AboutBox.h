/*
	File:		AboutBox.h

	Contains:	Interface to the About command handler.
	
	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		27 Jul 2001		rmgw	Don't expose dialog.
		09 Nov 2000		drd		Dropped gShowing
		08 Nov 2000		drd		Renamed dialog to AboutBox, subclass EDialog, and added gShowing
		18 Sep 2000		rmgw	Add registration.
		21 aug 2000		dml		Created
*/

#pragma once

#include "PhotoAppCommandAttachment.h"

class AboutCommand : public PhotoAppCommandAttachment
{
public:
						AboutCommand(const CommandT inCommand, 
									PhotoPrintApp* inApp);
						~AboutCommand();

	// ECommandAttachment
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};
	