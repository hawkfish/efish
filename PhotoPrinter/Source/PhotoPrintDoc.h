//	PhotoPrintDoc.h 
//		Copyright © 2000 Electric Fish, Inc. All rights reserved.
#pragma once
#include <LDocument.h>
#include "PhotoPrintController.h"

class PhotoPrintView;

class PhotoPrintDoc 	: public LSingleDoc
						, public LPeriodical
{
	protected:
		PhotoPrintView*			mPhotoPrintView;
		OSType					mFileType;
		
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


			//	LCommander
		virtual Boolean			ObeyCommand			(CommandT			inCommand,
													 void*				ioParam = nil);

			//	LDocument			
		virtual OSType			GetFileType			(void) const;
		virtual Boolean			AskSaveAs			(FSSpec&			outFSSpec,
													 Boolean			inRecordIt);

		virtual SInt16			AskSaveChanges		(bool				inQuitting);

		virtual void			DoSave				(void);
		virtual void			DoRevert			(void);

		virtual void			DoPrint				(void);
		virtual void			DoPrintPreview		(void);

			//	LSingleDoc
			
			//LPeriodical
		virtual	void			SpendTime			(const EventRecord&	inMacEvent);


		PhotoPrintView*			GetView(void) 		{return mPhotoPrintView;};
	

			
	}; // end PhotoPrintDoc