/*
	File:		PasteCommand.cp

	Contains:	Implementation of the Paste command (in Edit menu).

	Written by:	David Dunham

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		13 Aug 2001		drd		Call Release on handle we pass to an LHandleStream
		07 Aug 2001		rmgw	Replace PasteAction with ModelAction.  Bug #293.
		18 Jul 2001		rmgw	Split up ImageActions.
		11 Jul 2001		rmgw	kDragFlavor => kClipFlavor.
		23 May 2001		drd		69 Use PasteAction to actually paste
		22 May 2001		drd		69 Created

	Note:
		We can't handle PICT because PhotoPrintItem needs an alias
*/

#include "PasteCommand.h"

#include "ModelAction.h"
#include "PhotoExceptionHandler.h"
#include "PhotoPrintDoc.h"
#include "PhotoPrintConstants.h"
#include "PhotoPrintResources.h"
#include "XMLHandleStream.h"
#include "XMLItemParser.h"

#include "EPostAction.h"
#include "EUserMessageServer.h"

#include "xmlinput.h"

const	ResIDT	TEXT_XMLParseClipWarning 	= 1143;
const	ResIDT	TEXT_XMLClipError			= 1144;

class PasteItemParser : public XMLItemParser

	{
	
		PasteCommand*	mCommand;
		
	public:
	
						PasteItemParser		(XML::Input&	inInput,
											 EUserMessage&	inMessage,
											 PasteCommand*	inCommand)
											: XMLItemParser (inInput, inMessage), mCommand (inCommand) 
											{}

		virtual	void	OnItemRead			(PhotoItemRef 	inItem)
											{ mCommand->OnItemRead (inItem); }
	};


/*
PasteCommand
*/
PasteCommand::PasteCommand(
	const CommandT	inCommand, 
	PhotoPrintDoc*	inDoc)
	: PhotoDocCommandAttachment(inCommand, inDoc)
{
} // PasteCommand

/*
~PasteCommand
*/
PasteCommand::~PasteCommand()
{
} // ~PasteCommand

/*
ExecuteCommand {OVERRIDE}
	Undo-ably paste
*/
void		
PasteCommand::ExecuteCommand(void*				/*inCommandData*/)
{
	StHandleBlock		theData(0L);			// Empty handle to get scrap data
	UScrap::GetData(kXMLFlavor, theData);		// May throw
	
	EPostAction			postAction (mDoc);
	
	try { postAction = new ModelAction (mDoc, si_PasteImage); } catch (...) {}

	StDisableDebugThrow_();
	StDisableDebugSignal_();

	try { 
		XMLHandleStream		stream(theData.Release());	// LHandleStream assumes ownership of the handle
		XML::Input			in(stream);
		EUserMessage		parseMessage (TEXT_XMLParseClipWarning, kCautionIcon, ExceptionHandler::GetCurrentHandler ()->GetOperation ());
		
		PasteItemParser (in, parseMessage, this).ParseObjects ();
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

	PhotoPrintView*		view (mDoc->GetView ());
	view->Refresh();								// Doc orientation may change, so refresh before AND after
	view->GetLayout()->LayoutImages();
	view->Refresh();

} // ExecuteCommand										 

/*
FindCommandStatus {OVERRIDE}
	Sets its argument to TRUE if the command should be enabled
*/
void		
PasteCommand::FindCommandStatus(SCommandStatus* ioStatus)
{
	*ioStatus->enabled = UScrap::HasData(kClipFlavor);
} // FindCommandStatus

/*
OnItemRead
	This function handles the "photo" tag in our XML file, which represents a single image
*/
void
PasteCommand::OnItemRead(PhotoItemRef	item) {

	PhotoPrintView*		view (mDoc->GetView ());
	view->SetupDraggedItem(item);
	view->GetLayout()->AddItem(item, view->GetModel ()->end ());
	
} // OnItemRead

