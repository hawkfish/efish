/*
	File:		FileEditText.h

	Contains:	An adornment class which is used for nametag on-screen functions

	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		15 Aug 2001		drd		309 Override ObeyCommand
		24 Jul 2001		rmgw	No longer an LListener. Bug #219.
		24 Jul 2001		rmgw	Badges need to know about the document. Bug #202.
		18 Jul 2001		drd		194 Now an LListener
		26 feb 2001		dml		add BeTarget
		26 feb 2001		dml		AllowDontBeTarget replaces DontBeTarget.
		23 feb 2001		dml		created
*/

#pragma once

#include <LEditText.h>

#include "MFileSpec.h"
#include "PhotoPrintItem.h"

class PhotoPrintDoc;

class FileEditText 	: public LEditText			
{
	typedef LEditText	inherited;

protected:
	PhotoPrintDoc*	mDoc;
	PhotoItemRef	mItem;
	
public:

	enum {
		class_ID = FOUR_CHAR_CODE('Ftxt')
	};

						FileEditText(
								LStream*		inStream,
								ClassIDT		inImpID = imp_class_ID);

						FileEditText(
								const SPaneInfo&	inPaneInfo,
								LCommander*			inSuperCommander,
								ConstStringPtr		inInitialText,
								ResIDT				inTextTraitsID,
								MessageT			inMessage,
								SInt16				inMaxChars,
								UInt8				inAttributes,
								TEKeyFilterFunc		inKeyFilter,
								bool				inPasswordField = false,
								ClassIDT			inImpID = imp_class_ID);

	virtual				~FileEditText();

	virtual Boolean		AllowDontBeTarget(LCommander* inNewTarget);
	virtual	void		BeTarget();
	virtual Boolean		ObeyCommand(
								CommandT			inCommand,
								void*				ioParam);

	virtual Boolean		HandleKeyPress(const EventRecord&	inKeyEvent);
	virtual void		SetItem(PhotoPrintDoc*	inDoc, PhotoItemRef inItem);
	virtual bool		TryRename(void);
	};//end FileEditText
	