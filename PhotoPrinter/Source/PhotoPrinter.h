//	PhotoPrinter.h
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.
#pragma once
#include <LView.h>
#include "PhotoPrintModel.h"
#include "PhotoPrintDoc.h"

class PhotoPrinter : public LView
	{
protected:
		PhotoPrintDoc*			mDoc;
		PhotoPrintView*			mView;
		PhotoPrintModel*		mModel;
		LPrintSpec*				mPrintSpec;
		short					mResFile;			
		GrafPtr					mPrinterPort;

		long					mResolution;		
		float					mOverlap;
		long					originTop;
		long					originLeft;		

		static	ExceptionCode	sExceptionCode;
															
		virtual MRect	GetPrintableRect	(void); // printable area
		virtual	void			ApplyMargins		();

		virtual void CreateMapping(MatrixRecord* ioMat);
		
	public:
	
		enum { class_ID = 'FotP' };

						PhotoPrinter		(PhotoPrintDoc* inDoc, PhotoPrintView*	pPane, 
											LPrintSpec*	pPrintSpec,
											GrafPtr thePort);
		virtual			~PhotoPrinter		(void);
		
		virtual void	DrawSelf			(void);
		
		virtual	long	CountPages			();
		static	void 	BestFit				(long&	outWidth, 
											 long&	outHeight,
											 const	long&		fitWidth,
											 const	long&		fitHeight,
											 const	long&		propWidth,
											 const	long&		propHeight);

// these override LView's methods

#ifdef SOON
		virtual void	CountPanels(UInt32				&outHorizPanels,
									UInt32				&outVertPanels);

		virtual Boolean	ScrollToPanel(const PanelSpec	&inPanel);

		virtual void	SuperPrintPanel(const PanelSpec	&inSuperPanel,
										RgnHandle		inSuperPrintRgnH);
#endif
};
