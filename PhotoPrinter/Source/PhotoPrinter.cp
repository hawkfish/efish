//	PhotoPrinter.cp
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.

#include "PhotoPrinter.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintView.h"
#include "PrintProperties.h"
//-----------------------------------------------------
//
//-----------------------------------------------------
MRect	
PhotoPrinter::GetPrintableRect	(void)
{
	MRect printableArea;
	
	// start with printable area from print rec
	// subtract out margins
	// (how to handle overlap?)
	
	return printableArea;
}


//-----------------------------------------------------
//
//-----------------------------------------------------
void			
PhotoPrinter::ApplyMargins		()
{
}



//-----------------------------------------------------
//GetDocumentDimensions.  Handles all 90 degree rotations correctly
//-----------------------------------------------------
void
PhotoPrinter::GetDocumentDimensionsInPixels(SInt16& outHeight, SInt16& outWidth) {
	//ask doc for its bounds
	outHeight = (SInt16)(mDoc->GetHeight() * 72);
	outWidth = (SInt16)(mDoc->GetWidth() * 72);

	// if we are rotating, switch width and height;
	switch (mRotation) {
		case k90CWRotation:
		case k270CWRotation: 
			{
				SInt16 temp (outHeight);
				outHeight = outWidth;
				outWidth = temp;
			}
			break;
		}//end switch
}//end GetDocumentDimensions



//-----------------------------------------------------
//SetupPrintRecordToMatchProperties
//-----------------------------------------------------
void
PhotoPrinter::SetupPrintRecordToMatchProperties() 
{
	// learn about resolutions available
	SInt16 minX, minY, maxX, maxY;
	mPrintSpec->WalkResolutions( minX,  minY,  maxX,  maxY);
	// learn about current resolution
	SInt16 curVRes, curHRes;
	mPrintSpec->GetResolutions(curVRes, curHRes);
	
	if (mProps->GetHiRes()) {
		if (curVRes != maxY) 
			mPrintSpec->SetResolutions(maxY, maxX);
		}//endif best resolution
	else {
		if (curVRes != minY)
			mPrintSpec->SetResolutions(minY, minX);
		}//else draft resolution
	
}//end SetupPrintRecordToMatchProperties



//-----------------------------------------------------
//MapModelForPrinting.
//
// some of the work is done directly on the model (the items)
// (translation, scaling).  However, Rotation/skew/weird flipping, etc
// is done via a matrix, which is multiplied in to each site 
// when the site is drawn.
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
	MRect imageRect (0,0,1,1);
	imageRect.SetWidth(viewSize.width);
	imageRect.SetHeight(viewSize.height);
	
	inModel->MapItems(imageRect, pageBounds);
	}//end CreateMapping





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
	, mRotation (kNoRotation)
{
	SetupPrintRecordToMatchProperties();
}
					
					


//-----------------------------------------------------
//
//-----------------------------------------------------
PhotoPrinter::~PhotoPrinter		(void) 
{
}



//-----------------------------------------------------
//
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

	// map it
	MatrixRecord mat;
	MapModelForPrinting(&mat, printingModel);


	printingModel->Draw(&mat,
						(CGrafPtr)curPort,
						curDevice);
}



//-----------------------------------------------------
//
//-----------------------------------------------------
long	
PhotoPrinter::CountPages			()
{
	int howMany (1);
	
	return howMany;
}

					 
//-----------------------------------------------------
//
//-----------------------------------------------------
void 
PhotoPrinter::BestFit (	long&	outWidth, 
						long&	outHeight,
						const	long&		fitWidth,
						const	long&		fitHeight,
						const	long&		propWidth,
						const	long&		propHeight)

	{ // begin BestFit
		
		if (propWidth > 0) {
			outWidth = fitWidth ;
			outHeight = (outWidth *  propHeight ) / propWidth;
			
			if (outHeight <= fitHeight ) return;
			} // if
		
		if (propHeight > 0) {
			outHeight = fitHeight ;
			outWidth = (outHeight *  propWidth ) / propHeight ;
			} // if
			
		else outWidth = outHeight = 0;
	
	} // end BestFit
	
	


#ifdef SOON
void	
//-----------------------------------------------------
//
//-----------------------------------------------------
PhotoPrinter::CountPanels(UInt32				&outHorizPanels,
							UInt32				&outVertPanels)

//-----------------------------------------------------
//
//-----------------------------------------------------
Boolean	
PhotoPrinter::ScrollToPanel(const PanelSpec	&inPanel)

//-----------------------------------------------------
//
//-----------------------------------------------------
void	
PhotoPrinter::SuperPrintPanel(const PanelSpec	&inSuperPanel,
						RgnHandle		inSuperPrintRgnH)
#endif