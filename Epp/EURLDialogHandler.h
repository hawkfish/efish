/*
	File:		EURLDialogHandler.h

	Contains:	Interface for a URL using dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2000 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>     9/14/00    rmgw    Import to Epp.
         <1>     12/9/99    rmgw    first checked in.
*/


#pragma once

#include "EDialog.h"

class CInternetConfig;

class EURLDialogHandler 	: public EDialog 

	{

	protected:
	
		CInternetConfig*		mIC;
		short					mEventMask;
		
		void					SetupIC					(void);
		void					SetupURL				(PaneIDT			inPaneID);

		virtual	void			OnURL					(PaneIDT			inPaneID);


	public:
		
								EURLDialogHandler		(ResIDT				inResID,
														 LCommander*		inSuper,
														 short				inEventMask = everyEvent);
		virtual					~EURLDialogHandler		(void);
		
		virtual	MessageT		DoDialog 				(void);
	};

