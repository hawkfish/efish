/*
	File:		GridLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				GridLayout keeps multiple images in a best-fit grid

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		14 Jul 2000		drd		AdjustDocumentOrientation calls SetOrientation twice; fixed 5/6
		14 Jul 2000		drd		Changed layout (CalculateGrid instead of CalculateRowsCols,
								MaxItemsPerPage)
		13 jul 2000		dml		removed side-effect from MaxItemsPerPage
		12 jul 2000		dml		more aux functions for multipage support
		11 Jul 2000		drd		Eliminate warnings
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
#include "PhotoPrintPrefs.h"
#include "AlignmentGizmo.h"
#include "PrintProperties.h"
#include "ERect32.h"
/*
GridLayout
*/
GridLayout::GridLayout(HORef<PhotoPrintModel>& inModel)
	: Layout(inModel)
{
	mType = kGrid;
	mItemsPerPage = 0;

} // GridLayout

/*
~GridLayout
*/
GridLayout::~GridLayout()
{
} // ~GridLayout

/*
AddItem (OVERRIDE) 
*/
void		
GridLayout::AddItem(PhotoItemRef inItem)
{
	// add the item as usual
	Layout::AddItem(inItem);
	
}//end AddItem

/*
AdjustDocumentOrientation {OVERRIDE}
	Special case to put two landscapes on portrait paper
	Side effect: sets mRows, mColumns
*/
void
GridLayout::AdjustDocumentOrientation(SInt16 /*numPages*/)
{
	// printable area (taking into account margins, etc)
	MRect		printableArea;
	EPrintSpec* spec = mDocument->GetPrintRec();
	PhotoPrinter::CalculatePrintableRect(spec, &(mDocument->GetPrintProperties()), printableArea);

	// Figure
	OSType		orientation;
	this->CalculateGrid(printableArea, mModel->GetCount(), mRows, mColumns, orientation);

	mItemsPerPage = this->GetRows() * this->GetColumns();
	mNumPages = mModel->GetCount() / mItemsPerPage;
	if (mNumPages * mItemsPerPage < mModel->GetCount()) 
		mNumPages++; 

	spec->SetOrientation(orientation);
	spec->SetOrientation(orientation);			// ??? Lexmark seems to need this
	mDocument->MatchViewToPrintRec(mNumPages); // do this anyway, since changes according to #pages
} // AdjustDocumentOrientation


//--------------------------------------------------------------
// CalculateCellSize
//--------------------------------------------------------------
void		
GridLayout::CalculateCellSize(
	const ERect32&	inPageSize,
	const SInt16	inRows,
	const SInt16	inCols,
	ERect32&		outCellSize,
	ERect32&		outUnusedBottomPad) const
{
	double hMin;
	double vMin;
	PhotoItemProperties::SizeLimitToInches(PhotoPrintPrefs::Singleton()->GetMinimumSize(), hMin, vMin);
	double hMax;
	double vMax;
	PhotoItemProperties::SizeLimitToInches(PhotoPrintPrefs::Singleton()->GetMaximumSize(), hMax, vMax);
	
	// convert inches to screen resolution
	hMin *= mDocument->GetResolution();
	vMin *= mDocument->GetResolution();
	hMax *= mDocument->GetResolution();
	vMax *= mDocument->GetResolution();
	
	// starting with page size, remove unavailable gutter
	SInt32		width = inPageSize.Width() - ((inCols - 1) * this->GetGutter());
	SInt32		height = inPageSize.Height() - ((inRows - 1) * this->GetGutter());

//!!!	CalculateRowsCols(pageSize, mRows, mColumns);

	//divide by rows, cols
	width /= inCols;
	height /= inRows;

	outCellSize.MakeEmpty();
	outCellSize.SetWidth(width);
	outCellSize.SetHeight(height);

	ERect32		maximum(0, 0, vMax, hMax);
	if (!maximum.IsEmpty())
		outCellSize *= maximum;
	
	//ok.  now that we know the cell size, determine how much space is unused at the bottom of the page
	outUnusedBottomPad.MakeEmpty();
	outUnusedBottomPad.SetWidth(inPageSize.Width());
	outUnusedBottomPad.SetHeight(inPageSize.Height() - (outCellSize.Height() * GetRows()) - ((GetRows() - 1) * GetGutter()));
	outUnusedBottomPad.Offset(0, inPageSize.bottom - outUnusedBottomPad.Height());

}//end CalculateCellSize

