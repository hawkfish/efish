/*
	File:		DeleteAction.cp

	Contains:	Impelentation the selection deletion action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		20 Jul 2001		rmgw	Include PhotoPrintView.  Bug #200.
		18 Jul 2001		rmgw	Derive from ModelAction.
		18 Jul 2001		rmgw	Provide accessors for MVC values.
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#include "DeleteAction.h"

#include "PhotoPrintView.h"

// ---------------------------------------------------------------------------
//	¥ DeleteAction													  [public]
// ---------------------------------------------------------------------------

DeleteAction::DeleteAction (

	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex)

	: ModelAction(inDoc, inStringIndex)

	{ // begin DeleteAction
	
		PhotoItemList	selection (GetView ()->Selection ());
		
		GetView ()->RemoveFromSelection (selection);
		GetView ()->GetLayout ()->RemoveItems (selection.begin (), selection.end ());
		
		LayoutImages();
		
	} // DeleteAction

// ---------------------------------------------------------------------------
//	¥ ~DeleteAction													  [public]
// ---------------------------------------------------------------------------

DeleteAction::~DeleteAction (void)

	{ // begin ~DeleteAction
	
	} // end ~DeleteAction

