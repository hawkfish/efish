// PhotoPrintView.h
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved

#pragma once
#include "MRect.h"
#include "PhotoPrintController.h"

class PhotoPrintView : public LPane {
	protected:
		HORef<PhotoPrintController> mController;
		HORef<PhotoPrintModel>		mModel;
		
		LPane*	mDebug1;
		LPane*	mDebug2;
		
		virtual void	FinishCreateSelf();
			
			
	public:
		enum { class_ID = FOUR_CHAR_CODE('davP') };
	
						PhotoPrintView();
						PhotoPrintView(	const PhotoPrintView &inOriginal);		
						PhotoPrintView(	const SPaneInfo	&inPaneInfo);
						PhotoPrintView(	LStream			*inStream);
	
		virtual			~PhotoPrintView();

		virtual void		DrawSelf();
		virtual void		ClickSelf(const SMouseDownEvent &inMouseDown);

		PhotoPrintModel*	GetModel(void)	{return mModel;};

	};//end class PhotoPrintView