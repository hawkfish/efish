/*
	File:		PhotoPrintDoc.h

	Contains:	Definition of the document.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		13 Sep 2000		drd		Added gWindowProxies
		13 sep 2000		dml		moved kFeelGoodMargin here, removed CalcInitialWindowRect (see PhotoWindow.cp)
		12 Sep 2000		drd		Added UpdatePageNumber
		12 sep 2000		dml		add CalcInitialWindowRect
		07 aug 2000		dml		added mPageCount, mZoomDisplay
		06 aug 2000		dml		add mScroller (support for zooming), removed mPhotoPrintView
		23 Aug 2000		drd		Override HandleAppleEvent
		21 Aug 2000		drd		Removed ParseLayout, renamed sParseLayout
		11 aug 2000		dml		add SetController();
		04 Aug 2000		drd		Removed ObeyCommand; added HandleKeyPress
		27 Jul 2000		drd		No longer an LPeriodical; override IsModified; switched to
								overriding HandlePageSetup, HandlePrint
		14 jul 2000		dml		add StPrintSession accessors
		13 jul 2000		dml		add mNumPages, GetPageCount()
		11 jul 2000		dml		add numPages arg to MatchViewToPrintRec
		28 jun 2000		dml		add serialization of layout
		27 Jun 2000		drd		Made MatchViewToPrintRec public
		27 june 2000 	dml		add MatchViewToPrintRec
		27 june 2000	dml		add DoPageSetup
		26 June 2000 	dml		add Initialize
		21 june 2000	dml		exposed PrintProperties (for custom print dialog)
		20 Jun 2000		drd		Added gCurDocument, so others know who we are at constructor time
		15 jun 2000		dml		changed mDPI to be SInt16, now that xmlio supports it
		14 Jun 2000		drd		Added GetModel
*/

#pragma once

#include <LDocument.h>
#include <LScrollerView.h>
#include "PhotoPrintController.h"
#include "PhotoPrintView.h"						// So we can get model
#include "EPrintSpec.h"
#include "HORef.h"
#include "PrintProperties.h"
#include "DocumentProperties.h"
#include "MFileSpec.h"
#include "HORef.h"

namespace XML {
	class Output;
	class Element;
	class Handler;
}

class PhotoPrintDoc 	: public LSingleDoc
{
	public:
		static SInt16	kFeelGoodMargin;

	protected:
		HORef<MFileSpec>		mFileSpec;
		OSType					mFileType;
		PrintProperties			mPrintProperties;
		DocumentProperties		mProperties;
		HORef<EPrintSpec>		mPrintSpec;
		LScrollerView*			mScroller;	
		LPane*					mPageCount;
		LPane*					mZoomDisplay;

		// HOW BIG IS IT?!
		double					mWidth; 		//floating point inches.  hah!
		double					mHeight;
		SInt16					mDPI;
		SInt16					mNumPages;
		
		PhotoPrintView*			mScreenView;
		
		void					CreateWindow		(ResIDT				inWindowID, 
													 Boolean 			inVisible);
		
		void					Initialize			(void);
		void					AddEvents			(void);
		void					AddCommands			(void);
		
	public:
								PhotoPrintDoc		(LCommander*		inSuper,
													Boolean inVisible = true);
								PhotoPrintDoc		(LCommander*		inSuper,
													 const	FSSpec&		inSpec,
													 Boolean inVisible = true);
		virtual					~PhotoPrintDoc	(void);


		PhotoPrintView*			GetView(void) 				{ return mScreenView; }
	
		virtual double			GetWidth(void) const		{ return mWidth; }
		virtual double			GetHeight(void) const		{ return mHeight; }
		virtual SInt16			GetPageCount(void) const	{ return mNumPages; }
		
		virtual void			SetResolution(SInt16 inRes);
		virtual SInt16			GetResolution(void) const	{ return mDPI; }	

		// Pay attention!.  We do NOT use LDocument's stupidly built-in 
		// mPrintSpec (why isn't a pointer, dang-it), instead we always use
		// GetPrintRec().  Eventually this will be a per-document gizmo, but now
		// it actually is a global living in the App
		HORef<EPrintSpec>&		GetPrintRec(void);
		PrintProperties&		GetPrintProperties (void) {return mPrintProperties;};
		void					ForceNewPrintSession(void);

		DocumentProperties&		GetProperties(void)	{ return mProperties; }
		const DocumentProperties& GetProperties(void) const {return mProperties;}
		virtual bool			IsFileSpecified(void) const {return mFileSpec != nil;}
		PhotoPrintModel*		GetModel(void)		{ return mScreenView->GetModel(); }

		void					SetController(OSType newController);

		void					UpdatePageNumber(const SInt16 inPageCount);

// IO
				void 			Write(XML::Output &out) ;
				void 			Read(XML::Element &elem);
				static void		sParseObjects(XML::Element &elem, void *userData);
				static void		sParseObject(XML::Element &elem, void *userData);
				static void		sDocHandler(XML::Element &elem, void* userData);
				static void		ParseLayout(XML::Element &elem, void *userData);

		void					MatchViewToPrintRec (SInt16 numPages = 1);

			//	LCommander
		virtual Boolean			HandleKeyPress(const EventRecord&	inKeyEvent);

			//	LDocument			
		virtual OSType			GetFileType			(void) const;
		virtual Boolean			AskSaveAs			(FSSpec&			outFSSpec,
													 Boolean			inRecordIt);
		virtual SInt16			AskSaveChanges		(bool				inQuitting);

		// LModelObject
		virtual void			HandleAppleEvent(
										const AppleEvent	&inAppleEvent,
										AppleEvent			&outAEReply,
										AEDesc				&outResult,
										SInt32				inAENumber);

		virtual void			DoOpen				(const FSSpec& inSpec);
		virtual void			DoSave				(void);
		virtual void			DoSaveToSpec		(const FSSpec& inSpec);
		virtual void			DoRevert			(void);

		virtual void			DoPrint				(void);
		virtual StringPtr		GetDescriptor(Str255		outDescriptor) const;		
		virtual void			HandlePrint			(void);
		virtual void			HandlePrintPreview	(void);
		virtual void			HandlePageSetup		(void);
		virtual Boolean			IsModified();

			//	LSingleDoc
			
		static	PhotoPrintDoc*	gCurDocument;
		static	bool			gWindowProxies;
	}; // end PhotoPrintDoc
