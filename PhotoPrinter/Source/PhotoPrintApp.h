// ===========================================================================
//	PhotoPrintApp.cp 		Copyright © 2000 Electric Fish, Inc. All rights reserved.

#ifndef _H_CAppearanceApp
#define _H_CAppearanceApp
#pragma once

#include <LDocApplication.h>

class PhotoPrintApp : public LDocApplication {

public:
							PhotoPrintApp();
	virtual					~PhotoPrintApp();

	virtual Boolean			ObeyCommand(
								CommandT			inCommand,
								void*				ioParam = nil);	

	virtual void			FindCommandStatus(
								CommandT			inCommand,
								Boolean&			outEnabled,
								Boolean&			outUsesMark,
								UInt16&				outMark,
								Str255				outName);

	virtual void			OpenDocument(
									FSSpec*				inMacFSSpec);

protected:
	virtual void			StartUp();
	
			void			RegisterClasses();
			void			AddEvents			(void);
			void			AddCommands			(void);
};

#endif // _H_CAppearanceApp