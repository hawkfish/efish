/*
	File:		PrefsCommand.h

	Contains:	Definition of the Preferences dialog.
				PrefsCommand
				PrefsDialog

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		28 Jun 2000		drd		Created
*/

#pragma once

#include "PhotoAppCommandAttachment.h"
#include <UModalDialogs.h>

class PrefsCommand : public PhotoAppCommandAttachment
{
public:
						PrefsCommand				(const CommandT		inCommand,
													 PhotoPrintApp*		inApp);
	virtual 			~PrefsCommand				();

protected:
	// ECommandAttachment
	virtual	void		ExecuteCommand				(void*				inCommandData);
	virtual	void		FindCommandStatus			(SCommandStatus*	inStatus);
};

class PrefsDialog : public StDialogHandler
{
public:
	enum {
		PPob_Prefs = 1200
	};
						PrefsDialog					(LCommander*		inSuper);
						~PrefsDialog();
};
