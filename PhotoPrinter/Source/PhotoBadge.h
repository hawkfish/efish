/*
	File:		PhotoBadge.h

	Contains:	An adornment class which is used for nametag on-screen functions

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	23 feb 2001		dml			created

*/
#pragma once

#include <LView.h>
#include <LEditText.h>
#include "MPString.h"
#include "UKeyFilters.h"

#include "PhotoPrintItem.h"

class PhotoBadge : public LView {
protected:
	
	LEditText*		mNameTag;		
	virtual void	FinishCreateSelf();
	PhotoItemRef	mItem;
	
	static EKeyStatus	FileNameField(
								TEHandle		inMacTEH,
								UInt16			inKeyCode,
								UInt16			&ioCharCode,
								EventModifiers	inModifiers);


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
	virtual void				GetName(LString& outText) {return mNameTag->GetText(outText);};
	virtual void				SetName(ConstStrFileNameParam inName);
	virtual bool				ValidName(ConstStrFileNameParam inName);

	virtual void				SetItem(PhotoItemRef inItem);
	};//end class PhotoBadge