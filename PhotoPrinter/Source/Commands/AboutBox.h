#pragma once

/*
	File:		AboutBox.h

	Contains:	header for the about box
	
	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		18 Sep 2000		rmgw	Add registration.
		21 aug 2000		dml		Created
*/


#include "UModalDialogs.h"
#include "PhotoAppCommandAttachment.h"

class PhotoAboutBox : public StDialogHandler {
		
		LPane*				mRegister;
		
	public:
	
							PhotoAboutBox	(ResIDT			inDialogResID,
											 LCommander*		inSuper);

		virtual				~PhotoAboutBox	(void);

	
		bool				Run				(void);

	};//end class PhotoAboutBox
	
	
	

class AboutCommand : public PhotoAppCommandAttachment
{
public:
						AboutCommand(const CommandT inCommand, 
									PhotoPrintApp* inApp);
						~AboutCommand();

	// ECommandAttachment
	virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
												 void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};
	