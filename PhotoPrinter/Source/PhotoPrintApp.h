/*
	File:		PhotoPrintApp.h

	Contains:	Definition of the application class.

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		21 Jun 2000		drd		Override HandleCreateElementEvent
		15 Jun 2000		drd		Override Initialize
*/

#ifndef _H_CAppearanceApp
#define _H_CAppearanceApp
#pragma once

#include <LDocApplication.h>

class PhotoPrintApp : public LDocApplication {

public:
							PhotoPrintApp();
	virtual					~PhotoPrintApp();

	// LCommander
	virtual void			FindCommandStatus(
								CommandT			inCommand,
								Boolean&			outEnabled,
								Boolean&			outUsesMark,
								UInt16&				outMark,
								Str255				outName);
	virtual Boolean			ObeyCommand(
								CommandT			inCommand,
								void*				ioParam = nil);	

	// LDocApplication
	virtual void			OpenDocument(
									FSSpec*				inMacFSSpec);

	// LModelObject
	virtual LModelObject*	HandleCreateElementEvent(
									DescType			inElemClass,
									DescType			inInsertPosition,
									LModelObject*		inTargetObject,
									const AppleEvent&	inAppleEvent,
									AppleEvent&			outAEReply);

protected:
	// LApplication
	virtual void			Initialize();
	virtual void			StartUp();
	
			void			AddCommands			(void);
			void			AddEvents			(void);
			void			RegisterClasses();
};

#endif // _H_CAppearanceApp