// ===========================================================================
//	File:						UWindowStagger.cp
// Version:					1.0 - May 7, 1996
//	Author:					Mike Shields (mshields@inconnect.com)
//							
//	Copyright ©1996 Mike Shields. All rights reserved.
//	I hereby grant users of UWindowStagger permission to use it (or any modified 
//	version of it) in applications (or any other type of Macintosh software 
//	like extensions -- freeware, shareware, commercial, or other) for free, 
//	subject to the terms that:
//
//		(1)  This agreement is non-exclusive.
//
//		(2)  I, Mike Shields, retain the copyright to the original source code.
//
//	These two items are the only required conditions for use. However, I do have 
//	an additional request. Note, however, that this is only a request, and 
//	that it is not a required condition for use of this code.
//
//		(1) That I be given credit for UWindowStagger code in the copyrights or 
//			acknowledgements section of your manual or other appropriate documentation.
//
//
//	I would like to repeat that this last item is only a request. You are prefectly 
//	free to choose not to do any or all of them.
//	
//	    This source code is distributed in the hope that it will be useful,
//	    but WITHOUT ANY WARRANTY; without even the implied warranty of
//	    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// ===========================================================================
//	UWindowStagger.h		<- double-click + Command-D to see class declaration
//
// Static class which allows you to stagger windows on the desktop in an intelligent
// manner.
//
//	20 Sep 2000		drd		Carbonized; removed GetMonitorRect (now part of E++)

#include "UWindowStagger.h"

#include "EUtil.h"
#include <LWindow.h>
#include <UWindows.h>

SInt16	UWindowStagger::gSpaceAtLeft = 8;
SInt16	UWindowStagger::gSpaceBelowMenu = 22;

//---------------------------------------------------------------------------
// UWindowStagger::StaggerFromPosition()
//---------------------------------------------------------------------------
// This function places a window at a position which no other window occupies, 
// starting with the position passed in. If the calculated position is full, 
// we continue to offset until we find an open space.  If the space we end up 
// with does not allow the window to fit entirely on one monitor, we start 
// again at the upper left-hand corner of the monitor.
void UWindowStagger::StaggerFromPosition(LWindow* inWindow, Point inPosition, 
														short inDeltaH, short inDeltaV)
{
	Point			topLeftPosition;
	Rect			monitorRect;
	Point			newPosition = inPosition;
	
	// take into account the menu bar when determining where to put the first
	// window
	topLeftPosition.h = gSpaceAtLeft;
	topLeftPosition.v = gSpaceBelowMenu + ::GetMBarHeight();
	
	EUtil::GetMonitorRect(inWindow->GetMacWindow(), monitorRect);

	// check to see if we are not on the main screen. if that's the case, we need to
	// fix ourselves to NOT use the menubar height.
	if (topLeft(monitorRect).v != 0) {
		topLeftPosition.v += topLeft(monitorRect).v;
		topLeftPosition.v -= ::GetMBarHeight();
	}
	if (topLeft(monitorRect).h != 0) {
		topLeftPosition.h += topLeft(monitorRect).h;
	}

	Point	delta;
	delta.h = inDeltaH;
	delta.v = inDeltaV;
	UWindowStagger::MakePositionUnique(inWindow, newPosition, topLeftPosition, 
													delta, monitorRect);
	inWindow->DoSetPosition(newPosition);
}

