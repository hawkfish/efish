/*
	File:		PhotoPrintView.h

	Contains:	In MVC, view which maintaintains a model. It draws items.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):
	
		29 Aug 2000		drd		AddFlavors, DoDragSendData
		21 Aug 2000		drd		Added arg to RefreshItem
		17 aug 2000		dml		add Activate (override) to set tool
		11 aug 2000		dml		add SetController();
		09 aug 2000		dml		added CountItemsInFolder, made ExtractFSSpec... static public
		07 aug 2000		dml		change to use PhotoController 
		07 aug 2000		dml		add ClearSelection
		05 aug 2000		dml		add ToggleSelection
		04 aug 2000		dml		add AddToSelection, RemoveFromSelection
		04 Aug 2000		drd		Renamed GetSelection to Selection (already taken by LPane);
								added GetController accessor
		03 aug 2000		dml		add selection (move from model)
		03 aug 2000		dml		move sorting to model
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
#include "HORef.h"
#include <vector>
#include "PhotoController.h"

class	Layout;
class	MAppleEvent;

typedef HORef<MFileSpec> FileRef;
typedef std::vector<FileRef> FileRefVector;

class PhotoPrintView : public LView, CDragAndDrop {
protected:
	HORef<PhotoController>		mController;
	Layout*						mLayout;
	HORef<PhotoPrintModel>		mModel;
	PhotoItemList				mSelection;
			
	virtual void	FinishCreateSelf();

	// LDropArea
	virtual void	DoDragReceive(DragReference		inDragRef);
	virtual Boolean	ItemIsAcceptable(DragReference inDragRef, ItemReference inItemRef);


			void	ProcessFileList(FileRefVector& list);
	virtual void	ReceiveDraggedFile(const MFileSpec& inFile);				
	virtual void	ReceiveDraggedFolder(const MFileSpec& inFolder);				
	virtual void	SetupDraggedItem(PhotoItemRef item);		
		
public:
	enum {
		class_ID = FOUR_CHAR_CODE('davP'),

		kImageOnly = false,
		kImageAndHandles = true
	};

					PhotoPrintView();
					PhotoPrintView(	const PhotoPrintView &inOriginal);		
					PhotoPrintView(	const SPaneInfo		&inPaneInfo,
									const SViewInfo		&inViewInfo);
					PhotoPrintView(	LStream			*inStream);

	virtual			~PhotoPrintView();

	// Accessors
			PhotoController*	GetController()	{ return mController; }
			Layout*		GetLayout()					{ return mLayout; }
			PhotoPrintModel*	GetModel(void)		{ return mModel; }
			void		SetLayoutType(const OSType inType);

	// LPane
	virtual void		AdjustCursorSelf(
								Point				inPortPt,
								const EventRecord	&inMacEvent);
	virtual void		SetController(OSType inController);
	virtual void		ClickSelf(const SMouseDownEvent &inMouseDown);
	virtual void		DrawSelf();
	
	//LView
	virtual void		Activate();
	
	// CDragItem
	virtual	void		AddFlavors(DragReference inDragRef);
	virtual void		DoDragSendData(FlavorType inFlavor,
									ItemReference inItemRef,
									DragReference inDragRef);

	// enforce any constraints (like snap-to-grid, rotation increment, etc.  return if changes made
	virtual bool		AdjustTransforms(double& rot, double& skew, MRect& dest, 
											const PhotoItemRef item);

	virtual	void		ReceiveDragEvent(const MAppleEvent&	inAppleEvent);
			void		RefreshItem(PhotoItemRef inItem, const bool inHandles = kImageOnly);
			
	// Selection
	virtual PhotoItemRef			GetPrimarySelection(void) const;
	virtual void 					Select(const PhotoItemList& target);
	virtual void					AddToSelection(PhotoItemList& additions);
	virtual void					RemoveFromSelection(PhotoItemList& removals);
	virtual void					ToggleSelected(PhotoItemList& togglees);
	virtual void					ClearSelection(void);
	
	virtual	const 	PhotoItemList&	Selection(void) const;
			bool					IsAnythingSelected() const;
	

	static	void	ExtractFSSpecFromDragItem(DragReference inDragRef, 
									ItemReference inItemRef,
								  	Size inDataSize,
						  			const FlavorType expectedType,
						  			MFileSpec& outFileSpec);

	static  SInt16	CountItemsInsideFolder(const MFileSpec& inFolderSpec, bool recurseDown = true);

};//end class PhotoPrintView