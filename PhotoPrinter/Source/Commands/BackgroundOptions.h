/*
	File:		BackgroundOptions.h

	Contains:	Definition of the Background Options dialog.
				BackgroundOptionsCommand
				BackgroundOptionsDialog

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Jun 2000		drd		Created
*/

#include "PhotoDocCommandAttachment.h"

class BackgroundOptionsCommand : public PhotoDocCommandAttachment
{
public:
						BackgroundOptionsCommand	(const CommandT		inCommand,
													 PhotoPrintDoc*		inDoc);
	virtual 			~BackgroundOptionsCommand	();

protected:
	// ECommandAttachment
	virtual	void		ExecuteCommand				(void*				inCommandData);
	virtual	void		FindCommandStatus			(SCommandStatus*	inStatus);
};