//---------------------------------------------------------------------------
// UWindowStagger::Stagger()
//---------------------------------------------------------------------------
// Here, every window is, by default, offset from the position
// of the window behind it.  If the calculated position is full, we continue 
// to offset until we find an open space.  If the space we end up with does not 
// allow the window to fit entirely on one monitor, we start again at the upper 
// left-hand corner of the monitor.
void UWindowStagger::Stagger(LWindow* inWindow, short inDeltaH, short inDeltaV)
{
	Point			topLeftPosition;
	Point			newPosition;
	LWindow*		frontWindow;
	Rect			temp;
	Rect			monitorRect;
	Point			localTopLeft;
	
	// take into account the menu bar when determining where to put the first
	// window
	topLeftPosition.h = gSpaceAtLeft;
	topLeftPosition.v = gSpaceBelowMenu + ::GetMBarHeight();
		
	// find the front window and take note of its monitor rectangle
	frontWindow = UDesktop::FetchTopRegular();
	
	if (frontWindow == nil) {
		// if there is no front window, the new window will go in the upper left-hand
		// corner of the main monitor, defined above
		EUtil::GetMonitorRect(nil, monitorRect);
		newPosition = topLeftPosition;
		localTopLeft = topLeftPosition;
	} else {
		// if there is a front window, the new one will be offset from it by the
		// value of "delta" and the upper left-hand corner we use for positioning
		// will be that of the monitor containing the most of the current front
		// window (notice we have to adjust for the menu bar size)
		EUtil::GetMonitorRect(frontWindow->GetMacWindow(), monitorRect);
		temp = UWindows::GetWindowContentRect(frontWindow->GetMacWindow());
		newPosition = topLeft(temp);
		newPosition.h += inDeltaH;
		newPosition.v += inDeltaV;
		localTopLeft = topLeftPosition;
		if (topLeft(monitorRect).v != 0) 
		{
			localTopLeft.v += topLeft(monitorRect).v;
			localTopLeft.v -= ::GetMBarHeight();
		}
		if (topLeft(monitorRect).h != 0) 
		{
			localTopLeft.h += topLeft(monitorRect).h;
		}
	}
	
	Point	delta;
	delta.h = inDeltaH;
	delta.v = inDeltaV;
	UWindowStagger::MakePositionUnique(inWindow, newPosition, localTopLeft, 
													delta, monitorRect);
	inWindow->DoSetPosition(newPosition);
} // Stagger

//---------------------------------------------------------------------------
// MakePositionUnique
//---------------------------------------------------------------------------
// Find a position for the window which is not occupied by any other window
void UWindowStagger::MakePositionUnique(LWindow* inWindow, Point &inNewPosition, 
													Point inTopLeft, Point inDelta, 
													const Rect& inMonitorRect)
{
	Point			workPosition = inNewPosition;
	Point			tempTopLeft;
	
	// for as long as the proposed position is on the screen and another window
	// occupies this position, keep advancing down and to the right until we
	// either march off the screen or find an unoccupied position
	while (::PtInRect(workPosition, &inMonitorRect) && 
				!UWindowStagger::IsPositionUnique(workPosition)) {
		workPosition.h += inDelta.h;
		workPosition.v += inDelta.v;
	}
	
	// if we've marched off the screen or if the unoccupied position won't
	// allow the whole window to be on the screen, try again starting at the
	// monitor's upper left-hand corner
	if (!::PtInRect(workPosition, &inMonitorRect) || 
			!UWindowStagger::WillWindowFit(inWindow, workPosition, inMonitorRect)) {
		if (inNewPosition.v != inTopLeft.v || inNewPosition.h != inTopLeft.h) {
			workPosition = inTopLeft;
			if (!UWindowStagger::IsPositionUnique(workPosition)) {
				UWindowStagger::MakePositionUnique(inWindow, workPosition, inTopLeft, 
																inDelta, inMonitorRect);
				
				// if we can't find an unoccupied position that will hold the
				// window after starting at the upper left-hand corner of the
				// monitor, try again, but start halfway between the first two
				// window positions on the monitor (the test here causes
				// recursive calls to fall out if they don't find an acceptable
				// position)
				if (workPosition.v == inTopLeft.v && workPosition.h == inTopLeft.h) {
					workPosition.h += inDelta.h / 2;
					workPosition.v += inDelta.v / 2;
					
					if (!UWindowStagger::IsPositionUnique(workPosition)) {
						tempTopLeft = workPosition;
						UWindowStagger::MakePositionUnique(inWindow, workPosition, tempTopLeft, 
																		inDelta, inMonitorRect);
						
						// if that doesn't work either, give up and just put the
						// window in the upper left-hand corner of the monitor
						if (workPosition.v == tempTopLeft.v && workPosition.h == tempTopLeft.h)
							workPosition = inTopLeft;
					}
				}
			}
		} else
			workPosition = inNewPosition;
	}
	
	// return the new position
	inNewPosition = workPosition;
}

