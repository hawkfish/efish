/*
	File:		GridLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				GridLayout keeps multiple images in a best-fit grid

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 sep 2000		dml		LayoutImages must use BodyRect (offset for proper page), not PageBounds
		19 Sep 2000		drd		ResizeImage
		14 Sep 2000		drd		Even more respectful of maximum size
		13 sep 2000		dml		add header/footer support and annoyingware.  Fix Zoom caused bug in LayoutPage
		11 sep 2000		dml		a bit longer of a short-circuit exit at top of LayoutImages ensures view rects are updated
		07 sep 2000		dml		better respect of MaxBounds
		31 aug 2000		dml		layout must take into account xformed naturalbounds (rotation changes bounding)
		16 Aug 2000		drd		Be sure LayoutPage doesn't wipe out placeholder
		15 Aug 2000		drd		Removed AddItem (no need to override)
		18 jul 2000		dml		CalculateGrid uses desired-orientation internally (even if not yet set)
								also, adds 2 to sqrt call (make 7 force 3 cols)
		17 Jul 2000		drd		Make better use of max size (in LayoutPage, not CalculateCellSize)
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

/*
GridLayout
*/
GridLayout::GridLayout(HORef<PhotoPrintModel>& inModel)
	: Layout(inModel)
{
	mType = kGrid;
	mItemsPerPage = 0;
	mSizeCode = '****';
} // GridLayout

/*
~GridLayout
*/
GridLayout::~GridLayout()
{
} // ~GridLayout

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
	PhotoPrinter::CalculateBodyRect(spec, &(mDocument->GetPrintProperties()), printableArea); // at 72dpi

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
	if (mSizeCode == '****') {
		PhotoItemProperties::SizeLimitToInches(PhotoPrintPrefs::Singleton()->GetMinimumSize(), hMin, vMin);
	} else {
		PhotoUtility::GetSize(mSizeCode, hMin, vMin);
	}

	// convert inches to screen resolution
	SInt16		resolution = mDocument->GetResolution();
	hMin *= resolution;
	vMin *= resolution;
	
	// starting with page size, remove unavailable gutter
	SInt32		width = inPageSize.Width() - ((inCols - 1) * (this->GetGutter() * (kDPI / (double)resolution)));
	SInt32		height = inPageSize.Height() - ((inRows - 1) * (this->GetGutter() * (kDPI / (double)resolution)));

	//divide by rows, cols
	width /= inCols;
	height /= inRows;

	outCellSize.MakeEmpty();
	outCellSize.SetWidth(width);
	outCellSize.SetHeight(height);

	// Note that we don't care about maximum image size here, we are just allocating the grid

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
			outCols = std::sqrt(inCount + 2);		// was 1, but 7 needs to go to three cols
			outRows = (inCount + outCols - 1) / outCols;
			outOrientation = forcedOrientation;
			break;
	} // end switch

	// Now see if this works with our minimum
	SizeLimitT		minimumSize = PhotoPrintPrefs::Singleton()->GetMinimumSize();

	if (minimumSize != limit_None || mSizeCode != '****') {
		//Get minimum dimensions (return is inches)
		double		minWidth;
		double		minHeight;
		if (mSizeCode == '****') {
			PhotoItemProperties::SizeLimitToInches(minimumSize, minWidth, minHeight);
		} else {
			PhotoUtility::GetSize(mSizeCode, minWidth, minHeight);
		}
		minWidth *= kDPI;
		minHeight *= kDPI;

		// printable area (taking into account margins, etc)
		MRect		printableArea;
		EPrintSpec*	spec = mDocument->GetPrintRec();
		PhotoPrinter::CalculateBodyRect(spec, &(mDocument->GetPrintProperties()), printableArea); // at kDPI == 72!!

		// just before checking for goodness of cellsize, make sure pageSize reflects
		// our desired orientation
		ERect32 desiredPageSize (inPageSize);
		if (((outOrientation == kLandscape) && (desiredPageSize.Height() > desiredPageSize.Width())) ||
			((outOrientation == kPortrait) && (desiredPageSize.Width() > desiredPageSize.Height()))) {
				SInt32 temp (desiredPageSize.Width());
				desiredPageSize.SetWidth(desiredPageSize.Height());
				desiredPageSize.SetHeight(temp);

				swap(minWidth, minHeight); // seems better with this.  not rigorously validated, though
								
		}//endif need to swap width + height of desired rect to match desired orientation
		
		ERect32		cellSize, unusedBottomPad;
		this->CalculateCellSize(desiredPageSize, outRows, outCols, cellSize, unusedBottomPad);

		// See if it fits. If not, try again (recursively) to fit fewer
		if (cellSize.Width() < minWidth || cellSize.Height() < minHeight)
			this->CalculateGrid(desiredPageSize, inCount - 1, outRows, outCols, outOrientation);
	}
} // CalculateGrid

#ifdef DEBUG_LAYOUT
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
#endif

