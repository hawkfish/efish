/*
	File:		MultipleLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.
				MultipleLayout has multiple copies of a single image.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		09 Jul 2001		rmgw	AdoptNewItem now returns a PhotoIterator. Bug #142.
		02 Jul 2001		rmgw	AdoptNewItem now takes a PhotoIterator.
		17 Aug 2000		drd		Added MakeNewImage
		17 Aug 2000		drd		Removed LayoutImages (superclass works just fine);
								GetDistinctImages now inlined
		15 Aug 2000		drd		Moved Initialize to FixedLayout
		26 Jun 2000		drd		GetNameIndex, AddItem, Initialize, LayoutImages
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#pragma once

#include "FixedLayout.h"

class MultipleLayout : public FixedLayout
{
public:
						MultipleLayout(HORef<PhotoPrintModel>& inModel);
	virtual 			~MultipleLayout();

	// Layout
	virtual	PhotoIterator	AddItem(PhotoItemRef inItem, PhotoIterator inBefore);
	virtual	bool		CanAddToBackground(const UInt16 inCount);
	virtual	SInt16		GetNameIndex() const				{ return 4; }
	virtual SInt16		GetDistinctImages(void)				{ return 1; }

	// FixedLayout
	virtual	PhotoPrintItem*	MakeNewImage();
};
