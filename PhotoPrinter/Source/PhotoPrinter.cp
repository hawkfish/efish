//	PhotoPrinter.cp
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.

#include "PhotoPrinter.h"
#include "PhotoPrintView.h"
	
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
//
//-----------------------------------------------------
void
PhotoPrinter::CreateMapping(MatrixRecord* ioMatrix) {
	::SetIdentityMatrix(ioMatrix);

	MRect docBounds;
	//ask doc for its bounds

	MRect pageBounds;
	mPrintSpec->GetPageRect(pageBounds);

	}//end CreateMapping





//-----------------------------------------------------
//
//-----------------------------------------------------
PhotoPrinter::PhotoPrinter(PhotoPrintDoc* inDoc, 
							PhotoPrintView*	inView, 
							LPrintSpec*	inSpec,
							GrafPtr inPort) 
	: mDoc (inDoc)
	, mView (inView)
	, mModel (inDoc->GetView()->GetModel())
	, mPrintSpec (inSpec)
	, mResFile (::CurResFile())
	, mPrinterPort		(inPort)
{

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
	MatrixRecord mat;
	CreateMapping(&mat);
	
	GrafPtr	curPort;
	::GetPort(&curPort);
	GDHandle curDevice;
	curDevice =	::GetGDevice();

	mModel->Draw(&mat,
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