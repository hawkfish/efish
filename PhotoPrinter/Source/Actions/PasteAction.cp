/*
	File:		PasteAction.cp

	Contains:	Implementation of the paste action.

	Written by:	David Dunham and Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		26 Jul 2001		rmgw	Add EUserMessage.  Bug #228.
		26 Jul 2001		rmgw	Factor XML parsing.  Bug #228.
		24 Jul 2001		rmgw	Undo dirty state correctly.
		18 Jul 2001		rmgw	Provide accessors for MVC values.
		18 Jul 2001		rmgw	Split up ImageActions.
*/

#include "PasteAction.h"

#include "Layout.h"
#include "PhotoExceptionHandler.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintView.h"
#include "XMLHandleStream.h"
#include "XMLItemParser.h"

#include "EUserMessageServer.h"

#include "xmlinput.h"

const	ResIDT	TEXT_XMLParseClipWarning 	= 1143;
const	ResIDT	TEXT_XMLClipError			= 1144;

class PasteActionParser : public XMLItemParser

	{
	
		PasteAction*	mAction;
		
	public:
	
						PasteActionParser	(XML::Input&	inInput,
											 EUserMessage&	inMessage,
											 PasteAction*	inAction)
											: XMLItemParser (inInput, inMessage), mAction (inAction) 
											{}

		virtual	void	OnItemRead			(PhotoItemRef 	inItem)
											{mAction->OnItemRead (inItem);}
	};


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
	mUndoDirty = true;	//	Needed because of kNotAlreadyDone

	if (inType == kXMLFlavor) {
		StDisableDebugThrow_();
		StDisableDebugSignal_();

		try { 
			XMLHandleStream		stream(inData);		// LHandleStream assumes ownership of the handle
			XML::Input			in(stream);
			EUserMessage		parseMessage (TEXT_XMLParseClipWarning, kCautionIcon, ExceptionHandler::GetCurrentHandler ()->GetOperation ());
			
			PasteActionParser (in, parseMessage, this).ParseObjects ();
		} // try
		
		catch (const XML::ParseException& e) {
			// !!! should have an XML exception handler (we could get filename or something)
			LStr255 		sWhat (e.What());
			LStr255 		sLineNumber ((short)e.GetLine());
			LStr255 		sColumnNumber ((short)e.GetColumn());
			EUserMessage	parseError (TEXT_XMLClipError, kStopIcon, sWhat, sLineNumber, sColumnNumber);
			
			EUserMessageServer::GetSingleton ()->QueueUserMessage (parseError);
			throw;					// Rethrow -- hopefully app won't put up another alert
		} // catch
		
		catch (LException& e) {
			// 79 OpenCommand calls us via an Apple Event, so the catch in ECommandAttachment::ExecuteSelf
			// will never see the exception (it's not passed through the Toolbox). So we need to do our
			// error handling here.
			if (!ExceptionHandler::HandleKnownExceptions(e))
				throw;
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
OnItemRead
	This function handles the "photo" tag in our XML file, which represents a single image
*/
void
PasteAction::OnItemRead(PhotoItemRef	item) {
	GetView ()->SetupDraggedItem(item);
	mInsertedImages.push_back(item);
} // OnItemRead

/*
RedoSelf {OVERRIDE}
*/
void
PasteAction::RedoSelf()
{
		//	Get the new undo state
	bool				mRedoDirty (GetCurrentDirty ());
		
		//	Put them all back
	PhotoIterator	i;
	for (i = mInsertedImages.begin(); i != mInsertedImages.end(); i++) {
		GetView ()->GetLayout()->AddItem(*i, GetModel ()->end ());		// It will be adopted
	}
	mOwnsImages = false;						// They have all been adopted

		//	Redraw it
	this->LayoutImages();
	LCommander::SetUpdateCommandStatus(true);	// Menu may change due to paste

		//	Restore the dirty flag
	GetDocument ()->SetDirty (mUndoDirty);
	
		//	Swap the state
	mUndoDirty = mRedoDirty;

} // RedoSelf

/*
UndoSelf {OVERRIDE}
*/
void
PasteAction::UndoSelf()
{
		//	Get the new undo state
	bool				mRedoDirty (GetCurrentDirty ());

		// take them all away
	GetModel ()->RemoveItems(mInsertedImages, PhotoPrintModel::kRemove);
	mOwnsImages = true;

		//	Redraw it
	this->LayoutImages();

		//	Restore the dirty flag
	GetDocument ()->SetDirty (mUndoDirty);
	
		//	Swap the state
	mUndoDirty = mRedoDirty;

} // UndoSelf

