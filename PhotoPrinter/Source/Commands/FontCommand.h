/*
	File:		FontCommand.h

	Contains:	Definition of the Font command

	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		13 mar 2001		dml		Created
*/

#include "PhotoDocCommandAttachment.h"
#include <map>

typedef std::map<short, short> MenuFontMap;

class FontCommand : public PhotoDocCommandAttachment
{
protected:
	ResIDT 	mMenuID;
	
	MenuFontMap	mMenuToFontMap;
	MenuFontMap mFontToMenuMap;
	
	LMenu*		FontMenu(void) const;

public:
						FontCommand(const CommandT inCommand, 
									PhotoPrintDoc* inDoc,
									ResIDT inMenuID);
						~FontCommand();

	// ECommandAttachment
	virtual	void		ExecuteCommandNumber	(CommandT			inCommand,
												 void*				inCommandData);
	virtual	void		FindCommandStatus		(SCommandStatus*	inStatus);
	virtual	bool		HandlesCommand			(CommandT			inCommand) const;
	
};
