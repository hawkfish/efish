// PhotoPrintView.h
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved

#pragma once
#include "MRect.h"
#include "PhotoPrintController.h"
#include <LView.h>

class PhotoPrintView : public LView {
	protected:
		HORef<PhotoPrintController> mController;
		HORef<PhotoPrintModel>		mModel;
				
		virtual void	FinishCreateSelf();
			
			
	public:
		enum { class_ID = FOUR_CHAR_CODE('davP') };
	
						PhotoPrintView();
						PhotoPrintView(	const PhotoPrintView &inOriginal);		
						PhotoPrintView(	const SPaneInfo		&inPaneInfo,
										const SViewInfo		&inViewInfo);
						PhotoPrintView(	LStream			*inStream);
	
		virtual			~PhotoPrintView();

		virtual void		DrawSelf();
		virtual void		ClickSelf(const SMouseDownEvent &inMouseDown);

		PhotoPrintModel*	GetModel(void)	{return mModel;};
		
		// enforce any constraints (like snap-to-grid, rotation increment, etc.  return if changes made
		virtual Boolean		AdjustTransforms(double& rot, double& skew, MRect& dest, 
												const PhotoItemRef item);

	};//end class PhotoPrintView