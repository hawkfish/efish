/*
	File:		PhotoPrinter.h

	Contains:	Definition of the base Printing Pane

	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	06 Apr 2001		drd		Use EGWorld instead of LGWorld (since it locks better)
	19 mar 2001		dml		remove GetPrintableRect, rename GetMatrixForPrinting
	02 feb 2001		dml		add DrawTestPage
	14 dec 2000		dml		add CalculatePaperRect
	13 sep 2000		dml		add support for header/footer, CalculateBodyRect
	19 jul 2000		dml		add banded printing
	19 jul 2000		dml		MapModelForPrinting calculates PanelRect
	17 jul 2000		dml		add ApplyMinimalMargins
	14 jul 2000		dml		add outDPI arg to CalculatePrintableRect
	11 jul 2000		dml		add static public CalculatePrintableRect
	26 Jun 2000		drd		Use double, not float
	20 june 2000	dml		moved BestFit to EUtil
	16 june 2000	dml		moved overlap to PrintProperties
*/

#pragma once

#include <LView.h>
#include "EPrintSpec.h"
#include "MRect.h"
#include "PrintProperties.h"
#include "HORef.h"

class EGWorld;
class PhotoPrintDoc;
class PhotoPrintModel;
class PhotoPrintView;

class PhotoPrinter : public LView
{

protected:
		PhotoPrintDoc*			mDoc;
		PhotoPrintModel*		mModel;
		SInt32					mOriginTop;		// used for scrolling calculations
		SInt32					mOriginLeft;	// "
		EPrintSpec*				mPrintSpec;
		PrintProperties*		mProps;
		SInt32					mResolution;		
		SInt16					mResFile;		
		PrintProperties::RotationType	mRotation;	
		GrafPtr					mPrinterPort;
		PhotoPrintView*			mView;
		
		static OSType			sCreator;
						
		static  void	ApplyHeaderFooter		(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props);															
		static	void	ApplyMargins			(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props);
		static	void	ApplyCustomMargins		(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props);
		static 	void 	ApplyMinimalMargins		(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props);
		virtual	void	ApplyRotation			(void);
		static	void	ApplySymmetricMargins 	(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props);

		// how big is it (returned in current-printer-resolution pixels)
		virtual void 	GetDocumentDimensionsInPixels(SInt16& outHeight, SInt16& outWidth);

		// utility
		virtual	SInt32	InchesToPrintPixels(const double inUnits);
				
		// some of the work is done directly, other is deferred via a matrix
		virtual void 	CreateMatrixForPrinting(MatrixRecord* ioMat, MRect& outPanelBounds);
		
		virtual void	InnerDrawLoop		(PhotoPrintModel*, HORef<EGWorld>& possibleOffscreen, 
											MRect band, MatrixRecord* mat, CGrafPtr port, GDHandle device,
											RgnHandle clip, CGrafPtr printerPort);
		virtual void	EraseOffscreen(EGWorld* pGW);
		
		virtual void 	DrawHeader();
		virtual void	DrawFooter();
		virtual void	DrawTestPage();
		
	public:
	
		enum { class_ID = 'FotP' };

						PhotoPrinter		(PhotoPrintDoc* inDoc, PhotoPrintView*	pPane, 
											EPrintSpec*	pPrintSpec, PrintProperties* inProps,
											GrafPtr thePort);
		virtual			~PhotoPrinter		(void);
		
		virtual	SInt16	CountPages			(bool bRotated = false);
		virtual void	DrawSelf			(void);

		static void 	SetupPrintRecordToMatchProperties(EPrintSpec* inRecord, 
															PrintProperties* inProps);


		virtual void	CountPanels(UInt32				&outHorizPanels,
									UInt32				&outVertPanels);

		virtual Boolean	ScrollToPanel(const PanelSpec	&inPanel);

	
		static void		CalculatePaperRect(EPrintSpec* inSpec,
											const PrintProperties* inProps,
											MRect& outRect,
											SInt16 outDPI = 72);

		static void		CalculatePrintableRect(EPrintSpec* inSpec,
												const PrintProperties* inProps,
												MRect& outRect,
												SInt16 outDPI = 72);

		static void		CalculateBodyRect(EPrintSpec* inSpec,
											const PrintProperties* inProps,
											MRect& outRect,
											SInt16 outDPI = 72);
											
		static void		CalculateHeaderRect(EPrintSpec* inSpec,
											const PrintProperties* inProps,
											MRect& outRect,
											SInt16 outDPI = 72);
		
		static void 	CalculateFooterRect(EPrintSpec* inSpec,
											const PrintProperties* inProps,
											MRect& outRect,
											SInt16 outDPI = 72);									

		static void		UpdatePrintProgress();
		
		
		static void			SetCurPrinterCreator(const OSType inCreator) {sCreator = inCreator;};
		static const OSType	GetCurPrinterCreator(void) {return sCreator;};

};



