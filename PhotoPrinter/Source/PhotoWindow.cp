/*
	File:		PhotoWindow.h

	Contains:	Subclass of LWindow with different zoom behavior

	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):
	
		20 Aug 2001		rmgw	Update menus after resize.  Bug #335.
		20 Aug 2001		rmgw	Carbon event tracking.  Bug #334.
		16 Aug 2001		rmgw	Add exception handling.  Bug #330.
		15 Aug 2001		rmgw	Hack CalcStandardBounds use page size and compensate for non-view stuff.
		23 Feb 2001		drd		Hack CalcStandardBounds to leave space at bottom
		12 Sep 2000 	dml		created
	
*/

#include "PhotoWindow.h"
#include "PhotoExceptionHandler.h"
#include "PhotoPrinter.h"
#include "PhotoPrintDoc.h"
#include "PhotoUtility.h"

#include "MRect.h"

// ---------------------------------------------------------------------------
//		¥ PhotoWindow 
// ---------------------------------------------------------------------------

PhotoWindow::PhotoWindow(LStream* inStream) 
	: LWindow(inStream)
	, mDoc (nil)
{
	
#if TARGET_API_MAC_CARBON
	static	EventTypeSpec	
	sEvents [] = {
					{kEventClassWindow, kEventWindowBoundsChanged}
				 };

	InstallEventHandler (::GetWindowEventTarget (GetMacWindow ()),
						 sWindowEventFilterProc,
						 GetEventTypeCount(sEvents), sEvents, this, 0);
#endif						 

}//end stream ct


// ---------------------------------------------------------------------------
//		¥ ~PhotoWindow 
// ---------------------------------------------------------------------------

PhotoWindow::~PhotoWindow()
{
}//end dt


/*
* PhotoWindow calculates the "standard" (zoom) bounds differently than LWindow
* for us, the max zoom size is only as big as able to show the full paper size at 72dpi
* plus a tiny amount of grey margin, not entire screen.
*/
Boolean 
PhotoWindow::CalcStandardBounds(Rect& ioDest) const {
	MRect		outDest (ioDest);  // convert to MRect for convenience funcs

	// this is biggest possible window on current display
	LWindow::CalcStandardBounds(outDest);
	
	// !!! This is a quick kludge. We take something out of the bottom to avoid the tool palette,
	// or the OS X dock. We should probably use GetWindowAvailablePositioningBounds, once it's
	// available
	outDest.bottom -= 32;
	
	// and if we are associated with a document, then we can calc based on print record
	if (mDoc != nil) {
		//	Calculate the margin
		SDimension16	margin;
		GetFrameSize (margin);
		margin.width += PhotoPrintDoc::kFeelGoodMargin;
		margin.height += PhotoPrintDoc::kFeelGoodMargin;
		
		SDimension16	viewFrameSize;
		mDoc->GetView ()->GetFrameSize (viewFrameSize);
		margin.width -= viewFrameSize.width;
		margin.height -= viewFrameSize.height;
		
		// base our size on the current page's size
		MRect	pageBounds;
		PhotoPrinter::CalculatePaperRect(mDoc->GetPrintRec(), &(mDoc->GetPrintProperties()), 
											 pageBounds, kDPI);		//all computations at 72dpi
		
		// now, size the window down based on the printable area
		// ideally, we can show both dimensions at 100%
		if (pageBounds.Height() <= outDest.Height()) {
			if (outDest.Height() - pageBounds.Height() > margin.height)
				outDest.SetHeight(pageBounds.Height() + margin.height);
			}//endif enough room to show height at 100%

		if (pageBounds.Width() <= outDest.Width()) {
			if (outDest.Width() - pageBounds.Width() > margin.width)
				outDest.SetWidth(pageBounds.Width() + margin.width);
			}//endif enough room to show width at 100%
		}//endif we are associated with a doc

	ioDest = outDest; // bounce out to Rect

	Rect		contentRect = UWindows::GetWindowContentRect(mMacWindowP);
	return ::MacEqualRect(&ioDest, &contentRect);
}//end CalcStandardBounds

// ---------------------------------------------------------------------------
//		¥ ObeyCommand 												  [public]
// ---------------------------------------------------------------------------

Boolean
PhotoWindow::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{
	Boolean		cmdHandled = true;	// Assume we'll handle the command

	// Setup for reporting on any exceptions that may occur
	MemoryExceptionHandler	commandHandler (inCommand);
	
	try {
		cmdHandled = LWindow::ObeyCommand(inCommand, ioParam);
		} // try

	catch (LException& e) {
		ExceptionHandler::HandleKnownExceptions (e);
		} // catch
		
	return cmdHandled;
}

#if TARGET_API_MAC_CARBON

// ---------------------------------------------------------------------------
//		¥ OnWindowBoundsChanged 								   [protected]
// ---------------------------------------------------------------------------
//	The standard window menu resizes the window and only informs us via a
//	Carbon event.  We can't just call LWindow::DoSetBounds because it calls
//	SizeWindow and I don't trust us not to get a message when nothing has 
//	changed

OSStatus	
PhotoWindow::OnWindowBoundsChanged (

	EventHandlerCallRef		/*inHandlerCallRef*/, 
	EventRef 				/*inEvent*/)

	{ // begin OnWindowBoundsChanged
		
		Rect	inBounds;
		::GetWindowBounds(GetMacWindow (), kWindowContentRgn, &inBounds);
		SendAESetBounds (&inBounds, ExecuteAE_No);

		//	Copied from LWindow::DoSetBounds to avoid reentrancy
		ResizeFrameTo((SInt16) (inBounds.right - inBounds.left),
					  (SInt16) (inBounds.bottom - inBounds.top), true);

		SDimension16	frameSize;		// For Windows, Image is always the
		GetFrameSize(frameSize);		//   same size as its Frame
		ResizeImageTo(frameSize.width, frameSize.height, false);

										// Changing Bounds establishes a
										//   new User state for zooming
		CalcPortFrameRect(mUserBounds);
		PortToGlobalPoint(topLeft(mUserBounds));
		PortToGlobalPoint(botRight(mUserBounds));
		mMoveOnlyUserZoom = false;
		
		SetUpdateCommandStatus (true);
		
		return noErr;
		
	} // end OnWindowBoundsChanged

// ---------------------------------------------------------------------------
//		¥ OnWindowEvent 										   [protected]
// ---------------------------------------------------------------------------

OSStatus	
PhotoWindow::OnWindowEvent (

	EventHandlerCallRef		inHandlerCallRef, 
	EventRef 				inEvent)

	{ // begin OnWindowEvent
		
		switch (GetEventKind (inEvent)) {
			case kEventWindowBoundsChanged:
				return OnWindowBoundsChanged (inHandlerCallRef, inEvent);
			} // switch
			
		return noErr;
		
	} // end OnWindowEvent

// ---------------------------------------------------------------------------
//		¥ WindowEventFilterProc 									  [static]
// ---------------------------------------------------------------------------

MUPP<EventHandlerProcPtr>	
PhotoWindow::sWindowEventFilterProc (WindowEventFilterProc);

pascal OSStatus	
PhotoWindow::WindowEventFilterProc (

	EventHandlerCallRef		inHandlerCallRef, 
	EventRef 				inEvent, 
	void *					inUserData)

	{ // begin WindowEventFilterProc
		
		PhotoWindow*	that = static_cast<PhotoWindow*> (inUserData);
		if (that) that->OnWindowEvent (inHandlerCallRef, inEvent);
			
		return::CallNextEventHandler(inHandlerCallRef, inEvent);
		
	} // end WindowEventFilterProc

#endif

