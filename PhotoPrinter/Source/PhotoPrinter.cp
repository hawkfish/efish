/*	PhotoPrinter.cp

	Contains:	Implementation of the base Printing Pane

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	20 june 2000	dml		moved BestFit into EUtil
	19 june	2000	dml		implement auto-rotate (at document level, useful only for multi-page docs)
	16 june 2000	dml		implement margin functionality!
*/


#include "PhotoPrinter.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintView.h"
#include "PhotoDrawingProperties.h"

#include <UState.h>

//-----------------------------------------------------
//
//-----------------------------------------------------
PhotoPrinter::PhotoPrinter(PhotoPrintDoc* inDoc, 
							PhotoPrintView*	inView, 
							EPrintSpec*	inSpec,
							PrintProperties* inProps,
							GrafPtr inPort) 
	: mDoc (inDoc)
	, mView (inView)
	, mModel (inDoc->GetView()->GetModel())
	, mPrintSpec (inSpec)
	, mResFile (::CurResFile())
	, mProps (inProps)
	, mPrinterPort		(inPort)
	, mOriginTop (0)
	, mOriginLeft (0)
	, mRotation (mProps->GetRotation())
{
	SInt16 hRes;
	SInt16 vRes;
	
	mPrintSpec->GetResolutions(vRes, hRes);
	mResolution = vRes;

	// modifies mRotation, which is used in CalculateDocumentDimensionsInPixels
	ApplyRotation(); 
}
					
					


//-----------------------------------------------------
//
//-----------------------------------------------------
PhotoPrinter::~PhotoPrinter		(void) 
{
}



#pragma mark -

//-----------------------------------------------------
//ApplyMargins
//-----------------------------------------------------
void			
PhotoPrinter::ApplyMargins		(MRect& ioRect)
{
	switch (mProps->GetMarginType()) {
		case PrintProperties::kMinimalMargins: {
			// nothing to do for minimal margins!
			}//case
			break;
		case PrintProperties::kHorizontalSymmetric: 
		case PrintProperties::kVerticalSymmetric: 
		case PrintProperties::kFullSymmetric: 
			ApplySymmetricMargins(ioRect);
			break;
		case PrintProperties::kCustom:
			ApplyCustomMargins(ioRect);
			break;
		}//end
}






//-----------------------------------------------------
//ApplyCustomMargins
//-----------------------------------------------------
void
 PhotoPrinter::ApplyCustomMargins		(MRect& ioRect) {
	float top;
	float left;
	float bottom;
	float right;
	
	SInt16	vRes;
	SInt16 	hRes;
	
	// start with the entire page
	mPrintSpec->GetPaperRect(ioRect);
	// find out what the margins are
	mProps->GetMargins(top, left, bottom, right);

	// convert the margins from inches (incoming) to pixels
	// by asking the printSpec what its resolution is
	// (we always expect square resolution and use vRes)
	mPrintSpec->GetResolutions(vRes, hRes);
	top *= vRes;
	left *= vRes;
	bottom *= vRes;
	right *= vRes;
	
	// shrink the paper size by the sum of the margins (h, v)
	ioRect.Inset(left + right, top + bottom);
	// offset according to top, left
	ioRect.Offset(left, top);
	
 }//end ApplyCustomMargins
//-----------------------------------------------------
//ApplyRotation
//-----------------------------------------------------
void
PhotoPrinter::ApplyRotation() {
	bool	bNeedToRotate (false);
	
	switch (mProps->GetRotationBehavior()) {
		case PrintProperties::kNeverRotate:
			break;
		case PrintProperties::kAlwaysRotate: {
			bNeedToRotate = true;
			}//case
			break;
		case PrintProperties::kAutoRotate: {
			SInt16 vanilla (CountPages());
			SInt16	rotated (CountPages(true));
			if (rotated < vanilla)
				bNeedToRotate = true;
			}//case
			break;
		}//switch

	if (bNeedToRotate) {
		switch (mProps->GetRotation()) {
			case PrintProperties::kNoRotation:
				mRotation = PrintProperties::k90CWRotation;
				break;
			case PrintProperties::k90CWRotation:
				mRotation = PrintProperties::k180Rotation;
				break;
			case PrintProperties::k180Rotation:
				mRotation = PrintProperties::k270CWRotation;
				break;			
			case PrintProperties::k270CWRotation:
				mRotation = PrintProperties::kNoRotation;
				break;
			}//switch
		}//endif need to apply that rotation
	}//end ApplyRotation