/*
CalculateGrid
	Given a page and how many items we have to lay out, determines how many rows and columns
	are on each page.
*/
void
GridLayout::CalculateGrid(
	const ERect32&	inPageSize,
	const SInt32	inCount,
	SInt16&			outRows,
	SInt16&			outCols,
	OSType&			outOrientation)
{
	OSType		forcedOrientation, preferredOrientation, reversedOrientation;
	UInt32		l = this->CountOrientation(kLandscape);
	UInt32		p = this->CountOrientation(kPortrait);
	bool		forced = true;
	if (p > l) {
		preferredOrientation = kPortrait;
		reversedOrientation = kLandscape;
	} else {
		preferredOrientation = kLandscape;
		reversedOrientation = kPortrait;
	}
	switch (mDocument->GetPrintProperties().GetRotationBehavior()) {
		case PrintProperties::kForceLandscape:
			forcedOrientation = kLandscape;
			break;

		case PrintProperties::kForcePortrait:
			forcedOrientation = kPortrait;
			break;

		default:
			forcedOrientation = preferredOrientation;
			forced = false;
	}

	switch (inCount) {
		case 1:
			outRows = 1;
			outCols = 1;
			outOrientation = forcedOrientation;
			return;								// Nothing else to do! (Limits recursion)
			break;

		case 2:
			if (l == p || forced)				// Both have different orientation
				outOrientation = forcedOrientation;
			else
				outOrientation = reversedOrientation;
			if (outOrientation == kPortrait) {
				outRows = 2;
				outCols = 1;
			} else {
				outRows = 1;
				outCols = 2;
			}
			break;

		case 5:
		case 6:
			if (forced)
				outOrientation = forcedOrientation;
			else
				outOrientation = reversedOrientation;
			if (outOrientation == kPortrait) {
				outRows = 3;
				outCols = 2;
			} else {
				outRows = 2;
				outCols = 3;
			}
			break;

		default:
			// Just divide up the page, using the same orientation as the majority of the images
			outCols = std::sqrt(inCount + 1);
			outRows = (inCount + outCols - 1) / outCols;
			outOrientation = forcedOrientation;
			break;
	} // end switch

	// Now see if this works with our minimum
	SizeLimitT		minimumSize = PhotoPrintPrefs::Singleton()->GetMinimumSize();

	if (minimumSize != limit_None) {
		//Get minimum dimensions (return is inches)
		double		minWidth;
		double		minHeight;
		PhotoItemProperties::SizeLimitToInches(minimumSize, minWidth, minHeight);
		SInt32		minSize = min(minWidth, minHeight) * kDPI;	// Convert inches to pixels

		// printable area (taking into account margins, etc)
		MRect		printableArea;
		EPrintSpec*	spec = mDocument->GetPrintRec();
		PhotoPrinter::CalculatePrintableRect(spec, &(mDocument->GetPrintProperties()), printableArea);

		ERect32		cellSize, unusedBottomPad;
		this->CalculateCellSize(inPageSize, outRows, outCols, cellSize, unusedBottomPad);

		// See if it fits. If not, try again (recursively) to fit fewer
		if (cellSize.Width() < minSize || cellSize.Height() < minSize)
			this->CalculateGrid(inPageSize, inCount - 1, outRows, outCols, outOrientation);
	}
} // CalculateGrid

//--------------------------------------------------------------
// DrawEmptyRect
//--------------------------------------------------------------
void
GridLayout::DrawEmptyRect(const ERect32& where, RGBColor inColor) {
	HORef<StGDeviceSaver> saveDevice;
	CGrafPtr	destPort;
	::GetPort((GrafPtr*)&destPort);
	StColorPortState	saveState ((GrafPtr)destPort);
	saveState.Normalize();
	::RGBForeColor(&inColor);

	::MoveTo(where.left, where.top);
	::LineTo(where.right, where.top);
	::LineTo(where.right, where.bottom);
	::LineTo(where.left, where.bottom);
	::LineTo(where.left, where.top);
}//end DrawEmptyRect

//--------------------------------------------------------------
// LayoutImages
//--------------------------------------------------------------
void
GridLayout::LayoutImages()
{	
	// sanity check:  make sure there are some images to layout
	if (mModel->GetCount() == 0) return;

	// First be sure the paper is switched optimally (also sets view size depending on mNumPages)
	this->AdjustDocumentOrientation();

	// get entire size of Document
	SInt32		docW = (SInt16)(mDocument->GetWidth() * mDocument->GetResolution());
	SInt32		docH = (SInt16)(mDocument->GetHeight() * mDocument->GetResolution());
	// and reduce height down to a single page size for layout purposes (width is always single)
	docH /= mNumPages;
	ERect32	pageSize (0, 0, docH, docW);

	ERect32 cellRect;
	ERect32 padRect;
	this->CalculateCellSize(pageSize, mRows, mColumns, cellRect, padRect);

	// layout the pages
	ERect32	pageBounds (pageSize);			
	PhotoIterator	iter (mModel->begin());
	for (SInt16 pageCount = 0; pageCount < mNumPages; ++pageCount) {
		LayoutPage(pageBounds, cellRect, iter);		
		if (iter == mModel->end())
			break;
		pageBounds.Offset(0, pageSize.Height()); // subsequent pages appear below, no horiz offset
		}//end for

	Assert_(iter == mModel->end()); // if we haven't processed all items, something is WRONG
}//end LayoutImages


//--------------------------------------------------------------
// LayoutPage
//--------------------------------------------------------------
void
GridLayout::LayoutPage(const ERect32& pageBounds, const ERect32& cellRect, PhotoIterator& iter) {
	do {
		for (SInt16 row = 0; row < GetRows(); ++row) {
			for (SInt16 col = 0; col < GetColumns(); ++col) {
				PhotoItemRef	item = *iter;
				MRect			itemBounds = item->GetNaturalBounds();
				MRect cellBounds (0, 0, cellRect.Height(), cellRect.Width());
				AlignmentGizmo::FitAndAlignRectInside(itemBounds, cellBounds, kAlignAbsoluteCenter, 
														itemBounds, EUtil::kDontExpand);
				itemBounds.Offset(pageBounds.left + (GetGutter() * col) + (cellRect.Width() * col),	
									pageBounds.top + (GetGutter() * row) + (cellRect.Height() * row));
				item->SetDest(itemBounds);

#ifdef DEBUG_LAYOUT
				{//evil debugging block
					cellBounds.Offset((pageBounds.left + (GetGutter() * col) + (cellRect.Width() * col)),	
										pageBounds.top + (GetGutter() * row) + (cellRect.Height() * row));
					ERect32 fullCell (cellBounds);

					DrawEmptyRect(fullCell);
				}//endif not printing
#endif

				++iter;
				if (iter == mModel->end())
					break;
			}//for all columns
			if (iter == mModel->end())
				break;
		}//end for all rows
		
	} while (false);
}//end LayoutPage
