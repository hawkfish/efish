/*
	File:		ImageOptions.h

	Contains:	Definition of the Image Options dialog.
				ImageOptionsCommand
				ImageOptionsDialog

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		29 Jun 2000		drd		ImageOptionsDialog uses EDialog, overrides ListenToMessage
		14 Jun 2000		drd		Created
*/

#include "PhotoDocCommandAttachment.h"
#include "EDialog.h"

class ImageOptionsCommand : public PhotoDocCommandAttachment
{
public:
						ImageOptionsCommand		(const CommandT		inCommand,
													 PhotoPrintDoc*		inDoc);
	virtual 			~ImageOptionsCommand	();

protected:
	// ECommandAttachment
	virtual	void		ExecuteCommand			(void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
};

class ImageOptionsDialog : public EDialog
{
public:
	enum {
		PPob_ImageOptions = 2000
	};
						ImageOptionsDialog		(LCommander*		inSuper);
						~ImageOptionsDialog();

	// LListener
	virtual void		ListenToMessage			(MessageT		inMessage,
												 void*			ioParam);
};
