/*
	File:		GridLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				GridLayout keeps multiple images in a best-fit grid

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		20 Jun 2000		drd		Rudimentary gridding works
		19 Jun 2000		drd		Created
*/

#include "GridLayout.h"
#include <cmath>

/*
GridLayout
*/
GridLayout::GridLayout(PhotoPrintModel* inModel)
	: Layout(inModel)
{
} // GridLayout

/*
~GridLayout
*/
GridLayout::~GridLayout()
{
} // ~GridLayout

/*
LayoutImages {OVERRIDE}
*/
void
GridLayout::LayoutImages()
{
	// First determine how big the grid is
	// !!! this is hardly best fit at the moment, more like wild-assed guess
	SInt16		nImages = mModel->GetCount();
	SInt16		root = std::sqrt(nImages + 1);

	mColumns = root;
	mRows = (nImages + root - 1) / root;

	double		docW = mDocument->GetWidth() * mDocument->GetResolution();
	double		docH = mDocument->GetHeight() * mDocument->GetResolution();

	// Determine how big the cell is (taking into account gutter between cells)
	SInt16		cellW;
	if (this->GetColumns() == 1)
		cellW = docW;
	else
		cellW = (docW - (this->GetColumns() - 1) * this->GetGutter()) / this->GetColumns();
	SInt16		cellH;
	if (this->GetRows() == 1)
		cellH = docH;
	else
		cellH = (docH - (this->GetRows() - 1) * this->GetGutter()) / this->GetRows();

	// Place each
	PhotoIterator	iter;
	SInt16			i = 0;
	for (iter = mModel->begin(); iter != mModel->end(); iter++) {
		i++;									// Keep track of how many
		SInt16			row = (i - 1) / this->GetColumns();		// 1-based index, 0-based row
		SInt16			col = (i - 1) % this->GetColumns();		// 1-based index, 0-based column

		PhotoItemRef	item = *iter;
		MRect			itemBounds = item->GetNaturalBounds();
		long			propWidth = itemBounds.Width();
		long			propHeight = itemBounds.Height();
		
		long			fitWidth = cellW;
		long			fitHeight = cellH;
			
		long			outWidth;
		long			outHeight;
		PhotoPrinter::BestFit(outWidth, 
							 outHeight,
							 fitWidth,
							 fitHeight,
							 propWidth,
							 propHeight);

		itemBounds.SetWidth(outWidth);
		itemBounds.SetHeight(outHeight);	

		itemBounds.Offset(((cellW + this->GetGutter()) * col + ((fitWidth - itemBounds.Width()) / 2) -
							itemBounds.left),
							((cellH + this->GetGutter()) * row + ((fitHeight - itemBounds.Height()) / 2) -
							itemBounds.top));
		item->SetDest(itemBounds);
	}
} // LayoutImages
