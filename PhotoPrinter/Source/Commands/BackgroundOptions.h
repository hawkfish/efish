/*
	File:		BackgroundOptions.h

	Contains:	Definition of the Background Options dialog.
				BackgroundOptionsCommand
				BackgroundOptionsDialog

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		12 jul 2001		dml		148 Add ListenToMessage.  Move unit enum to PhotoUtility.h
		28 jun 2001		dml		26 add parm to EnableMarginFields for 3hol logic
		22 mar 2001		dml		add EnableMarginFields
		30 Jun 2000		drd		Descend from EDialog; added mDoc
		14 Jun 2000		drd		BackgroundOptionsDialog
		14 Jun 2000		drd		Created
*/

#ifndef BackgroundOptionsINC
#define BackgroundOptionsIND

#include "EDialog.h"
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

class BackgroundOptionsDialog : public EDialog
{
public:
						BackgroundOptionsDialog		(PhotoPrintDoc*		inSuper);
						~BackgroundOptionsDialog();

	virtual void	EnableMarginFields(bool inSides, bool inHoles);
	virtual void	ListenToMessage(MessageT	inMessage, void*		ioParam );

protected:
	PhotoPrintDoc*		mDoc;
};

#endif //BackgroundOptionsINC