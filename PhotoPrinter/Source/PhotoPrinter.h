//	PhotoPrinter.h
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.
#pragma once
#include <LView.h>
#include "EPrintSpec.h"
#include "MRect.h"

class PhotoPrintDoc;
class PhotoPrintModel;
class PhotoPrintView;
class PrintProperties;

class PhotoPrinter : public LView
{
public:
	enum RotationType {
		kNoRotation = 0,
		k90CWRotation,
		k180Rotation,
		k270CWRotation,
		kFnordRotation };

protected:
		PhotoPrintDoc*			mDoc;
		PhotoPrintView*			mView;
		PhotoPrintModel*		mModel;
		EPrintSpec*				mPrintSpec;
		SInt16					mResFile;			
		PrintProperties*		mProps;
		GrafPtr					mPrinterPort;
		RotationType			mRotation;

		long					mResolution;		
		float					mOverlap;
		long					originTop;
		long					originLeft;
																	
		virtual MRect	GetPrintableRect	(void); // printable area
		virtual	void	ApplyMargins		();

		// some of the work is done directly, other is deferred via a matrix
		virtual void MapModelForPrinting(MatrixRecord* ioMat, PhotoPrintModel* inModel);
		virtual void GetDocumentDimensionsInPixels(SInt16& outHeight, SInt16& outWidth);
		
		static const char *const sRotationLabels[kFnordRotation];
		
		
	public:
	
		enum { class_ID = 'FotP' };

						PhotoPrinter		(PhotoPrintDoc* inDoc, PhotoPrintView*	pPane, 
											EPrintSpec*	pPrintSpec, PrintProperties* inProps,
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

		static void 	SetupPrintRecordToMatchProperties(EPrintSpec* inRecord, 
															PrintProperties* inProps);


		static const char* const* GetRotationLabels(void) {return sRotationLabels;};
};
