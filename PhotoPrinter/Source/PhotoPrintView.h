/*
	File:		PhotoPrintView.h

	Contains:	In MVC, view which maintaintains a model. It draws items.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		26 Jul 2001		rmgw	Factor out XML parsing.  Bug #228.
		24 Jul 2001		rmgw	Add GetControllerType.  Bug #230.
		24 Jul 2001		rmgw	Add OnFilenameChanged.  Bug #219.
		24 Jul 2001		rmgw	Remove bogus OnModelDirtied.
		23 Jul 2001		rmgw	Listen to new model messages.
		20 jul 2001		dml		204 break up ListenToMessage, add ListenToCommand, OnModelChanged
		20 jul 2001		dml		190 add WarnAboutRename
		20 Jul 2001		rmgw	Make SwitchLayout take the arguments it needs.  Bug #200.
		18 Jul 2001		drd		196 Get rid of DeclareActiveBadge 
		18 Jul 2001		drd		187 Override AdaptToSuperScroll
		18 Jul 2001		drd		153 185 186 Added init arg to SetLayoutType
		12 Jul 2001		rmgw	Add MakeDragRegion.  Bug #156.
		12 Jul 2001		rmgw	Convert item copies to 'clone' events.
		12 Jul 2001		rmgw	Convert the import event to make new import.
		11 Jul 2001		rmgw	Drag and Drop uses AE now.
		10 Jul 2001		drd		91 Override ActivateSelf
		10 jul 2001		dml		add SetPrimarySelection
		06 jul 2001		dml		stop including ESortedFileList.h
		05 jul 2001		dml		25 again.  add optionalOutDestNoCaption parm to AdjustTransforms
		 2 Jul 2001		rmgw	Add const GetModel.
		29 Jun 2001		drd		96 Override InsideDropArea so we can show CopyArrowCursor
		28 Jun 2001		drd		92 gOffscreen (for drawing drag image into)
		14 Jun 2001		rmgw	CreateBadges now takes commander argument.  Bug #66.
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

#include <LView.h>
#include "CDragAndDrop.h"
#include <LListener.h>

#include "MRect.h"
#include "HORef.h"

#include "Layout.h"
#include "PhotoPrintController.h"
#include "PhotoPrintItem.h"
#include "PhotoController.h"

#include <vector>
#include <map>

class	BadgeGroup;
class	PhotoBadge;
class	MAppleEvent;

typedef HORef<MFileSpec> FileRef;
typedef std::vector<FileRef> FileRefVector;
typedef	std::map<PhotoItemRef, PhotoBadge*> BadgeMap;

class PhotoPrintView 	: public LView
						, public CDragAndDrop
						, public LListener 
{
protected:
	HORef<PhotoController>		mController;
	OSType						mControllerType;
	Layout*						mLayout;
	HORef<PhotoPrintModel>		mModel;
	PhotoItemList				mSelection;
	SInt16						mCurPage;
	HORef<BadgeGroup>			mBadgeGroup;
	BadgeMap					mBadgeMap;
	PhotoPrintDoc*				mDoc;
	
	// LPane
	virtual void	ActivateSelf();
	virtual void	AdaptToSuperScroll(SInt32 inHorizScroll, SInt32 inVertScroll);
	virtual void	FinishCreateSelf();

	// LListener
	virtual void	OnFilenameChanged	(const	FSSpec* 				inNewSpec);
	virtual void	OnModelChanged		(PhotoPrintDoc*					inDoc);
	virtual	void	OnModelItemsAdded	(PhotoPrintModel::MessageRange*	inRange);
	virtual	void	OnModelItemsChanged	(PhotoPrintModel::MessageRange*	inRange);
	virtual	void	OnModelItemsRemoved	(PhotoPrintModel::MessageRange*	inRange);
	
	virtual void	ListenToCommand(MessageT inMessage, void* ioParam);
	virtual void	ListenToMessage(MessageT inMessage, void* ioParam);

	// LDropArea
	virtual void	DoDragReceive		(DragReference	inDragRef);
	virtual	Boolean DragIsAcceptable 	(DragReference	inDragRef);

	// CDragAndDrop
	virtual	PhotoItemRef	FindDropItem (DragReference	inDragRef) const;
	
	virtual	void	MakeDropAELocation (AEDesc&			outLoc,
										DragReference	inDragRef) const;
	virtual	void	MakeItemAELocation (AEDesc&			outLoc,
										PhotoItemRef	inItem) const;
										
	virtual void	InsideDropArea( DragReference inDragRef, Point& theMouseLocation,
									Point& thePinnedLocation);
	virtual	void	ReceiveDragItem( DragReference inDragRef, ItemReference inItemRef,
									  Size inDataSize, Boolean inCopyData, 
									  Boolean inFromFinder, Rect& inItemBounds);


public:			
	virtual void	SetupDraggedItem(PhotoItemRef item);
protected:
	virtual void	CreateBadges(LCommander* inBadgeCommander);
	virtual void	UpdateBadges(bool inState);
	virtual void	DestroyBadges(void);

public:
	enum {
		class_ID = FOUR_CHAR_CODE('davP'),

		kImageOnly = false,
		kImageAndHandles = true,

		kInitialize = true,
		kDontInitialize = false
	};

					PhotoPrintView();
					PhotoPrintView(	const PhotoPrintView &inOriginal);		
					PhotoPrintView(	const SPaneInfo		&inPaneInfo,
									const SViewInfo		&inViewInfo);
					PhotoPrintView(	LStream			*inStream);

	virtual			~PhotoPrintView();

	// Accessors
			PhotoBadge*			GetBadgeForItem(PhotoItemRef inItem);
			PhotoController*	GetController()		{ return mController; }
			OSType				GetControllerType()		{ return mControllerType; }
			Layout*				GetLayout()					{ return mLayout; }
			PhotoPrintModel*			GetModel 	(void)				{ return mModel; }
			const	PhotoPrintModel*	GetModel 	(void) const		{ return mModel; }
			PhotoPrintDoc*				GetDocument	(void) const		{ return mDoc; }	
			void						SetDocument	(PhotoPrintDoc*	inDoc);	
			SInt16		GetCurPage(void)			{ return mCurPage; }
			void		GetBodyToScreenMatrix(MatrixRecord& outMatrix);
	virtual void		SetController(OSType inController, LCommander* inBadgeCommander);
	virtual bool		WarnAboutRename	(void);

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
	virtual	void		AddFlavors		(DragReference inDragRef);
	virtual void		MakeDragRegion	(DragReference inDragRef, RgnHandle inDragRegion);

	virtual void		DoDragSendData(FlavorType inFlavor,
									ItemReference inItemRef,
									DragReference inDragRef);

	// enforce any constraints (like snap-to-grid, rotation increment, etc.  return if changes made
	virtual bool		AdjustTransforms(double& rot, double& skew, MRect& dest, 
											const PhotoItemRef item, MRect* optionalOutDestNoCaption = NULL);

			void		RefreshItem(PhotoItemRef inItem, const bool inHandles = kImageOnly);

			void		SetLayoutType(const OSType inType, const bool inInit = kInitialize);
			void		SwitchLayout(Layout::LayoutType inType, const UInt32 inCount);

	// Selection
	virtual void					AddToSelection(PhotoItemList& additions);
	virtual void					AddToSelection(PhotoItemRef inAddition);
	virtual void					ClearSelection(void);
	virtual PhotoItemRef			GetPrimarySelection(void) const;
	virtual void					SetPrimarySelection(PhotoItemRef newPrimary);
	virtual	Handle					GetSelectedData(const OSType inType) const;
			bool					IsAnythingSelected() const;
	virtual	bool					IsSelected(PhotoItemRef inItem);
	virtual	void					RemoveFromSelection(ConstPhotoIterator 	inBegin,
														ConstPhotoIterator 	inEnd);
	virtual void					RemoveFromSelection(PhotoItemList& removals);
	virtual void 					Select(const PhotoItemList& target);
	virtual	const 	PhotoItemList&	Selection(void) const;
	virtual void					ToggleSelected(PhotoItemList& togglees);

protected:
	static	LGWorld*		gOffscreen;
};//end class PhotoPrintView