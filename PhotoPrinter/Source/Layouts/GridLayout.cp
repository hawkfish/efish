/*
	File:		GridLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				GridLayout keeps multiple images in a best-fit grid

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		11 jul 2000		dml		multipage support
		06 Jul 2000		drd		Override AdjustDocumentOrientation for 2-landscape special case
		28 jun 2000		dml		use EUtil symbolic constants, use FitRectInside instead of BestFit
		27 Jun 2000		drd		LayoutImages sends AdjustDocumentOrientation
		26 Jun 2000		drd		Get rid of a few conversion warnings
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		21 jun 2000 	dml		BestFit not takes (optional) parm -- don't expand past natural bounds
		20 jun 2000		dml		using EUtil
		20 Jun 2000		drd		Rudimentary gridding works
		19 Jun 2000		drd		Created
*/

#include "GridLayout.h"
#include <cmath>
#include "EUtil.h"
#include "PhotoUtility.h"
#include "PhotoPrinter.h"
/*
GridLayout
*/
GridLayout::GridLayout(HORef<PhotoPrintModel>& inModel)
	: Layout(inModel)
{
	mType = kGrid;
	mItemsPerPage  = 0;
} // GridLayout

/*
~GridLayout
*/
GridLayout::~GridLayout()
{
} // ~GridLayout




//--------------------------------------------------------------
//AddItem (OVERRIDE) 
//--------------------------------------------------------------
void		
GridLayout::AddItem(PhotoItemRef inItem){

	// add the item as usual
	Layout::AddItem(inItem);
	
	if (mItemsPerPage == 0)
		mItemsPerPage = MaxItemsPerPage(1.8, 1.8);  //READ FROM PREFS!

	mNumPages = mModel->GetCount() / mItemsPerPage;
	if (mNumPages * mItemsPerPage < mModel->GetCount())
		++mNumPages; 

}//end AddItem




/*
AdjustDocumentOrientation {OVERRIDE}
	Special case to put two landscapes on portrait paper
*/
void
GridLayout::AdjustDocumentOrientation(SInt16 /*numPages*/)
{

	UInt32		l = this->CountOrientation(kLandscape);
	UInt32		p = this->CountOrientation(kPortrait);

	EPrintSpec* spec = (EPrintSpec*)mDocument->GetPrintRec();
	// Note that we have a slight bias for landscape (since most pictures are done that way)
	if (l == 2 && p == 0) {
		spec->SetOrientation(kPortrait);
		mDocument->MatchViewToPrintRec(mNumPages);
	} else {
		Layout::AdjustDocumentOrientation(mNumPages);
	}
} // AdjustDocumentOrientation

/*
LayoutImages {OVERRIDE}
*/
void
GridLayout::LayoutImages()
{
	// First be sure the paper is switched optimally
	this->AdjustDocumentOrientation();

	// Then determine how big the grid is
	// !!! this is hardly best fit at the moment, more like wild-assed guess
	UInt32		nImages = mModel->GetCount();
	SInt16		root = std::sqrt(nImages + 1);

	mColumns = root;
	mRows = (nImages + root - 1) / root;

	// get entire size of Document
	SInt32		docW = (SInt16)(mDocument->GetWidth() * mDocument->GetResolution() + 0.5);
	SInt32		docH = (SInt16)(mDocument->GetHeight() * mDocument->GetResolution() + 0.5);

	// and reduce down to a single page size for layout purposes
	docW /= mNumPages;
	docH /= mNumPages;

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

		MRect cellBounds (0, 0, cellH, cellW);
		EUtil::FitRectInside(itemBounds, cellBounds, itemBounds, EUtil::kDontExpand);


		itemBounds.Offset(((cellW + this->GetGutter()) * col + ((cellW - itemBounds.Width()) / 2) -
							itemBounds.left),
							((cellH + this->GetGutter()) * row + ((cellH - itemBounds.Height()) / 2) -
							itemBounds.top));

		item->SetDest(itemBounds);
	}
} // LayoutImages



//--------------------------------------------------------------
// MaxItemsPerPage
// 
// incoming values in floating-point inches
//--------------------------------------------------------------
SInt16		
GridLayout::MaxItemsPerPage(double minWidth, double minHeight) {

	// find out about our current page
	// orientation
	EPrintSpec* spec = (EPrintSpec*)mDocument->GetPrintRec();
	OSType	curOrientation (spec->GetOrientation());

	// printable area (taking into account margins, etc)
	MRect printableArea;
	PhotoPrinter::CalculatePrintableRect(spec, &(mDocument->GetPrintProperties()), printableArea);

	// resolution
	SInt16 vRes;
	SInt16 hRes;
	spec->GetResolutions(vRes, vRes);

	// multiply minimum sizes by resolution (we use vRes exclusively) to convert to pixels
	minWidth *= vRes;
	minHeight *= vRes;

	// add gutter (which comes as pixels at 72 dpi)
	double gutterAtPrintResolution 	(GetGutter() * vRes / kDPI);
	minWidth += gutterAtPrintResolution;
	minHeight += gutterAtPrintResolution;

	SInt16 hCount (printableArea.Width() / minWidth);
	SInt16 vCount (printableArea.Height() / minHeight);	

	return hCount * vCount;
}//end CountPages
	


//--------------------------------------------------------------
//AdjustViewSize
//--------------------------------------------------------------
void		
GridLayout::AdjustViewSizeToHoldItems(SInt16 numItems) {
	
}//end AdjustViewSize




