/*
	File:		PhotoPrinter.cp

	Contains:	Implementation of the base Printing Pane

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	09 mar 2001		dml		bug 34, bug 58.  changes in GridLayout and elsewhere necessitate
							MapModel actually setting the matrix to be xlation to bodyrect origin
	02 feb 2001		dml		add DrawTestPage
	19 jan 2001		dml		whoops.  ApplyMinimal restored to functioning
	19 jan 2001		dml		ApplyMargins invoked w/ wrong rectangle:  should be paper, not page (?)
	14 dec 2000		dml		change CountPages, CountPanels, ScrollToPanel to handle header/footers
	11 Oct 2000		drd		Use new IsInSession method in CalculatePrintableRect
	21 Sep 2000		drd		DrawSelf checks for cTempMemErr, not just memFullErr
	19 Sep 2000		drd		Switch to using Color_White since sWhiteRGB is going away
	14 sep 2000		dml		GetDocumentDimensionsInPixels removes header/footer
	13 sep 2000		dml		add support for Header + Footer.  re-alphabetized funcs
	24 Jul 2000		drd		DrawSelf ignores bands unless we're using alternate printing
	24 jul 2000		dml		using app prefs alternate
	21 jul 2000		dml		added banded printing.
	19 jul 2000		dml		MapModelForPrinting calculates PanelRect, which is passed to Model::Draw as clip
	17 jul 2000		dml		ApplyMinimalMargins must offset to PageRect origin
	14 Jul 2000		drd		Use new RotationBehaviorT constants
	14 jul 2000		dml		SetupPrintRecordToMatchProperties calls Validate
	14 jul 2000		dml		most instances of GetPageRect replaced with GetPrintableRect
	13 jul 2000		dml		GetDocumentDimensionsInPixels uses mResolution, functioning multi-page
	11 jul 2000		dml		add CalculatePrintableRect
	06 Jul 2000		drd		If we get memFullErr, don't use alternate printing
	26 Jun 2000		drd		Use double, not float
	21 june 2000 	dml		add AlternatePrinting
	20 june 2000	dml		moved BestFit into EUtil
	19 june	2000	dml		implement auto-rotate (at document level, useful only for multi-page docs)
	16 june 2000	dml		implement margin functionality!
*/

#include "PhotoPrinter.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintView.h"
#include "PhotoDrawingProperties.h"
#include "ERect32.h"
#include "MNewRegion.h"
#include <UState.h>
#include "PhotoUtility.h"
#include "PhotoPrintPrefs.h"
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
//ApplyHeaderFooter
//-----------------------------------------------------
void			
PhotoPrinter::ApplyHeaderFooter(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props)
{
	SInt16 hRes;
	SInt16 vRes;
	spec->GetResolutions(vRes, hRes);

	ioRect.top += props->GetHeader() * vRes;
	ioRect.bottom -= props->GetFooter() * vRes;
}//end ApplyHeaderFooter

//-----------------------------------------------------
//ApplyMargins
//  incoming should be paper rect from the print record
//-----------------------------------------------------
void			
PhotoPrinter::ApplyMargins		(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props)
{
	switch (props->GetMarginType()) {
		case PrintProperties::kMinimalMargins:
			ApplyMinimalMargins(ioRect, spec, props);
			break;
		case PrintProperties::kHorizontalSymmetric: 
		case PrintProperties::kVerticalSymmetric: 
		case PrintProperties::kFullSymmetric: 
			ApplySymmetricMargins(ioRect, spec, props);
			break;
		case PrintProperties::kCustom:
			ApplyCustomMargins(ioRect, spec, props);
			break;
		}//end
}


