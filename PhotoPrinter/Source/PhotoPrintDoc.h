/*
	File:		PhotoPrintDoc.h

	Contains:	Definition of the document.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
						, public LPeriodical
{
	protected:
		HORef<MFileSpec>		mFileSpec;
		PhotoPrintView*			mPhotoPrintView;
		OSType					mFileType;
		PrintProperties			mPrintProperties;
		DocumentProperties		mProperties;
		HORef<StPrintSession>	mPrintSession;

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


		PhotoPrintView*			GetView(void) 		{return mPhotoPrintView;};
	
		virtual double			GetWidth(void) const {return mWidth;};
		virtual double			GetHeight(void) const {return mHeight;};
		virtual SInt16			GetPageCount(void) const {return mNumPages;};
		
		virtual void			SetResolution(SInt16 inRes);
		virtual SInt16			GetResolution(void) const { return mDPI; }	

		// Pay attention!.  We do NOT use LDocument's stupidly built-in 
		// mPrintSpec (why isn't a pointer, dang-it), instead we always use
		// GetPrintRec().  Eventually this will be a per-document gizmo, but now
		// it actually is a global living in the App
		HORef<EPrintSpec>		GetPrintRec(void);
		PrintProperties&		GetPrintProperties (void) {return mPrintProperties;};
		void SetPrintSession	(HORef<StPrintSession>& inSession) {mPrintSession = inSession;};
		HORef<StPrintSession>&	GetPrintSession(void) {return mPrintSession;};

		DocumentProperties&		GetProperties(void)	{ return mProperties; }
		const DocumentProperties& GetProperties(void) const {return mProperties;}
		virtual bool			IsFileSpecified(void) const {return mFileSpec != nil;}
		PhotoPrintModel*		GetModel(void)		{ return mPhotoPrintView->GetModel(); }

// IO
				void 			Write(XML::Output &out) ;
				void 			Read(XML::Element &elem);
				static void		sParseObjects(XML::Element &elem, void *userData);
				static void		sParseObject(XML::Element &elem, void *userData);
				static void		sDocHandler(XML::Element &elem, void* userData);
				void			ParseLayout(XML::Element &elem, void *userData);
				static void		sParseLayout(XML::Element &elem, void *userData);

		void					MatchViewToPrintRec (SInt16 numPages = 1);

			//	LCommander
		virtual Boolean			ObeyCommand			(CommandT			inCommand,
													 void*				ioParam = nil);

			//	LDocument			
		virtual OSType			GetFileType			(void) const;
		virtual Boolean			AskSaveAs			(FSSpec&			outFSSpec,
													 Boolean			inRecordIt);

		virtual SInt16			AskSaveChanges		(bool				inQuitting);

		virtual void			DoOpen				(const FSSpec& inSpec);
		virtual void			DoSave				(void);
		virtual void			DoSaveToSpec		(const FSSpec& inSpec);
		virtual void			DoRevert			(void);

		virtual void			DoPrint				(void);
		virtual void			DoPrintPreview		(void);
		virtual void			DoPageSetup			(void);
		virtual StringPtr		GetDescriptor(Str255		outDescriptor) const;		

			//	LSingleDoc
			
			//LPeriodical
		virtual	void			SpendTime			(const EventRecord&	inMacEvent);

		static	PhotoPrintDoc*	gCurDocument;
	}; // end PhotoPrintDoc