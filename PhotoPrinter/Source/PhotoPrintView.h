/*
	File:		PhotoPrintView.h

	Contains:	In MVC, view which maintaintains a model. It draws items.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Jun 2001		drd		73 Removed Activate (since there's no gCurTool we need to track)
		23 May 2001		drd		69 SetupDraggedItem now public
		23 May 2001		drd		Moved XML parsing functions here, and renamed them; IsSelected
		16 May 2001		drd		67 Override ApplyForeAndBackColors (fixes drop hilite OS 9)
		22 Mar 2001		drd		SwitchLayout
		21 Mar 2001		drd		Now an LListener
		09 feb 2001		dml		add GetBodyToScreenMatrix(), bug 34, bug 58
		23 feb 2001		dml		add ShowBadges, CreateBadges
		15 Feb 2001		rmgw	10 Add RemoveFromSelection that takes iterators
		11 Dec 2000		drd		13 Override ReceiveDragItem
		07 dec 2000		dml		DrawHeader, DrawFooter take yOffset arg for multiple pages
		05 dec 2000		dml		add DrawHeader, DrawFooter
		27 Sep 2000		rmgw	Change ItemIsAcceptable to DragIsAcceptable.
		18 sep 2000		dml		add mCurPage, GetCurPage()
		30 Aug 2000		drd		New version of AddToSelection
		29 Aug 2000		drd		GetSelectedData
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
#include "PhotoController.h"
#include <vector>
#include <map>

class	BadgeGroup;
class	PhotoBadge;
class	Layout;
class	MAppleEvent;

typedef HORef<MFileSpec> FileRef;
typedef std::vector<FileRef> FileRefVector;
typedef	std::map<PhotoItemRef, PhotoBadge*> BadgeMap;

class PhotoPrintView : public LView, CDragAndDrop, public LListener {
protected:
	HORef<PhotoController>		mController;
	Layout*						mLayout;
	HORef<PhotoPrintModel>		mModel;
	PhotoItemList				mSelection;
	SInt16						mCurPage;
	HORef<BadgeGroup>			mBadgeGroup;
	BadgeMap					mBadgeMap;
			
	virtual void	FinishCreateSelf();

	// LListener
	virtual void	ListenToMessage(MessageT inMessage, void* ioParam);

	// LDropArea
	virtual void	DoDragReceive		(DragReference	inDragRef);
	virtual	Boolean DragIsAcceptable 	(DragReference	inDragRef);

	// CDragAndDrop
	virtual	void	ReceiveDragItem( DragReference inDragRef, ItemReference inItemRef,
									  Size inDataSize, Boolean inCopyData, 
									  Boolean inFromFinder, Rect& inItemBounds);


			void	ProcessFileList(FileRefVector& list);
	virtual void	ReceiveDraggedFile(const MFileSpec& inFile);				
	virtual void	ReceiveDraggedFolder(const MFileSpec& inFolder);
public:			
	virtual void	SetupDraggedItem(PhotoItemRef item);
protected:
	virtual void	CreateBadges();
	virtual void	UpdateBadges(bool inState);
	virtual void	DestroyBadges(void);
	virtual void	DeclareActiveBadge(void);

	// XML
	static void		ObjectsHandler(XML::Element &elem, void* userData);
	static void		PhotoHandler(XML::Element &elem, void* userData);

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
			PhotoBadge*		GetBadgeForItem(PhotoItemRef inItem);
			PhotoController*	GetController()		{ return mController; }
			Layout*		GetLayout()					{ return mLayout; }
			PhotoPrintModel*	GetModel(void)		{ return mModel; }
			SInt16		GetCurPage(void)			{ return mCurPage; }
			void		GetBodyToScreenMatrix(MatrixRecord& outMatrix);
	virtual void		SetController(OSType inController);

	// LPane
	virtual void		AdjustCursorSelf(
								Point				inPortPt,
								const EventRecord	&inMacEvent);
	virtual void		ApplyForeAndBackColors() const;
	virtual void		ClickSelf(const SMouseDownEvent &inMouseDown);

	virtual void		DrawHeader(SInt32 yOffset = 0);
	virtual void		DrawFooter(SInt32 yOffset = 0);
	virtual void		DrawPrintable(SInt32 yOffset = 0);
	virtual void		DrawSelf();
	
	//LView
	virtual void		Refresh();
	
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

			void		SetLayoutType(const OSType inType);
			void		SwitchLayout(const SInt32 inType, const SInt32 inDuplicated);

	// Selection
	virtual void					AddToSelection(PhotoItemList& additions);
	virtual void					AddToSelection(PhotoItemRef inAddition);
	virtual void					ClearSelection(void);
	virtual PhotoItemRef			GetPrimarySelection(void) const;
	virtual	Handle					GetSelectedData(const OSType inType) const;
			bool					IsAnythingSelected() const;
	virtual	bool					IsSelected(PhotoItemRef inItem);
	virtual	void					RemoveFromSelection(PhotoIterator 	inBegin,
														PhotoIterator 	inEnd);
	virtual void					RemoveFromSelection(PhotoItemList& removals);
	virtual void 					Select(const PhotoItemList& target);
	virtual	const 	PhotoItemList&	Selection(void) const;
	virtual void					ToggleSelected(PhotoItemList& togglees);
};//end class PhotoPrintView