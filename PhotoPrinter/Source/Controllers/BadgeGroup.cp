/*
	File:		BadgeGroup.cp

	Contains:	tab-group commander for photo badges
	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	14 mar 2001		dml			override RotateTarget to handle single badge submit
	26 feb 2001		dml			created

*/
#include "BadgeGroup.h"
#include "FileEditText.h"

BadgeGroup::BadgeGroup() 
	:LTabGroup()
{}

BadgeGroup::BadgeGroup(const BadgeGroup&	inOriginal) 
	:LTabGroup(inOriginal)
{}

BadgeGroup::BadgeGroup(LStream*			inStream) 
	:LTabGroup(inStream)
{}

BadgeGroup::BadgeGroup(LCommander*			inSuper) 
	:LTabGroup(inSuper)
{}

BadgeGroup::~BadgeGroup() 
{}

#pragma mark -

Boolean
BadgeGroup::HandleKeyPress(const EventRecord&	inKeyEvent) {

	Boolean	keyHandled = true;
	UInt16	theChar = (UInt16) (inKeyEvent.message & charCodeMask);

		// Process Tab or Shift-Tab. Pass up if there are any other
		// modifiers keys pressed.

	switch (theChar) {
		case char_Home:
		case char_LeftArrow:
		case char_UpArrow:
		case char_PageUp:
			RotateTarget(true/*forward*/);
			break;
			
		case char_RightArrow:
		case char_DownArrow:
		case char_End:
		case char_PageDown:
			RotateTarget(false/*backward*/);
			break;
			
		case char_Escape:
		case char_Tab:
		case char_Enter:
		case char_Return: {
			RotateTarget((inKeyEvent.modifiers & shiftKey) != 0);
			}//end case finish + move 
			break;
		default:
			keyHandled = LCommander::HandleKeyPress(inKeyEvent);
			break;
		}//switch 
		


	return keyHandled;
	}//


//---------------------------------------------------------------
// ¥RotateTarget
// the badges use SwitchTarget to install their changes
// but if there is only one badge, there is no switch
//---------------------------------------------------------------

void		
BadgeGroup::RotateTarget(Boolean				inBackward) {

	LCommander	*currTabTarget = GetTarget();
	// use all the wacky logic in superclass and let it try to deal
	LTabGroup::RotateTarget(inBackward);
	LCommander	*newTarget = GetTarget();
	
	if (newTarget == currTabTarget) {
		FileEditText* badge = dynamic_cast<FileEditText*>(newTarget);
		if (badge)
			badge->TryRename();
		}//endif unable to rotate targets means only one
		
	}//end RotateTarget