//-----------------------------------------------------
//ApplySymmetricMargins
//-----------------------------------------------------
 void	
 PhotoPrinter::ApplySymmetricMargins 	(MRect& ioRect)
 {
	MRect paperRect;
	mPrintSpec->GetPaperRect(paperRect);
	
	short dTop (ioRect.top - paperRect.top);
	short dBottom (paperRect.bottom - ioRect.bottom);
	short dLeft (ioRect.left - paperRect.left);
	short dRight (paperRect.right - ioRect.right);
	
	switch (mProps->GetMarginType()) {
		case PrintProperties::kHorizontalSymmetric: {
			if (dLeft > dRight) {
				ioRect.right -= (dLeft - dRight);
				}//endif need to bring right edge in to match left
			else {
				ioRect.left += (dRight - dLeft);
				}//else need to bring left edge in to match right
			}//case
			break;
		case PrintProperties::kVerticalSymmetric: {
			if (dTop > dBottom) {
				ioRect.bottom -= (dTop - dBottom);
				}//endif need to bring bottom up to match top
			else {
				ioRect.top += (dBottom - dTop);
				}// else need to bring top down to match bottom
			}//case
			break;
		case PrintProperties::kFullSymmetric: {
			// repeat the steps above!
			if (dLeft > dRight) {
				ioRect.right -= (dLeft - dRight);
				}//endif need to bring right edge in to match left
			else {
				ioRect.left += (dRight - dLeft);
				}//else need to bring left edge in to match right
			if (dTop > dBottom) {
				ioRect.bottom -= (dTop - dBottom);
				}//endif need to bring bottom up to match top
			else {
				ioRect.top += (dBottom - dTop);
				}// else need to bring top down to match bottom
			}//case
			break;
		}//end switch
 }//end ApplySymmetricMargins;


//-----------------------------------------------------
//GetDocumentDimensions.  Handles all 90 degree rotations correctly
//-----------------------------------------------------
void
PhotoPrinter::GetDocumentDimensionsInPixels(SInt16& outHeight, SInt16& outWidth) {

	if (mProps->GetFit()) {
		MRect printableArea (GetPrintableRect());
		outHeight = printableArea.Height();
		outWidth = printableArea.Width();
		}//endif fit-to-page
	else {
		//ask doc for its bounds
		outHeight = (SInt16)(mDoc->GetHeight() * mDoc->GetResolution());
		outWidth = (SInt16)(mDoc->GetWidth() * mDoc->GetResolution());

		// if we are rotating, switch width and height;
		switch (mRotation) {
			case PrintProperties::k90CWRotation:
			case PrintProperties::k270CWRotation: 
				{
					SInt16 temp (outHeight);
					outHeight = outWidth;
					outWidth = temp;
				}
				break;
			}//end switch
		}//else true-size
}//end GetDocumentDimensions




//-----------------------------------------------------
//
//-----------------------------------------------------
MRect	
PhotoPrinter::GetPrintableRect	(void)
{
	MRect printableArea;
	
	// start with printable area from print rec
	mPrintSpec->GetPageRect(printableArea);

	ApplyMargins(printableArea);

	// overlap isn't considered here, since it only comes into
	// play with multiple panels.  The Printable Area isn't affected.  
	// @see CountPanels
	// @see ScrollToPanels
	
	return printableArea;
}






SInt32
PhotoPrinter::InchesToPrintPixels(float inUnits) 
{
	SInt16 hRes;
	SInt16 vRes;
	
	mPrintSpec->GetResolutions(vRes, hRes);
	return inUnits * vRes;
}//end


#pragma mark -




							
	


					 
//-----------------------------------------------------
//CountPages
//-----------------------------------------------------
SInt16	
PhotoPrinter::CountPages(bool bRotate)
{
	
	MRect pageSize (GetPrintableRect());
	SInt16	docWidth;
	SInt16 	docHeight;
	if (bRotate)
		GetDocumentDimensionsInPixels(docWidth, docHeight);
	else				
		GetDocumentDimensionsInPixels(docHeight, docWidth);
	
	SInt16	hCount (0);
	SInt16	vCount (0);
	
	hCount = pageSize.Width() / docWidth;
	if (pageSize.Width() * hCount < docWidth)
		++hCount;
		
	vCount = pageSize.Height() / docHeight;
	if (pageSize.Height() * vCount < docHeight)
		++vCount;
	
	return (vCount * hCount);
}




//-----------------------------------------------------
//CountPanels
//-----------------------------------------------------
void		
PhotoPrinter::CountPanels(UInt32			&outHorizPanels,
						 UInt32				&outVertPanels)
{
	SInt16 hPixels;
	SInt16 vPixels;
	
	SInt32	overlapPixels (InchesToPrintPixels(mProps->GetOverlap()));
	
	GetDocumentDimensionsInPixels(vPixels, hPixels);
	MRect availablePaper (GetPrintableRect());

	// see if we're going across multiple pages.
	// if we are, decrease available area by overlap amount
	if (availablePaper.Width() < hPixels)
		availablePaper.SetWidth(availablePaper.Width() - overlapPixels);
	if (availablePaper.Height() < vPixels)
		availablePaper.SetHeight(availablePaper.Height() - overlapPixels);
		

	// do that stupid integer division + look at the remainder calculation
	outHorizPanels = hPixels / availablePaper.Width();
	if (outHorizPanels * availablePaper.Width() < hPixels)
		++outHorizPanels;
		
	outVertPanels = vPixels / availablePaper.Height();
	if (outVertPanels * availablePaper.Height() < vPixels)
		++outVertPanels;
}//CountPanels							


