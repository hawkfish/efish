/*
	File:		PhotoPrintView.h

	Contains:	In MVC, view which maintaintains a model. It draws items.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		15 Jun 2000		drd		RefreshItem
*/

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

		// LDropArea
		virtual Boolean	ItemIsAcceptable( DragReference inDragRef, ItemReference inItemRef);

		// CDragAndDrop
		virtual	void	ReceiveDragItem( DragReference inDragRef, ItemReference inItemRef,
										  Size inDataSize, Boolean inCopyData, 
										  Boolean inFromFinder, Rect& inItemBounds);

		virtual void	ReceiveDraggedFile(const MFileSpec& inFile);				
		virtual void	ReceiveDraggedFolder(const MFileSpec& inFolder);				
		virtual void	SetupDraggedItem(PhotoItemRef item);
			
	public:
		enum { class_ID = FOUR_CHAR_CODE('davP') };
	
						PhotoPrintView();
						PhotoPrintView(	const PhotoPrintView &inOriginal);		
						PhotoPrintView(	const SPaneInfo		&inPaneInfo,
										const SViewInfo		&inViewInfo);
						PhotoPrintView(	LStream			*inStream);
	
		virtual			~PhotoPrintView();

		// Accesors
		PhotoPrintModel*	GetModel(void)		{ return mModel; }

		// LPane
		virtual void		ClickSelf(const SMouseDownEvent &inMouseDown);
		virtual void		DrawSelf();
		
		// enforce any constraints (like snap-to-grid, rotation increment, etc.  return if changes made
		virtual bool		AdjustTransforms(double& rot, double& skew, MRect& dest, 
												const PhotoItemRef item);

				void		RefreshItem(PhotoItemRef inItem);
};//end class PhotoPrintView