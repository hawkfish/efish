/*
	File:		FileEditText.h

	Contains:	An adornment class which is used for nametag on-screen functions

	Written by:	Dav Lion
	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	26 feb 2001		dml			AllowDontBeTarget replaces DontBeTarget.
	23 feb 2001		dml			created

*/
#pragma once

#include <LEditText.h>
#include "MFileSpec.h"
#include "PhotoPrintItem.h"

class FileEditText : public LEditText {
protected:
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
	virtual Boolean		HandleKeyPress(const EventRecord&	inKeyEvent);
	virtual void		SetItem(PhotoItemRef inItem);
	virtual bool		TryRename(void);
	};//end FileEditText
	