/*
	File:		PhotoPrinter.h

	Contains:	Implementation of the base Printing Pane

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
																	
		static	void	ApplyMargins			(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props);
		static	void	ApplyCustomMargins		(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props);
		static 	void 	ApplyMinimalMargins		(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props);
		virtual	void	ApplyRotation			(void);
		static	void	ApplySymmetricMargins 	(MRect& ioRect, EPrintSpec* spec, const PrintProperties* props);

		// how big is it (returned in current-printer-resolution pixels)
		virtual void 	GetDocumentDimensionsInPixels(SInt16& outHeight, SInt16& outWidth);
		// return the printable area after margin calculations
		virtual MRect	GetPrintableRect		(void); 
		// utility
		virtual	SInt32	InchesToPrintPixels(const double inUnits);
				
		// some of the work is done directly, other is deferred via a matrix
		virtual void 	MapModelForPrinting(MatrixRecord* ioMat, PhotoPrintModel* inModel,
											MRect& outPanelBounds);
		
		virtual void	InnerDrawLoop		(PhotoPrintModel*, HORef<LGWorld>& possibleOffscreen, 
											MRect band, MatrixRecord* mat, CGrafPtr port, GDHandle device,
											RgnHandle clip, CGrafPtr printerPort);
		virtual void	EraseOffscreen(LGWorld* pGW);
		
		
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

	
		static void		CalculatePrintableRect(EPrintSpec* inSpec,
												const PrintProperties* inProps,
												MRect& outRect,
												SInt16 outDPI = 72);

		static void		UpdatePrintProgress();

};