//---------------------------------------------------------------------------
// WillWindowFit()
//---------------------------------------------------------------------------
// determines whether the window will fit entirely within the specified monitorRect 
// if its upper left-hand corner is placed at "pos".  Returns true if the window will 
// fit there, and false if it won't.
Boolean UWindowStagger::WillWindowFit(LWindow* inWindow, Point inCurrentPosition, 
												const Rect& inMonitorRect)
{
	Point				windowBotRight;
	SDimension16	frameSize;
		
	// the insetting here takes into account the frame and drop shadow (we don't
	// care about the title bar, since we're always going down and to the right
	// and we know we started in a position where the title bar will work)
	inWindow->GetFrameSize(frameSize);
	windowBotRight.h = inCurrentPosition.h + frameSize.width - 2;
	windowBotRight.v = inCurrentPosition.v + frameSize.height - 2;
	
	return ::PtInRect(windowBotRight, &inMonitorRect);
}

//---------------------------------------------------------------------------
// IsPositionUnique
//---------------------------------------------------------------------------
// Returns true if no currently visible window in the Window Manager's window list 
// has its upper left-hand corner at the position specified by "pos".
Boolean UWindowStagger::IsPositionUnique(Point inPosition)
{
	WindowPtr		aWindowPtr;
	Rect				windowRect;

	for (short i = 1; (aWindowPtr = UWindows::FindNthWindow(i)) != nil; i++) {
		LWindow	*aWindow = LWindow::FetchWindowObject(aWindowPtr);

		// We only want to check visible and "normal" windows. No floating windows
		// should be checked against.
		if (aWindow->IsVisible() && aWindow->HasAttribute(windAttr_Regular)) {
			windowRect = UWindows::GetWindowContentRect(aWindowPtr);
			if (topLeft(windowRect).h == inPosition.h && topLeft(windowRect).v == inPosition.v)
				return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------
// UWindowStagger::ForceOnScreen
//---------------------------------------------------------------------------
// Forces the window to be completely visible on the screen
void UWindowStagger::ForceOnScreen(LWindow* inWindow)
{
	Rect				monitorRect;
	Point				windowBotRight = {0, 0};
	SDimension16		frameSize;
	
	EUtil::GetMonitorRect(inWindow->GetMacWindow(), monitorRect);

	// Calculate the bottom right edge of the window.
	inWindow->PortToGlobalPoint(windowBotRight);
	inWindow->GetFrameSize(frameSize);
	windowBotRight.h += frameSize.width;
	windowBotRight.v += frameSize.height;
	
	if (!::PtInRect(windowBotRight, &monitorRect)) {
		// the bottom right edge is off the screen, we need to adjust.
		
		Rect		currBounds = UWindows::GetWindowContentRect(inWindow->GetMacWindow());
		
		// First position the window so it's bottom right corner is on the screen.
		if (windowBotRight.v > monitorRect.bottom) {
			currBounds.top -= (windowBotRight.v - monitorRect.bottom);
			currBounds.bottom -= (windowBotRight.v - monitorRect.bottom);
		}
		
		if (windowBotRight.h > monitorRect.right) {
			currBounds.left -= (windowBotRight.h - monitorRect.right);
			currBounds.right -= (windowBotRight.h - monitorRect.right);
		}
		
		// now check to see if that makes the window's top, left go off the screen.
		// if that's the case, the window is simply too big to fit, so we make it
		// exactly the size of the monitor. We also check to see if we're on the main
		// screen (or not on any screen) and take into account the menu bar in that case.
		// We could also be nice and if we are on the main monitor, we could take off some
		// width for the disk icons, but for now we won't
		if (currBounds.top < monitorRect.top) {
			if (topLeft(monitorRect).v == 0) 
				currBounds.top = monitorRect.top + ::GetMBarHeight() + gSpaceBelowMenu;
			else
				currBounds.top = monitorRect.top + gSpaceBelowMenu;
			
			currBounds.bottom = monitorRect.bottom;
		}
		
		if (currBounds.left < monitorRect.left) {
			currBounds.left = monitorRect.left;
			currBounds.right = monitorRect.right;
		}
		
		inWindow->DoSetBounds(currBounds);
	}
} // ForceOnScreen
