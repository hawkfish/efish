/*
	File:		GridLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				GridLayout keeps multiple images in a best-fit grid

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
*/
void
GridLayout::AdjustDocumentOrientation(SInt16 /*numPages*/)
{

	OSType bestOrientation;
	mItemsPerPage = MaxItemsPerPage(mMaxRows, mMaxCols, bestOrientation);

	mNumPages = mModel->GetCount() / mItemsPerPage;
	if (mNumPages * mItemsPerPage < mModel->GetCount()) 
		++mNumPages; 


	EPrintSpec* spec = (EPrintSpec*)mDocument->GetPrintRec();
	mOrientation = spec->GetOrientation();
	if (mOrientation != bestOrientation) {
		mOrientation = bestOrientation;
		mDocument->GetPrintRec()->SetOrientation(mOrientation);
		}//endif need to switch orientation
	mDocument->MatchViewToPrintRec(mNumPages); // do this anyway, since changes according to #pages
} // AdjustDocumentOrientation



//--------------------------------------------------------------
// CalculateCellSizeForPage
//--------------------------------------------------------------
void		
GridLayout::CalculateCellSize(const ERect32& pageSize, ERect32& outCellSize, ERect32& outUnusedBottomPad) {

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
	SInt32 width (pageSize.Width() - ((mColumns - 1) * GetGutter()));
	SInt32 height (pageSize.Height() - ((mRows - 1) * GetGutter()));

	CalculateRowsCols(pageSize, mRows, mColumns);

	//divide by rows, cols
	width /= GetColumns();
	height /= GetRows();

	outCellSize.MakeEmpty();
	outCellSize.SetWidth(width);
	outCellSize.SetHeight(height);

	ERect32 maximum (0, 0, vMax, hMax);
	if (!maximum.IsEmpty())
		outCellSize *= maximum;
	
	
	//ok.  now that we know the cell size, determine how much space is unused at the bottom of the page
	outUnusedBottomPad.MakeEmpty();
	outUnusedBottomPad.SetWidth(pageSize.Width());
	outUnusedBottomPad.SetHeight(pageSize.Height() - (outCellSize.Height() * GetRows()) - ((GetRows() - 1) * GetGutter()));
	outUnusedBottomPad.Offset(0, pageSize.bottom - outUnusedBottomPad.Height());

}//end CalculateCellSize



//--------------------------------------------------------------
// CalculateRowsCols
//--------------------------------------------------------------
void
GridLayout::CalculateRowsCols(const ERect32& pageSize, SInt16& outRows, SInt16& outCols) {
	SInt16 count (mModel->GetCount());
	do {
		// simplest case first:  single item
		if (count == 1) {
			outRows = outCols = 1;
			break;
			}//endif only one		

		// next simplest case:  full page
		if (count >= mItemsPerPage) {
			outRows = mRows;
			outCols = mColumns;
			break;
			}//endif simple case, full page, use settings determined for maximum
	
		// everything else is slightly more complicated.
		// count how many landscape, portrait
		UInt32		l = CountOrientation(kLandscape);
		UInt32		p = CountOrientation(kPortrait);
		
		// determine our row/col bias based on that count and our orientation
		OSType bias;
		if (l > p) {
			if (GetOrientation() == kLandscape)
				bias = kBiasCols;
			else
				bias = kBiasRows;
			}//endif more landscape than portrait
		else {
			if (GetOrientation() == kPortrait)
				bias = kBiasRows;
			else
				bias = kBiasCols;
			}//else equal or more portrait

		double root (std::sqrt(count));
		double aspect ((double)pageSize.Height() / (double)pageSize.Width());

		if (bias == kBiasRows) {
			outRows = (root * aspect) + 0.5;
			outCols = count / outRows;
			if (outCols * outRows < count)
				++outCols;
			}//endif kBiasRows
		else {
			outCols = (root * aspect) + 0.5;
			outRows = count / outCols;
			if (outCols * outRows < count)
				++outRows;
			}//else kBiasCols			

		} while (false);

	
		
	}//end CalculateRowsCols




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
GridLayout::LayoutImages() {
	
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
	CalculateCellSize(pageSize, cellRect, padRect);
	
	DrawEmptyRect(padRect);


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

				{//evil debugging block
					cellBounds.Offset((pageBounds.left + (GetGutter() * col) + (cellRect.Width() * col)),	
										pageBounds.top + (GetGutter() * row) + (cellRect.Height() * row));
					ERect32 fullCell (cellBounds);

					DrawEmptyRect(fullCell);
					}//endif not printing
				
				++iter;
				if (iter == mModel->end())
					break;
				}//for all columns
			if (iter == mModel->end())
				break;
			}//end for all rows
			
		
		} while (false);
	}//end LayoutPage



//--------------------------------------------------------------
// MaxItemsPerPage
// --------------------------------------------------------------
SInt16		
GridLayout::MaxItemsPerPage(SInt16& outRows, SInt16& outCols, OSType& outOrientation) {

	SizeLimitT minimumSize (PhotoPrintPrefs::Singleton()->GetMinimumSize());

	// simple check first:  no limit case
	if (minimumSize == limit_None) {
		outRows = outCols = 10;
		return mModel->GetCount();
		}//endif

	//otherwise, we must do some calculations...
	//Get minimum dimensions (return is inches)
	double minWidth;
	double minHeight;
	PhotoItemProperties::SizeLimitToInches(minimumSize, minWidth, minHeight);

	// printable area (taking into account margins, etc)
	MRect		printableArea;
	EPrintSpec* spec = (EPrintSpec*)mDocument->GetPrintRec();
	PhotoPrinter::CalculatePrintableRect(spec, &(mDocument->GetPrintProperties()), printableArea);

	// resolution
	SInt16		vRes;
	SInt16		hRes;	// Not really used
	spec->GetResolutions(vRes, hRes);

	// multiply minimum sizes by resolution (we use vRes exclusively) to convert to pixels
	minWidth *= vRes;
	minHeight *= vRes;

	// add gutter (which comes as pixels at 72 dpi), since this will effectively enlarge each "cell"
	double gutterAtPrintResolution 	(GetGutter() * vRes / kDPI);
	minWidth += gutterAtPrintResolution;
	minHeight += gutterAtPrintResolution;

	SInt16 hCount (printableArea.Width() / minWidth);
	SInt16 vCount (printableArea.Height() / minHeight);	

	SInt16 hRotated (printableArea.Height() / minHeight);
	SInt16 vRotated (printableArea.Width() / minWidth);

	if (hCount * vCount >= hRotated * vRotated) {
		outRows = vCount; 
		outCols = hCount;
		outOrientation = GetOrientation();
		return hCount * vCount;
		}
	else {
		outRows = vRotated;
		outCols = hRotated;
		outOrientation = (GetOrientation() == kLandscape) ? kPortrait : kLandscape;
		return hRotated * vRotated;
		}//else
} // MaxItemsPerPage

