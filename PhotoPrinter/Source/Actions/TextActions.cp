/*
	File:		TextActions.cp

	Contains:	various actions associated with fonts (face, size, style)

	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	13 mar 2001		dml		created
*/

#include "TextActions.h"

FontAction::FontAction(PhotoPrintDoc*	inDoc,
						const SInt16	inStringIndex,
						SInt16 inFontID)
	: MultiImageAction(inDoc, inStringIndex)
	, newFont (inFontID)
{
	for (PhotoIterator i = mImages.begin(); i != mImages.end(); ++i) {
		SInt16Map::value_type		thePair(*i, (*i)->GetProperties().GetFontNumber());
		oldMap.insert(thePair);
		}//for
	}//end ct
	
FontAction::~FontAction()
{
}//end dt
	
void
FontAction::RedoSelf() {
	for (PhotoIterator i = mImages.begin(); i != mImages.end(); ++i) {
		(*i)->GetProperties().SetFontNumber(newFont);
		}//for
}//end RedoSelf

void
FontAction::UndoSelf() {
	for (PhotoIterator i = mImages.begin(); i != mImages.end(); ++i) {
		(*i)->GetProperties().SetFontNumber((*(oldMap.find(*i))).second);
		}//for
}//end UndoSelf
	
	