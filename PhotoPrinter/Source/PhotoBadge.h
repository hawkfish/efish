/*
	File:		PhotoBadge.h

	Contains:	An adornment class which is used for nametag on-screen functions

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Badges need to know about the document. Bug #202.
		23 feb 2001		dml		created

*/
#pragma once

#include <LView.h>

#include "FileEditText.h"
#include "PhotoPrintItem.h"

#include "MFileSpec.h"
#include "MPString.h"

#include <UKeyFilters.h>

class PhotoBadge : public LView {
protected:
	
	FileEditText*	mNameTag;		
	PhotoPrintDoc*	mDoc;
	PhotoItemRef	mItem;
	
	static EKeyStatus	FileNameField(
								TEHandle		inMacTEH,
								UInt16			inKeyCode,
								UInt16			&ioCharCode,
								EventModifiers	inModifiers);

	virtual void		FinishCreateSelf();
	PhotoPrintDoc*		GetDocument(void)	{return mDoc;};
	PhotoItemRef		GetItem(void)	{return mItem;};
	
public:
	enum {
		class_ID = FOUR_CHAR_CODE('Pbdg')
	};

					PhotoBadge();
					PhotoBadge(	const PhotoBadge &inOriginal);		
					PhotoBadge(	const SPaneInfo		&inPaneInfo,
									const SViewInfo		&inViewInfo);
					PhotoBadge(	LStream			*inStream);

	virtual			~PhotoBadge();


	virtual void				ClickSelf(const SMouseDownEvent &inMouseDown);
	virtual LEditText*			GetNameTag(void) {return mNameTag;};
	virtual void				SetItem(PhotoPrintDoc*	inDoc, PhotoItemRef inItem);
	};//end class PhotoBadge