/*
LayoutImages {OVERRIDE}
*/
void
GridLayout::LayoutImages()
{	
	// sanity check:  make sure there are some images to layout
	if (mModel->GetCount() == 0) {
		mDocument->MatchViewToPrintRec(1);
		return;
	}//endif
		
	// First be sure the paper is switched optimally (also sets view size depending on mNumPages)
	this->AdjustDocumentOrientation();

	// get entire size of Document
	SInt32		docW = (SInt16)(mDocument->GetWidth() * mDocument->GetResolution());
	SInt32		docH = (SInt16)(mDocument->GetHeight() * mDocument->GetResolution());
	// and reduce height down to a single page size for layout purposes (width is always single)
	docH /= mNumPages;
	ERect32		pageSize (0, 0, docH, docW);

	ERect32		cellRect;
	ERect32		padRect;
	this->CalculateCellSize(pageSize, mRows, mColumns, cellRect, padRect);

	// setup locals for laying out the page.  full page rect, body rect
	ERect32	pageBounds (pageSize);			
	MRect		bodyRect;
	EPrintSpec* spec = mDocument->GetPrintRec();
	PhotoPrinter::CalculateBodyRect(spec, &(mDocument->GetPrintProperties()), bodyRect, mDocument->GetResolution()); // at curRes

	// layout the pages
	PhotoIterator	iter (mModel->begin());
	ERect32 bodyBounds (bodyRect);
	for (SInt16 pageCount = 0; pageCount < mNumPages; ++pageCount) {
		this->LayoutPage(bodyBounds, cellRect, iter);		
		if (iter == mModel->end())
			break;
		bodyBounds.Offset(0, pageSize.Height()); // subsequent pages appear below, no horiz offset
	}//end for

	Assert_(iter == mModel->end()); // if we haven't processed all items, something is WRONG
}//end LayoutImages

//--------------------------------------------------------------
// LayoutPage
//--------------------------------------------------------------
void
GridLayout::LayoutPage(const ERect32& inPageBounds, const ERect32& inCellRect, PhotoIterator& iter)
{
	double		hMax;
	double		vMax;

	if (mSizeCode == '****') {
		PhotoItemProperties::SizeLimitToInches(
			PhotoPrintPrefs::Singleton()->GetMaximumSize(), hMax, vMax);
	} else {
		PhotoUtility::GetSize(mSizeCode, hMax, vMax);
	}

	// convert inches to screen resolution
	hMax *= mDocument->GetResolution();
	vMax *= mDocument->GetResolution();
	double		maxDimension = max(hMax, vMax);

	MRect		maximum(0, 0, maxDimension, maxDimension);	// Use larger since we don't know orientation

	do {
		for (SInt16 row = 0; row < GetRows(); ++row) {
			for (SInt16 col = 0; col < GetColumns(); ++col) {
				PhotoItemRef	item = *iter;
				MRect			itemBounds = item->GetNaturalBounds();
				// scale it by the resolution to handle zooming
				RectScale(itemBounds, (double)mDocument->GetResolution() / (double)kDPI);

				MatrixRecord	rotator;
				if (!PhotoUtility::DoubleEqual(item->GetRotation(), 0.0)) {				
					::SetIdentityMatrix(&rotator);
					::RotateMatrix(&rotator, ::Long2Fix(item->GetRotation()), ::Long2Fix(itemBounds.MidPoint().h), 
																				::Long2Fix(itemBounds.MidPoint().v));
					::TransformRect(&rotator, &itemBounds, nil); // transform the bounds by the current rotation
					}//endif there is rotation				
				
				MRect			cellBounds (0, 0, inCellRect.Height(), inCellRect.Width());
				MRect			maxBounds (cellBounds);
				if (!maximum.IsEmpty())
					maxBounds *= maximum;
				// cellBounds is the grid; we need to layout inside this with user's choice of max
				MRect			imageBounds = maxBounds;

				// If we have a placeholder, make it as big as the cell
				if (item->IsEmpty())
					itemBounds = maxBounds;

				// Fit imageBounds into cellBounds.
				AlignmentGizmo::FitAndAlignRectInside(imageBounds, cellBounds, kAlignAbsoluteCenter, 
														imageBounds, EUtil::kDontExpand);
				// !!! note: If the max is 3*5, we don't change both dimensions. This is because
				// we'd probably have to crop.

				// Fit itemBounds (the full size of the image data) into imageBounds
				AlignmentGizmo::FitAndAlignRectInside(itemBounds, imageBounds, kAlignAbsoluteCenter, 
														itemBounds, EUtil::kDontExpand);
				itemBounds.Offset(inPageBounds.left + (GetGutter() * col) + (inCellRect.Width() * col),	
									inPageBounds.top + (GetGutter() * row) + (inCellRect.Height() * row));

				if (!PhotoUtility::DoubleEqual(item->GetRotation(), 0.0)) {				
					::SetIdentityMatrix(&rotator);
					::RotateMatrix(&rotator, ::Long2Fix(-1 * item->GetRotation()), ::Long2Fix(itemBounds.MidPoint().h), 
																			::Long2Fix(itemBounds.MidPoint().v));
					::TransformRect(&rotator, &itemBounds, nil);
					}//endif there is rotation so need to inverse xform the bounds
					
				item->SetDest(itemBounds);

				++iter;
				if (iter == mModel->end())
					break;
			}//for all columns
			if (iter == mModel->end())
				break;
		}//end for all rows
		
	} while (false);
}//end LayoutPage

/*
ResizeImage {OVERRIDE}
	Resizes an image within us -- we need to resize *all* our images
	Returns TRUE if there was an actual size change
*/
bool
GridLayout::ResizeImage(const OSType inCode, const FitT inFit, PhotoItemRef ioItemRef)
{
#pragma unused(inFit)	// !!!

	if (mSizeCode != inCode) {
		mSizeCode = inCode;		// We'll rely on layout being called

		// Get rid of the existing proxies
		PhotoIterator	iter;
		for (iter = mModel->begin(); iter != mModel->end(); iter++) {
			(*iter)->DeleteProxy();
		}
	}

	Str255		sizeText;
	OSType		oldCode = ioItemRef->GetDimensions(sizeText, mDocument->GetResolution(),
		PhotoPrintItem::si_OtherDimensions);
	return oldCode != inCode;
} // ResizeImage
