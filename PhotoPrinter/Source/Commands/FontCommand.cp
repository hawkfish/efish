/*
	File:		FontCommand.cp

	Contains:	Implementation of the Clear command (in Edit menu).

	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		13 Mar 2001		dml		Created
*/

#include "FontCommand.h"

#include "TextActions.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintResources.h"

/*
FontCommand
*/
FontCommand::FontCommand(
	const CommandT	inCommand, 
	PhotoPrintDoc*	inDoc,
	ResIDT	inMenuID)
	: PhotoDocCommandAttachment(inCommand, inDoc)
	, mMenuID (inMenuID)
{
	for (short menuIndex = 1; menuIndex <= ::CountMenuItems(FontMenu()->GetMacMenuH()); ++menuIndex) {
		Str255 fontName;
		::GetMenuItemText (FontMenu()->GetMacMenuH(), menuIndex, fontName);
		short fontIndex;
		::GetFNum(fontName, &fontIndex);
		
		MenuFontMap::value_type menuToFont (menuIndex, fontIndex);
		MenuFontMap::value_type fontToMenu (fontIndex, menuIndex);

		mMenuToFontMap.insert(menuToFont);
		mFontToMenuMap.insert(fontToMenu);
		}//end
} // FontCommand




/*
~FontCommand
*/
FontCommand::~FontCommand()
{
} // ~FontCommand







/*
ExecuteCommandNumber {OVERRIDE}
	Put up print dialog
*/
void		
FontCommand::ExecuteCommandNumber	(CommandT			inCommand,
									 void*				/*inCommandData*/)
{
	short	index = FontMenu ()->IndexFromCommand (inCommand);
	short	fontIndex = (*(mMenuToFontMap.find(index))).second;
	mDoc->PostAction(new FontAction(mDoc, si_ChangeFont, fontIndex));
} // ExecuteCommand										 



LMenu*
FontCommand::FontMenu(void) const {
	LMenuBar*	menuBar (LMenuBar::GetCurrentMenuBar ());
	Assert_(menuBar);
	
	LMenu*		menu (menuBar->FetchMenu (mMenuID));
	Assert_(menu);
	
	return menu;
	}//end FontMenu



/*
FindCommandStatus {OVERRIDE}
	Sets its argument to TRUE if the command should be enabled (if anything is selected)
*/
void		
FontCommand::FindCommandStatus(SCommandStatus* ioStatus)
{
	if (mDoc->GetView()->Selection().empty()) {
		*ioStatus->enabled = false;
		return;
		}//endif no selection
		
	*ioStatus->enabled = true;
	*ioStatus->usesMark = true;
	short menuIndex ((*(mFontToMenuMap.find(mDoc->GetView()->GetPrimarySelection()->GetProperties().GetFontNumber()))).second);
	*ioStatus->mark = (menuIndex == FontMenu()->IndexFromCommand(ioStatus->command)) ? checkMark : 0;
} // FindCommandStatus



bool		
FontCommand::HandlesCommand			(CommandT			inCommand) const {
	return (FontMenu ()->IndexFromCommand (inCommand) > 0);
	}///end HandlesCommand