//-----------------------------------------------------
//ApplyCustomMargins
//-----------------------------------------------------
void
 PhotoPrinter::ApplyCustomMargins		(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props) {
	double	top;
	double	left;
	double	bottom;
	double	right;
	
	SInt16	vRes;
	SInt16 	hRes;
	
	// start with the entire page
	spec->GetPaperRect(ioRect);
	// find out what the margins are
	props->GetMargins(top, left, bottom, right);

	// convert the margins from inches (incoming) to pixels
	// by asking the printSpec what its resolution is
	// (we always expect square resolution and use vRes)
	spec->GetResolutions(vRes, hRes);
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
//ApplyMinimalMargins
//-----------------------------------------------------
void
PhotoPrinter::ApplyMinimalMargins(MRect& ioRect, EPrintSpec* spec, const PrintProperties* /*props*/) 
 {
	// minimal margins are the printable area (page rect)
	MRect pageRect;
	spec->GetPageRect(pageRect);
	pageRect.Offset(-ioRect.left, -ioRect.top);
	ioRect = pageRect;
	}//end AlignMinimalMargins


//-----------------------------------------------------
//ApplyRotation
//-----------------------------------------------------
void
PhotoPrinter::ApplyRotation() {
	bool	bNeedToRotate (false);
	
	switch (mProps->GetRotationBehavior()) {
		case PrintProperties::kForceLandscape:		// ??? was kNeverRotate
			break;
		case PrintProperties::kForcePortrait: {		// ??? was kAlwaysRotate
			bNeedToRotate = true;
			}//case
			break;
		case PrintProperties::kPickBestRotation: {
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
 PhotoPrinter::ApplySymmetricMargins 	(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props)
 {
	MRect pageRect;
	spec->GetPageRect(pageRect);
	
	// calculate the minimal margins
	short dTop (pageRect.top - ioRect.top);
	short dBottom (ioRect.bottom - pageRect.bottom);
	short dLeft (pageRect.left - ioRect.left);
	short dRight (ioRect.right - pageRect.right);

	// start off w/ the minimal margins	(pagerect) BUT
	// first offset pageRect to deal w/ PP's printing goofiness
	pageRect.Offset(-ioRect.left, -ioRect.top);
	ioRect = pageRect;

	//  and adjust accordingly for the symmetry
	switch (props->GetMarginType()) {
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


#pragma mark -

//-----------------------------------------------------
//CalculateBodyRect
//-----------------------------------------------------
void		
PhotoPrinter::CalculateBodyRect(EPrintSpec* inSpec,
								const PrintProperties* inProps,
								MRect& outRect,
								SInt16 outDPI) {

	// figure out the raw printable page
	CalculatePrintableRect(inSpec, inProps, outRect, outDPI);

	// subtract the header + footer
	outRect.top += inProps->GetHeader() * outDPI;
	outRect.bottom -= inProps->GetFooter() * outDPI;
	
	// theoretically possible for headers and footers to leave no body space
	if (outRect.bottom < outRect.top)
		outRect.bottom = outRect.top;

}//end CalculatePrintableRect


//-----------------------------------------------------
//CalculateHeaderRect
//-----------------------------------------------------
void	
PhotoPrinter::CalculateHeaderRect(EPrintSpec* inSpec,
								const PrintProperties* inProps,
								MRect& outRect,
								SInt16 outDPI) {

	CalculatePrintableRect(inSpec, inProps, outRect, outDPI);
	outRect.bottom = outRect.top + (inProps->GetHeader() * outDPI);
}//end CalculateHeaderRect
		

//-----------------------------------------------------
//CalculateFooterRect
//-----------------------------------------------------
void 	
PhotoPrinter::CalculateFooterRect(EPrintSpec* inSpec,
							const PrintProperties* inProps,
							MRect& outRect,
							SInt16 outDPI){									

	CalculatePrintableRect(inSpec, inProps, outRect, outDPI);
	outRect.top = outRect.bottom - (inProps->GetFooter() * outDPI);

}//end CalculateFooterRect



//-----------------------------------------------------
//CalculatePaperRect
// includes unprintable area
//-----------------------------------------------------
void		
PhotoPrinter::CalculatePaperRect(EPrintSpec* inSpec,
								const PrintProperties* /*inProps*/,
								MRect& outRect,
								SInt16 outDPI)
{
	HORef<StPrintSession> possibleSession;
	if (inSpec != nil && !inSpec->IsInSession())
		possibleSession = new StPrintSession(*inSpec);

	// start with paper from print rec
	inSpec->GetPaperRect(outRect);

	SInt16 hRes;
	SInt16 vRes;
	inSpec->GetResolutions(vRes, hRes);
	
	RectScale(outRect, (double)outDPI / (double)vRes);
}//end CalculatePaperRect



//-----------------------------------------------------
//CalculatePrintableRect
// *Entire printable area, including header/footer*
//-----------------------------------------------------
void		
PhotoPrinter::CalculatePrintableRect(EPrintSpec* inSpec,
									const PrintProperties* inProps,
									MRect& outRect,
									SInt16 outDPI)
{
	HORef<StPrintSession> possibleSession;
	if (inSpec != nil && !inSpec->IsInSession())
		possibleSession = new StPrintSession(*inSpec);

	// start with entire paper
	inSpec->GetPaperRect(outRect);

	ApplyMargins(outRect, inSpec, inProps);

	SInt16 hRes;
	SInt16 vRes;
	inSpec->GetResolutions(vRes, hRes);
	
	RectScale(outRect, (double)outDPI / (double)vRes);
}//end CalculatePrintableRect

					 
//-----------------------------------------------------
//CountPages
//-----------------------------------------------------
SInt16	
PhotoPrinter::CountPages(bool bRotate)
{
	
//orig	MRect pageSize (GetPrintableRect());
	MRect pageSize;
	CalculatePrintableRect(mPrintSpec, mProps, pageSize, mResolution);

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

//orig	MRect availablePaper (GetPrintableRect());
	MRect availablePaper;
	CalculatePrintableRect(mPrintSpec, mProps, availablePaper, mResolution);

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
//DrawFooter
//-----------------------------------------------------
void
PhotoPrinter::DrawFooter() {
	MRect bounds;	
	CalculateFooterRect(mPrintSpec, mProps, bounds, mResolution);

	::TextFont(mDoc->GetProperties().GetFontNumber());
	SInt16 unscaledFontSize (mDoc->GetProperties().GetFontSize());
	::TextSize(unscaledFontSize * ((double)mResolution / 72.0));

	MPString footer ((mDoc->GetProperties().GetFooter()));	
	UTextDrawing::DrawWithJustification(footer.Chars(), ::StrLength(footer), bounds, teJustCenter, true);	
}//end DrawFooter


//-----------------------------------------------------
//DrawHeader
//-----------------------------------------------------
void
PhotoPrinter::DrawHeader() {
	MRect bounds;	
	CalculateHeaderRect(mPrintSpec, mProps, bounds, mResolution);

	::TextFont(mDoc->GetProperties().GetFontNumber());
	SInt16 unscaledFontSize (mDoc->GetProperties().GetFontSize());
	::TextSize(unscaledFontSize * ((double)mResolution / 72.0));

	MPString header (mDoc->GetProperties().GetHeader());	
	UTextDrawing::DrawWithJustification(header.Chars(), ::StrLength(header), bounds, teJustCenter, true);	
}//end DrawHeader


//-----------------------------------------------------
//DrawTestPage
//-----------------------------------------------------
void
PhotoPrinter::DrawTestPage() {
	MRect frame;
	short lineWidth (10);
	
	static const RGBColor greys[10] = {
		{65535/32000,65535/32000,65535/32000},
		{65535/16000,65535/16000,65535/16000},
		{65535/8000,65535/8000,65535/8000},
		{65535/4000,65535/4000,65535/4000},
		{65535/2000,65535/2000,65535/2000},
		{65535/1000,65535/1000,65535/1000},
		{65535/500,65535/500,65535/500},
		{65535/100,65535/100,65535/100},
		{65535/50,65535/50,65535/50},
		{65535/2,65535/2,65535/2}};
	static const RGBColor colors[10] = {
		{65535, 0, 0},
		{0, 65535, 0},
		{0, 0, 65535},
		{32767, 0, 0},
		{0, 32767, 0},
		{0, 0, 32767},
		{16384, 0, 0},
		{0, 16384, 0},
		{0, 0, 16384},
		{16384,0,16384}};

	CalculatePrintableRect(mPrintSpec, mProps, frame, mResolution);
	
	short count (0);
	while ((frame.Width() > 10) && (frame.Height() > 10)) {
		if (count % 10 == 0)
			RGBForeColor(&colors[count / 10]);
		else
			RGBForeColor(&greys[count % 10]);
		::PenSize(lineWidth/2, lineWidth/2);
		::FrameRect(&frame);
		frame.Inset(lineWidth, lineWidth);
		++count;
		}//while
	

}//end DrawTestPage


//-----------------------------------------------------
//DrawSelf
//-----------------------------------------------------

static bool bDrawTestPage (false);

void	
PhotoPrinter::DrawSelf(void)
{
	if (bDrawTestPage) {
		DrawTestPage();
		return;
		}//endif only drawing the test page

	// assumption:  curport/device *now* is printer (whether actual or preview)
	GrafPtr	printerPort;
	::GetPort(&printerPort);
	GDHandle printerDevice;
	printerDevice =	::GetGDevice();

	// make the copy model
	HORef<PhotoPrintModel> printingModel = new PhotoPrintModel(*mModel);
	// and setup the restoration of the drawing flags
	MRestoreValue<PhotoDrawingProperties> saveProps (printingModel->GetDrawingProperties());
	printingModel->GetDrawingProperties().SetPrinting(true);

	// map it
	MatrixRecord mat;
	MRect panelBounds;
	MapModelForPrinting(&mat, printingModel, panelBounds);

	MRect		pageBounds;
	pageBounds = GetPrintableRect();
		
	SInt32		bandSize;
	PhotoPrintPrefs*	prefs = PhotoPrintPrefs::Singleton();
	if (prefs->GetAlternatePrinting() && prefs->GetBandedPrinting()) {
		bandSize = 32; // reasonable hardwired band height
		}//endif
	else
		bandSize = pageBounds.Height();
	MRect band (pageBounds);

	band.SetHeight(bandSize);

	// we might be drawing offscreen first (alternate printing)
	HORef<LGWorld>	possibleOffscreen;
	try {
		if (prefs->GetAlternatePrinting()) {
			possibleOffscreen = new LGWorld(band, 32, useTempMem);	//localcoords, truecolor, systemheap
			}//endif alternate printing selected
		}//try
	catch (LException e) {
		// Swallow out of memory
		if (e.GetErrorCode() != memFullErr && e.GetErrorCode() != cTempMemErr)
			throw;
		}//catch
	
	// draw header
	if (!PhotoUtility::DoubleEqual(mProps->GetHeader(), 0.0))
		DrawHeader();

	// draw body
	MNewRegion panelR;
	CGrafPtr 	port 	(possibleOffscreen ? possibleOffscreen->GetMacGWorld() : printerPort);
	GDHandle		device	(possibleOffscreen ? ::GetGWorldDevice(possibleOffscreen->GetMacGWorld()) : printerDevice);
	for (SInt16 line = 0; line <= pageBounds.Height() / bandSize; ++line) {
		panelR = band;
		InnerDrawLoop(printingModel, possibleOffscreen, band,
						&mat, port, device, panelR, printerPort);
				
		band.Offset(0, bandSize);
		band *= panelBounds; // last band may extend past, so clamp to panel bounds

		if (possibleOffscreen) {
			EraseOffscreen(possibleOffscreen);
			possibleOffscreen->SetBounds(band); // only sets origin, not rect
			}//endif offscreen to update
		
		}//end for all bands
	
	//draw footer
	if (!PhotoUtility::DoubleEqual(mProps->GetFooter(), 0.0))
		DrawFooter();	
	
}//end DrawSelf

/*
EraseOffscreen
*/
void
PhotoPrinter::EraseOffscreen(LGWorld* pGW) {
	pGW->BeginDrawing();

	::RGBBackColor(&Color_White);
	MRect bounds;
	pGW->GetBounds(bounds);
	::EraseRect(&bounds);

	pGW->EndDrawing();
} // EraseOffscreen
	

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
		outHeight 	= mDoc->GetHeight() * mResolution; // dimensions from doc are in inches.
		outWidth 	= mDoc->GetWidth() * mResolution;  // our resolution is dpi

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

	//DANGER!!
	// we apply (remove) header/footer from document dimensions to match
	// GetPrintableRect, because currently (14 sept 00) Doc maintains full printable
	// rect as image size (even though it uses only body rect for layout)
	
		outHeight -= mProps->GetHeader() * mResolution;
		outHeight -= mProps->GetFooter() * mResolution;


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
	mPrintSpec->GetPaperRect(printableArea);
	ApplyMargins(printableArea, mPrintSpec, mProps);
	ApplyHeaderFooter(printableArea, mPrintSpec, mProps);

	// overlap isn't considered here, since it only comes into
	// play with multiple panels.  The Printable Area isn't affected.  
	// @see CountPanels
	// @see ScrollToPanels
	
	return printableArea;
}


/*
InchesToPrintPixels
*/
SInt32
PhotoPrinter::InchesToPrintPixels(const double inUnits) 
{
	SInt16 hRes;
	SInt16 vRes;
	
	mPrintSpec->GetResolutions(vRes, hRes);
	return inUnits * vRes;
} // InchesToPrintPixels

void	
PhotoPrinter::InnerDrawLoop		(PhotoPrintModel* printingModel, HORef<LGWorld>& possibleOffscreen, MRect band,
									MatrixRecord* mat, CGrafPtr port, GDHandle device, RgnHandle clip,
									CGrafPtr printerPort) 
{
	if (possibleOffscreen) 
		possibleOffscreen->BeginDrawing();
		
	printingModel->Draw(mat,		// matrix for transforming model to destination space
						port, //  offscreen gworld
						device,
						clip);

	if (possibleOffscreen){
		possibleOffscreen->EndDrawing();
		possibleOffscreen->CopyImage(printerPort, band);
	}//endif end drawing offscreen + copy back

	}//end InnerDrawLoop

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
PhotoPrinter::MapModelForPrinting(MatrixRecord* ioMatrix, PhotoPrintModel* inModel, MRect& outPanelBounds) {

	// this is entire size, all pages if multiple
	SInt16 docHeight; 
	SInt16 docWidth;
	GetDocumentDimensionsInPixels(docHeight, docWidth);

	// get the printable area  (typically larger if resolution > 72 dpi)
	// note that topleft moves according to panel
	MRect pageBounds;
	CalculatePrintableRect(mPrintSpec, mProps, pageBounds, mResolution);
	
	// the current panel is always located at 0,0 (PageRect convention)
	// (we offset/remap the layout so that the "current panel" lines up w/ 0,0)
	outPanelBounds = pageBounds;

	// get the view dimensions
	// these are the (base) coordinate system of the model
	// even though the doc view may not use all that area (due to header/footer)
	// this is the same entire area as CalculatePrintableRect uses 
	SDimension32 viewSize;
	mView->GetImageSize(viewSize);

	MRect imageRect;
	imageRect.SetWidth(viewSize.width);
	imageRect.SetHeight(viewSize.height);
	
	pageBounds.Offset(-mOriginLeft, -mOriginTop);
	pageBounds.SetHeight(pageBounds.Height() * mDoc->GetPageCount());
	
	inModel->MapItems(imageRect, pageBounds);
	
	
	// at the moment, we are not supporing any rotational/flip effects
	::SetIdentityMatrix(ioMatrix);
	// but we do create the matrix necessary to translate down to bodyRect origin
	MRect bodyRect;
	CalculateBodyRect(mPrintSpec, mProps, bodyRect, mResolution);
	::TranslateMatrix(ioMatrix, ::FixRatio(bodyRect.left, 1), ::FixRatio(bodyRect.top, 1));
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
// shouldn't we scroll the entire paper size?
	mPrintSpec->GetPageRect(frameSize);
	
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
	
	Boolean wasChanged;
	OSStatus status (inRecord->Validate(wasChanged));
	if (kPMNoError != status) Throw_(status);
}//end SetupPrintRecordToMatchProperties


