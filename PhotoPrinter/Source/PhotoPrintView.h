/*
	File:		PhotoPrintView.h

	Contains:	In MVC, view which maintaintains a model. It draws items.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		28 jul 2000		dml		more sorting madness
		28 jul 2000		dml		add SortFileList
		20 Jul 2000		drd		Override AdjustCursorSelf
		26 Jun 2000		drd		Override DoDragReceive
		26 Jun 2000		drd		Added GetLayout
		23 Jun 2000		drd		ReceiveDragEvent arg is now MAppleEvent; SetLayoutType instead
								of MakeLayout
		21 Jun 2000		drd		Added MakeLayout, ReceiveDragEvent
		19 Jun 2000		drd		Added mLayout
		15 Jun 2000		drd		RefreshItem
*/

#pragma once

#include "MRect.h"
#include "PhotoPrintController.h"
#include <LView.h>
#include "CDragAndDrop.h"
#include "PhotoPrintItem.h"
#include "ESortedFileList.h"

class	Layout;
class	MAppleEvent;

class PhotoPrintView : public LView, CDragAndDrop {
	protected:
		HORef<PhotoPrintController> mController;
		Layout*						mLayout;
		HORef<PhotoPrintModel>		mModel;
				
		virtual void	FinishCreateSelf();

		// LDropArea
		virtual void	DoDragReceive(DragReference		inDragRef);
		virtual Boolean	ItemIsAcceptable(DragReference inDragRef, ItemReference inItemRef);

		void	ProcessSortedFileList(FullFileList& sortedList);

		// CDragAndDrop

		virtual void	ReceiveDraggedFile(const MFileSpec& inFile);				
		virtual void	ReceiveDraggedFolder(const MFileSpec& inFolder);				
		virtual void	SetupDraggedItem(PhotoItemRef item);
		virtual void	SortFileList(FileRefVector& items, FullFileList& outSortedList);
		
		virtual void	ExtractFSSpecFromDragItem(DragReference inDragRef, 
										ItemReference inItemRef,
									  	Size inDataSize,
							  			const FlavorType expectedType,
							  			MFileSpec& outFileSpec);

			
	public:
		enum { class_ID = FOUR_CHAR_CODE('davP') };
	
						PhotoPrintView();
						PhotoPrintView(	const PhotoPrintView &inOriginal);		
						PhotoPrintView(	const SPaneInfo		&inPaneInfo,
										const SViewInfo		&inViewInfo);
						PhotoPrintView(	LStream			*inStream);
	
		virtual			~PhotoPrintView();

		// Accessors
				Layout*		GetLayout()					{ return mLayout; }
				PhotoPrintModel*	GetModel(void)		{ return mModel; }
				void		SetLayoutType(const OSType inType);

		// LPane
		virtual void		AdjustCursorSelf(
									Point				inPortPt,
									const EventRecord	&inMacEvent);
		virtual void		ClickSelf(const SMouseDownEvent &inMouseDown);
		virtual void		DrawSelf();
		
		// enforce any constraints (like snap-to-grid, rotation increment, etc.  return if changes made
		virtual bool		AdjustTransforms(double& rot, double& skew, MRect& dest, 
												const PhotoItemRef item);

		virtual	void		ReceiveDragEvent(const MAppleEvent&	inAppleEvent);
				void		RefreshItem(PhotoItemRef inItem);
};//end class PhotoPrintView