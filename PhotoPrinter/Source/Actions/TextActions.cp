/*
	File:		TextActions.cp

	Contains:	various actions associated with fonts (face, size, style)

	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		24 Jul 2001		rmgw	Use RefreshImages.  Bug #220.
		13 mar 2001		dml		created
*/

#include "TextActions.h"

#include "PhotoPrintDoc.h"

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
		//	Get the new undo state
	bool				mRedoDirty (GetCurrentDirty ());

		//	Swap the values
	for (PhotoIterator i = mImages.begin(); i != mImages.end(); ++i) {
		(*i)->GetProperties().SetFontNumber(newFont);
		}//for
	RefreshImages();

	//	Restore the dirty flag
	GetDocument ()->SetDirty (mUndoDirty);
	
	//	Swap the state
	mUndoDirty = mRedoDirty;
		
}//end RedoSelf

void
FontAction::UndoSelf() {
		//	Get the new undo state
	bool				mRedoDirty (GetCurrentDirty ());

		//	Swap the values
	for (PhotoIterator i = mImages.begin(); i != mImages.end(); ++i) {
		(*i)->GetProperties().SetFontNumber((*(oldMap.find(*i))).second);
		}//for
	RefreshImages();

	//	Restore the dirty flag
	GetDocument ()->SetDirty (mUndoDirty);
	
	//	Swap the state
	mUndoDirty = mRedoDirty;

}//end UndoSelf
	
	