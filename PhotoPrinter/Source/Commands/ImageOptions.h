/*
	File:		ImageOptions.h

	Contains:	Definition of the Image Options dialog.
				ImageOptionsCommand
				ImageOptionsDialog

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		24 aug 2000		dml		stop creating Items just to get rotated picts
		04 Aug 2000		drd		Removed Initialized, mInitialized
		07 Jul 2000		drd		Fewer panels; Commit
		06 Jul 2000		drd		Separate methods to initialize dialog panels
		05 Jul 2000		drd		Added mImage0, mImage90, mImage180, mImage270 to manage thumbnails
		29 Jun 2000		drd		ImageOptionsDialog uses EDialog, overrides ListenToMessage
		14 Jun 2000		drd		Created
*/

#pragma once

#include "PhotoDocCommandAttachment.h"
#include "EDialog.h"
#include "PhotoPrintItem.h"
#include "MNewPicture.h"

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

class	Layout;

class ImageOptionsDialog : public EDialog
{
public:
	enum {
		PPob_ImageOptions = 2000,

		panel_Image = 1,
		panel_Text,
		panel_Frame,
		panel_COUNT = panel_Frame
	};
						ImageOptionsDialog		(LCommander*		inSuper);
						~ImageOptionsDialog();

	// LListener
	virtual void		ListenToMessage			(MessageT		inMessage,
												 void*			ioParam);

			void		Commit();

protected:
			void		SetupFrame();
			void		SetupImage();
			void		SetupText();

	MNewPicture		mImage0;
	MNewPicture		mImage90;
	MNewPicture		mImage180;
	MNewPicture		mImage270;
};
