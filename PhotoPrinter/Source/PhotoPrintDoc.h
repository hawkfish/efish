/*
	File:		PhotoPrintDoc.h

	Contains:	Definition of the document.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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

		// HOW BIG IS IT?!
		double					mWidth; 		//floating point inches.  hah!
		double					mHeight;
		int						mDPI;
		
		// Pay attention!.  We do NOT use LDocument's stupidly built-in 
		// mPrintSpec (why isn't a pointer, dang-it), instead we always use
		// GetPrintRec() (public, below), which references mEPrintSpec
		// a much more useful object
		HORef<EPrintSpec>		mEPrintSpec;
		PhotoPrintView*			mScreenView;
		
		void					CreateWindow		(ResIDT				inWindowID, 
													 Boolean 			inVisible);
		
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
		
		virtual void			SetResolution(SInt16 inRes);
		virtual SInt16			GetResolution(void) const { return mDPI; }	

		HORef<EPrintSpec>		GetPrintRec(void);
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
		virtual StringPtr		GetDescriptor(Str255		outDescriptor) const;		

			//	LSingleDoc
			
			//LPeriodical
		virtual	void			SpendTime			(const EventRecord&	inMacEvent);


	}; // end PhotoPrintDoc