/*
	File:		PasteAction.cp

	Contains:	Implementation of the paste action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		18 Jul 2001		rmgw	Provide accessors for MVC values.
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#include "PasteAction.h"

#include "Layout.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintView.h"

#include "XMLHandleStream.h"

/*
PasteAction
	This works for both Paste and Drop
	We assume ownership of the handle
*/
PasteAction::PasteAction(
	PhotoPrintDoc*	inDoc,
	const SInt16	inStringIndex,
	const ScrapFlavorType	inType,
	Handle			inData)
	: PhotoPrintAction(inDoc, inStringIndex, kNotAlreadyDone)
{
	if (inType == kXMLFlavor) {
		XMLHandleStream		stream(inData);		// LHandleStream assumes ownership of the handle
		XML::Input			in(stream);

		try { //PhotoItem will throw if it can't find a QTImporter
			StDisableDebugThrow_();
			StDisableDebugSignal_();

			XML::Handler handlers[] = {
				XML::Handler("Objects", ObjectsHandler),
				XML::Handler::END
			};

			in.Process(handlers, static_cast<void*>(this));
		} catch (...) {
			//silently fail. !!! should put up an alert or log
		}//catch
	} else {
		// !!! Need to do something with PICT; for now, just prevent leak
		::DisposeHandle(inData);
	}
} // PasteAction

/*
~PasteAction
*/
PasteAction::~PasteAction()
{
	// Delete images if they have not been inserted
	if (mOwnsImages) {
		PhotoIterator	i;
		for (i = mInsertedImages.begin(); i != mInsertedImages.end(); i++) {
			delete (*i);
		}
	}
} // ~PasteAction

/*
ObjectsHandler
	This function handles the "Objects" tag in our XML file, which represents a collection
	of images
*/
void
PasteAction::ObjectsHandler(XML::Element &elem, void* userData) {
	
	XML::Handler handlers[] = {
		XML::Handler("photo", PhotoHandler),
		XML::Handler::END
		};
		
	elem.Process(handlers, userData);
} // ObjectsHandler

/*
PhotoHandler
	This function handles the "photo" tag in our XML file, which represents a single image
*/
void
PasteAction::PhotoHandler(XML::Element &elem, void* userData) {
	PasteAction*		action = static_cast<PasteAction*>(userData);

	PhotoPrintItem*		item = new PhotoPrintItem();
	item->Read(elem);
	action->GetView ()->SetupDraggedItem(item);
	action->mInsertedImages.push_back(item);
} // PhotoHandler

/*
RedoSelf {OVERRIDE}
*/
void
PasteAction::RedoSelf()
{
	PhotoIterator	i;
	for (i = mInsertedImages.begin(); i != mInsertedImages.end(); i++) {
		GetView ()->GetLayout()->AddItem(*i, GetModel ()->end ());		// It will be adopted
	}

	mOwnsImages = false;						// They have all been adopted

	this->LayoutImages();
	LCommander::SetUpdateCommandStatus(true);	// Menu may change due to paste
} // RedoSelf

/*
UndoSelf {OVERRIDE}
*/
void
PasteAction::UndoSelf()
{
	// take them all away
	GetView ()->RemoveFromSelection(mInsertedImages);
	GetModel ()->RemoveItems(mInsertedImages, PhotoPrintModel::kRemove);

	this->LayoutImages();

	mOwnsImages = true;
} // UndoSelf

