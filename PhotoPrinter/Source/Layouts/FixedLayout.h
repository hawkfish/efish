/*
	File:		FixedLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.
				FixedLayout has Fixed copies of one or more images (and is subclassed).

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		01 Aug 2001		rmgw	Rename ImageCount property to ItemsPerPage.  Bug #265.
		23 jul 2001		dml		179 add CalcOrientation
		23 Jul 2001		rmgw	Add doc and type to constructor.
		19 Jul 2001		drd		173 176 IsFlexible
		19 Jul 2001		rmgw	Add HasPlaceholders method.  Bug #183.
		18 Jul 2001		rmgw	Add RemoveItems method.
		18 Jul 2001		rmgw	Add SetItems method.
		10 jul 2001		dml		add GetItemsPerPage
		09 Jul 2001		rmgw	AdoptNewItem now returns a PhotoIterator. Bug #142.
		02 Jul 2001		rmgw	AdoptNewItem now takes a PhotoIterator.
		16 May 2001		drd		38 We can use generic options dialog
		18 Jan 2001		drd		CommitOptionsDialog returns value and has new arg
		07 Sep 2000		drd		Override GetName
		07 sep 2000		dml		add TryToFillFirstEmpty, override AdjustDocumentOrientation
		17 Aug 2000		drd		Added MakeNewImage
		15 Aug 2000		drd		Added HasOptions; changed ancestry from Layout to GridLayout;
								override AddItem
		09 Aug 2000		drd		Moved mImageCount from SchoolLayout; changes for dialog
		26 Jun 2000		drd		GetNameIndex
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#pragma once

#include "GridLayout.h"

class FixedLayout : public GridLayout
{
public:
						FixedLayout(PhotoPrintDoc*			inDoc, 
								    HORef<PhotoPrintModel>&	inModel,
								    LayoutType 				inType = kFixed);
	virtual 			~FixedLayout();

	virtual	PhotoIterator	AddItem(PhotoItemRef inItem, PhotoIterator inBefore);
	virtual	void		SetItems (ConstPhotoIterator inBegin, ConstPhotoIterator inEnd);
	virtual	void		RemoveItems (ConstPhotoIterator inBegin, ConstPhotoIterator inEnd);
	virtual void		AdjustDocumentOrientation(SInt16 numPages);

	virtual	OSType		CalcOrientation() const;

	virtual	bool		CanAddToBackground(const UInt16 inCount);
	virtual	LStr255		GetName() const;
	virtual	SInt16		GetNameIndex() const						{ return 3; }
	virtual	bool		HasOptions() const							{ return true; }
	virtual	bool		HasPlaceholders() const						{ return true; }
	virtual	void		Initialize();
	virtual	bool		IsFlexible() const							{ return false; }

	virtual	PhotoPrintItem*	MakeNewImage();
	virtual	void		SetItemsPerPage(const UInt32 inItemsPerPage);
	virtual UInt32		GetItemsPerPage(void) const {return mItemsPerPage;};

protected:

	virtual PhotoIterator	TryToFillFirstEmpty(PhotoItemRef inItem, PhotoIterator inBefore);

	UInt32				mItemsPerPage;
};