//-----------------------------------------------------
//DrawSelf
//-----------------------------------------------------

void	
PhotoPrinter::DrawSelf			(void)
{
	GrafPtr	curPort;
	::GetPort(&curPort);
	GDHandle curDevice;
	curDevice =	::GetGDevice();

	// make the copy model
	HORef<PhotoPrintModel> printingModel = new PhotoPrintModel(*mModel);
	// and setup the restoration of the drawing flags
	MRestoreValue<PhotoDrawingProperties> saveProps (printingModel->GetDrawingProperties());
	printingModel->GetDrawingProperties().SetPrinting(true);

	// map it
	MatrixRecord mat;
	MapModelForPrinting(&mat, printingModel);


	printingModel->Draw(&mat,
						(CGrafPtr)curPort,
						curDevice);
}




//-----------------------------------------------------
//MapModelForPrinting.
//
// some of the work is done directly on the model (the items)
// (translation, scaling).  
// However, Rotation/skew/weird flipping, etc
// is done via a matrix, which is multiplied in to each site 
// when the site is drawn.
// Here we perform the item operations, and create the matrix for use later
//-----------------------------------------------------
void
PhotoPrinter::MapModelForPrinting(MatrixRecord* ioMatrix, PhotoPrintModel* inModel) {
	// at the moment, we are not supporing any rotational/flip effects
	::SetIdentityMatrix(ioMatrix);

	SInt16 docHeight;
	SInt16 docWidth;
	GetDocumentDimensionsInPixels(docHeight, docWidth);

	// get the printable area  (typically larger if resolution > 72 dpi)
	MRect pageBounds;
	mPrintSpec->GetPageRect(pageBounds);

	// get the view dimensions
	// these are the (base) coordinate system of the model
	SDimension32 viewSize;
	mView->GetImageSize(viewSize);
	MRect imageRect (mOriginTop,mOriginLeft,1,1);
	imageRect.SetWidth(viewSize.width);
	imageRect.SetHeight(viewSize.height);
	
	inModel->MapItems(imageRect, pageBounds);
	}//end CreateMapping







	
//-----------------------------------------------------
//ScrollToPanel
//
// similar to LView's, but taking into account overlap
//-----------------------------------------------------
Boolean
PhotoPrinter::ScrollToPanel(const PanelSpec	&inPanel)
{
	Boolean panelInImage (false);
	

	UInt32	horizPanelCount;
	UInt32	vertPanelCount;
	
	MRect frameSize (GetPrintableRect());
	
	CountPanels(horizPanelCount, vertPanelCount);
	if ((inPanel.horizIndex <= horizPanelCount) &&
		(inPanel.vertIndex <= vertPanelCount)) {

		// figure out the offset without consideration for overlap
		SInt32 horizPos = (SInt32) (frameSize.Width() * (inPanel.horizIndex - 1));
		SInt32 vertPos = (SInt32) (frameSize.Height() * (inPanel.vertIndex - 1));

		SInt32	overlapPixels (InchesToPrintPixels(mProps->GetOverlap()));
		horizPos -= (overlapPixels * (inPanel.horizIndex - 1));
		vertPos -= (overlapPixels * (inPanel.vertIndex - 1));

		mOriginLeft = horizPos;
		mOriginTop = vertPos;

		panelInImage = true;
	}

return panelInImage;	
}//end ScrollToPanel
						
//-----------------------------------------------------
//SetupPrintRecordToMatchProperties
//-----------------------------------------------------
void
PhotoPrinter::SetupPrintRecordToMatchProperties(EPrintSpec* inRecord, PrintProperties* inProps) 
{
	// learn about resolutions available
	SInt16 minX, minY, maxX, maxY;
	inRecord->WalkResolutions( minX,  minY,  maxX,  maxY);
	// learn about current resolution
	SInt16 curVRes, curHRes;
	inRecord->GetResolutions(curVRes, curHRes);
	
	if (inProps->GetHiRes()) {
		if (curVRes != maxY) 
			inRecord->SetResolutions(maxY, maxX);
		}//endif best resolution
	else {
		if (curVRes != minY)
			inRecord->SetResolutions(minY, minX);
		}//else draft resolution
	
}//end SetupPrintRecordToMatchProperties


