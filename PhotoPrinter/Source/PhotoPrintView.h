// PhotoPrintView.h
// Copyright © 2000 Electric Fish, Inc.  All Rights Reserved

#pragma once
#include "MRect.h"
#include "PhotoPrintController.h"
#include <LView.h>
#include "CDragAndDrop.h"
#include "PhotoPrintItem.h"

class PhotoPrintView : public LView, CDragAndDrop {
	protected:
		HORef<PhotoPrintController> mController;
		HORef<PhotoPrintModel>		mModel;
				
		virtual void	FinishCreateSelf();

		virtual Boolean	ItemIsAcceptable( DragReference inDragRef, ItemReference inItemRef);
		virtual	void	ReceiveDragItem( DragReference inDragRef, ItemReference inItemRef,
										  Size inDataSize, Boolean inCopyData, 
										  Boolean inFromFinder, Rect& inItemBounds);
		virtual void	ReceiveDraggedFolder(const MFileSpec& inFolder);				
		virtual void	ReceiveDraggedFile(const MFileSpec& inFile);				
		virtual void	SetupDraggedItem(PhotoItemRef item);
			
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
		virtual bool		AdjustTransforms(double& rot, double& skew, MRect& dest, 
												const PhotoItemRef item);

	};//end class PhotoPrintView