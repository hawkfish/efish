/*
	File:		GridLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.
				GridLayout keeps multiple images in a best-fit grid

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 jul 2000		dml		multipage support
		06 Jul 2000		drd		Override AdjustDocumentOrientation for 2-landscape special case
		26 Jun 2000		drd		GetNameIndex
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "Layout.h"

class GridLayout : public Layout
{
protected:
	SInt16	mItemsPerPage;
	SInt16	mNumPages;

	virtual SInt16		MaxItemsPerPage(double minWidth, double minHeight);
	virtual void		AdjustViewSizeToHoldItems(SInt16 numItems);

public:
						GridLayout(HORef<PhotoPrintModel>& inModel);
	virtual 			~GridLayout();

	virtual	void		AddItem(PhotoItemRef inItem);
	virtual	void		AdjustDocumentOrientation(SInt16  numPages = 1);
	virtual	bool		CanAddToBackground(const UInt16 /*inCount*/)	{ return true; }
	virtual	SInt16		GetNameIndex() const				{ return 1; }
	virtual	void		LayoutImages();
};
