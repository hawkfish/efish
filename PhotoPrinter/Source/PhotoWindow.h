/*
	File:		PhotoWindow.h

	Contains:	Subclass of LWindow

	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):
	
		20 Aug 2001		rmgw	Carbon event tracking.  Bug #334.
		16 Aug 2001		rmgw	Add exception handling.  Bug #330.
		12 Sep 2000 	dml		created
	
*/


#include <LWindow.h>

#if TARGET_API_MAC_CARBON
	#include "MUPP.h"
	#include <CarbonEvents.h>
	DefineMUUPP(EventHandler);
#endif

class PhotoPrintDoc;

class PhotoWindow : public LWindow 

	{

	protected:
		PhotoPrintDoc*	mDoc;

#if TARGET_API_MAC_CARBON
		//	Carbon Events
		friend	class				MUPP<EventHandlerProcPtr>;
		static	MUPP<EventHandlerProcPtr>	sWindowEventFilterProc;
		static	pascal OSStatus	
							WindowEventFilterProc	(EventHandlerCallRef		inHandlerCallRef, 
													 EventRef 				inEvent, 
													 void *					inUserData);

		virtual	OSStatus	OnWindowBoundsChanged	(EventHandlerCallRef		inHandlerCallRef, 
													 EventRef 					inEvent);
		virtual	OSStatus	OnWindowEvent			(EventHandlerCallRef		inHandlerCallRef, 
													 EventRef 					inEvent);
#endif

	public:
		enum { class_ID = FOUR_CHAR_CODE('kwin') };

							PhotoWindow(LStream*			inStream);
		virtual				~PhotoWindow();
		
		//	¥ LCommander
		virtual Boolean		ObeyCommand			(CommandT			inCommand,
												 void*				ioParam = nil);	

		virtual Boolean		CalcStandardBounds(Rect	&outStdBounds) const;
	
		void	SetDoc(PhotoPrintDoc* inDoc) {mDoc = inDoc;};
		
	};//end class PhotoWindow