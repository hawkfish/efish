#pragma once

/*
	File:		AboutBox.h

	Contains:	header for the about box
	
	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		09 Nov 2000		drd		Dropped gShowing
		08 Nov 2000		drd		Renamed dialog to AboutBox, subclass EDialog, and added gShowing
		18 Sep 2000		rmgw	Add registration.
		21 aug 2000		dml		Created
*/

#include "EDialog.h"
#include "PhotoAppCommandAttachment.h"

class AboutBox : public EDialog {
		LPane*				mRegister;
		
	public:
	
							AboutBox		(ResIDT			inDialogResID,
											 LCommander*	inSuper);
		virtual				~AboutBox		(void);
	
		bool				Run				(void);
};//end class PhotoAboutBox
	
	

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
	