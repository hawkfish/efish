#include <Balloons.h>
#include <LCheckBox.h>
#include <LRadioButton.h>
#include <LCheckBoxGroupBox.h>
#include <LPopupGroupBox.h>
#include <LPeriodical.h>
#include <LEventDispatcher.h>
#include <LWindow.h>
#include <TArrayIterator.h>
#include <LStream.h>

#include "ABalloon.h"

#if TARGET_API_MAC_CARBON
	#error this implementation is not for Carbon.
#endif

#ifndef NEW
	#define	NEW	new
#endif

#if !__option(RTTI)
	#error you must compile with RTTI for this to work right
#endif

/*	---------------------------------------------------------------------------------------------
	class ABalloon
	
	Author:		James W. Walker <mailto:jw@jwwalker.com>
				<http://www.jwwalker.com/>
	
	Purpose:	PowerPlant attachment class to provide balloon help
				under Mac OS 7/8/9, and Carbon Help (AKA help tags)
				under Carbon.
			
	Usage:		Attach an ABalloon to each pane (usually a control)
				for which you want to provide help.
				Specify a STR# ID and a string index for each state
				(on or off, enabled or disabled).
				And remember to register ABalloon at startup.
				You must compile with RTTI for proper function.
				If you want a balloon to pop up automatically when
				the mouse is motionless over a pane for a certain time,
				you must call EnableAutoPop().
				If you want the help to appear without delay when the
				control key is pressed, you must call EnableControlKeyPop().

	Copyright:	ABalloon is ©1998, 2000 by James W. Walker.
				Permission is granted for use of ABalloon
				free of charge, other than acknowledgement of
				James W. Walker in any program using ABalloon
				(perhaps in an About box or in accompanying
				documentation). This copyright notice must remain
				in the source even if you modify it.
				If you contribute improvements which I incorporate
				into ABalloon, I will acknowledge your contribution,
				but I will retain sole copyright.
	
	History:
				1.0  2/28/98		First release.
				1.1  4/11/98
						• Don’t show balloons for invisible panes
						  (Thanks, Daniel Chiaramello, for the correction)
						• Rolled ABalloonHelper::CreateABalloonHelper into
						  ABalloonHelper::AddABalloon.
						• sBalloonAttachments is now a TArray<ABalloon*>
						  instead of TArray<LAttachment*>.
						• In the HMShowBalloon call, use kHMRegularWindow
						  instead of kHMSaveBitsNoWindow, because IM says
						  that the former should normally be used.
				1.2  6/7/98...6/28/98
						• New feature of automatically showing a balloon when
						  the cursor is motionless over a pane for a certain
						  length of time.  (Thanks to Fabrice for getting me
						  started on this and helping to implement it.)
						• New subclass that automatically shows a balloon even
						  if you have not globally enabled automatic balloons.
						• Added pragmas to get rid of annoying TArray warnings.
						• Made the TArray an ordinary member rather than a
						  static member of ABalloonHelper.
						• Changed the balloon mode to kHMSaveBitsWindow to fix
						  an update problem when a balloon was removed by the
						  appearance of a modal dialog.
				1.3  10/3/98, 10/25/98
						• New function CalcExposedPortFrame used in FindBalloonPane.
						  This is so that a pane inside a scrolling view will
						  show its balloon even if it is only partially visible.
						  Thanks to Rainer Brockerhoff for reporting this problem.
						• Implemented an option to make balloons appear immediately
						  when the control key is down.  This was contributed by
						  Bill Modesitt.
				1.4  11/27/98
						• Added the helper class ABalloonTip, which controls the
						  location of the balloon tip and the variant of the
						  balloon window.
						• Added another special case to provide good tip
						  placement for LCheckBoxGroupBox.
				1.5	 3/2/99-3/26/99
						• Modified ABalloonHelper::SpendTime so that it would
						  not continually call FindBalloonPane unless the mouse
						  position or the state of the control key is changing.
						• Treat LPopupGroupBox like LCheckBoxGroupBox.
				1.6  3/25/2000
						• Replaced Int16 type by SInt16
						• Added more #includes so it would compile without
						  precompiled PowerPlant headers
				1.7	 4/16/2000
						• Improved control key behavior, e.g., if you start pressing
						  the control key when the cursor is not over a window, and
						  then move the cursor over an item with a balloon, a balloon
						  will appear.  This was contributed by Richard Falk.
						• In FindBalloonPane, the order of the loop has been reversed.
						  This is so that if two panes overlap, neither containing the
						  other in the view hierarchy, then the frontmost pane will
						  control the balloon for the area of overlap.
						• In ABalloon::ShowBalloon, removed the test of the string
						  list ID.  As Richard Falk pointed out, this was unnecessary.
						• Added a few static casts to avoid warnings about implicit
						  conversions.
				2.0b1 7/10/2000
						• Simplified the public interface so that the Carbon and
						  Classic versions could have the same header.
						• The new function CalcPortExposedFrame, replacing
						  ABalloon::CalcExposedPortFrame, gives a smaller hot
						  rectangle for LCheckboxGroupBox and LPopupGroupBox.
				2.0b2 10/8/2000
				2.0b3 11/2/2000
						• No change in Classic implementation.
				2.0b4 11/17/2000
						• Manfred Schubert pointed out the little event loop when
						  showing a balloon did not give any time to cooperative
						  threads.  So now I imitate more of what LApplication::
						  ProcessNextEvent does, giving idle time to idlers, repeaters,
						  and event attachments.
				2.0b5 12/10/2000
						• Overhauled class hierarchy.  There is now an abstract base
						  class ABalloonBase, making it easier to write a subclass that
						  finds its help text any way you want.
						• Rearranged the code so that is basically the same as the
						  code in ABalloon-Carbon.cp, minus the Carbon-specific code.
						• Changed back to the kHMRegularWindow balloon method.  See
						  comments in ABalloonHelperBalloon::ShowBalloon.
	---------------------------------------------------------------------------------------------
*/
#pragma mark ====== ABalloonHelper declaration

#pragma warn_hidevirtual off
template class TArray<ABalloon*>;
#pragma warn_hidevirtual reset

#pragma mark class ABalloonHelper
/*	---------------------------------------------------------------------------------------------
	class ABalloonHelper
	
	This singleton object is created the first time an ABalloon is created,
	and maintains a list of all ABalloon objects.
	---------------------------------------------------------------------------------------------
*/
class ABalloonHelper  : public LPeriodical
{
public:
	static void				Initialize();
	
	
	
							ABalloonHelper() {}
					
	virtual					~ABalloonHelper();
	
	virtual void			AddABalloon( ABalloonBase* inABalloon );
	virtual void			RemoveABalloon( ABalloonBase* inABalloon );
	
	virtual	void			StartWatchingControlKey() {}
	virtual	void			StopWatchingControlKey() {}

	ABalloonBase*			FindBalloonPane(
									const Point& inGlobalPoint,
									WindowRef inWindow );

	static bool				CalcPortHotRect( LPane* inPane, Rect& outFrame );
	
	
	static 	ABalloonHelper*	sTheHelper;		// the single object of this class
	
	TArray<ABalloonBase*>	mBalloonAttachments;	// all ABalloon instances
	
	static	bool	sAutoPop;
	static	UInt32	sAutoPopTicks;
	static	bool	sControlKeyPop;
};

#pragma mark class ABalloonHelperBalloon
/*	---------------------------------------------------------------------------------------------
	class ABalloonHelperBalloon
	
	This version of ABalloonHelper is used when running under CarbonLib 1.0.x,
	where Carbon Help is available but buggy.  We will implement help using
	good old balloon help.
	---------------------------------------------------------------------------------------------
*/
class ABalloonHelperBalloon : public ABalloonHelper
{
public:
							ABalloonHelperBalloon();
					
	virtual	void			SpendTime(
									const EventRecord		&inMacEvent);
	
	void					DoBalloon(
									ABalloonBase*			inABalloon );

	void					ShowBalloon(
									ABalloonBase*			inABalloon,
									LPane*					inHostPane,
									ConstStringPtr			inHelpText,
									const Rect&				inAltRect );

	void					ComputeBalloonTip(
									LPane*					inHostPane,
									const Rect&				inAltRect,
									Point&					outTip,
									BalloonVariant&			outBalloonType );
																		
	
	ABalloonBase*			mLastBalloon;
	LStr255					mLastText;
};

// Define and initialize static variables
ABalloonHelper*			ABalloonHelper::sTheHelper = nil;
bool					ABalloonHelper::sAutoPop = false;
bool					ABalloonHelper::sControlKeyPop = false;
UInt32					ABalloonHelper::sAutoPopTicks = 60;

const UInt32 kMillisecondsPerTick = 17;


#pragma mark -
#pragma mark ====== ABalloonHelper code

ABalloonHelper::~ABalloonHelper()
{
	sTheHelper = nil;
}

void	ABalloonHelper::Initialize()		// [static]
{
	if (sTheHelper == nil)
	{
		sTheHelper = NEW ABalloonHelperBalloon();
	}
}

void	ABalloonHelper::AddABalloon( ABalloonBase* inABalloon )
{
	mBalloonAttachments.InsertItemsAt( 1, LArray::index_Last, inABalloon );
}

void	ABalloonHelper::RemoveABalloon( ABalloonBase* inABalloon )
{
	mBalloonAttachments.Remove( inABalloon );
	
	if (mBalloonAttachments.GetCount() == 0)
	{
		delete this;
	}
}

/*	---------------------------------------------------------------------------------------------
	ABalloonHelper::FindBalloonPane
	
	Look for a visible pane in an active window that contains the given
	point in global coordinates and has an attached ABalloon.
	---------------------------------------------------------------------------------------------
*/
ABalloonBase*	ABalloonHelper::FindBalloonPane( const Point& inGlobalPoint,
									WindowRef inWindow )
{
	ABalloonBase*	resultABalloon = nil;
	
	if (inWindow != nil)
	{
		LWindow	*theWindow = LWindow::FetchWindowObject(inWindow);
		if ( (theWindow != nil) &&
			theWindow->IsActive() &&
			theWindow->IsEnabled() )
		{
			Point	portMouse = inGlobalPoint;
			theWindow->GlobalToPortPoint(portMouse);
			
			// We have an array of all active ABalloons. Of those
			// attached to subpanes of this window, we want to find
			// the deepest one containing the point.
			Rect		theBestFrameSoFar;
			TArrayIterator<ABalloonBase*>	iterator( mBalloonAttachments,
				LArrayIterator::from_End );
			ABalloonBase*	theAttachment;
			
			// We iterate in reverse order so that if two panes overlap
			// (without one containing the other in the view hierarchy)
			// then the frontmost one (later in the list) will control
			// the balloon on the area of overlap.
			while (iterator.Previous(theAttachment))
			{
				LPane* ownerPane = dynamic_cast<LPane*>(
					theAttachment->GetOwnerHost() );
				Rect	ownerFrame;
				if ( (ownerPane != nil) &&
					ownerPane->IsVisible() &&		// 4/10/98
				#if __PowerPlant__ >= 0x02100000
					(ownerPane->GetMacWindow() == inWindow) &&
				#else
					(ownerPane->GetMacPort() == inWindow) &&
				#endif
					CalcPortHotRect( ownerPane, ownerFrame ) &&
					::PtInRect( portMouse, &ownerFrame ) )
				{
					// The current frame is completely enclosed in
					// theBestFrameSoFar just in case their union
					// equals theBestFrameSoFar.
					Rect	theUnion;
					::UnionRect( &theBestFrameSoFar, &ownerFrame, &theUnion );
					
					if ( (resultABalloon == nil) ||
						::EqualRect( &theBestFrameSoFar, &theUnion )
					)
					{
						resultABalloon = theAttachment;
						theBestFrameSoFar = ownerFrame;
					}
				}
			} // end of loop over ABalloons
		}
	}
	
	return resultABalloon;
}

/*	---------------------------------------------------------------------------------------------
	ABalloonHelper::CalcPortHotRect					[static]
	
	Normally, the whole content of the pane should be the "hot rectangle".
	But in some cases we want to use a smaller rectangle, for instance with
	LCheckBoxGroupBox it is better if only the checkbox part is active.
	Note: It is possible that this will make the rectangle empty.
	---------------------------------------------------------------------------------------------
*/
bool	ABalloonHelper::CalcPortHotRect( LPane* inPane, Rect& outFrame )
{
	bool	gotFrame = inPane->CalcPortExposedRect( outFrame );
	if (gotFrame)
	{
		if ( (nil != dynamic_cast<LCheckBoxGroupBox*>(inPane)) ||
			(nil != dynamic_cast<LPopupGroupBox*>(inPane)) )
		{
			// Find the line height for the control's font.
			FontInfo	metrics;
			{
				LControlView*	theView = dynamic_cast<LControlView*>( inPane );
				ResIDT	traitsID = theView->GetTextTraitsID();
				StTextState	saveFont;
				StColorState	saveColor;
				UTextTraits::SetPortTextTraits( traitsID );
				::GetFontInfo( &metrics );
			}
			Rect	portFrame;
			inPane->CalcPortFrameRect( portFrame );
			portFrame.bottom = portFrame.top + metrics.ascent + metrics.descent;
			portFrame.left += 10;	// just a guess
			portFrame.right -= 10;
			
			::SectRect( &outFrame, &portFrame, &outFrame );
		}
	}
	return gotFrame;
}

#pragma mark -

ABalloonHelperBalloon::ABalloonHelperBalloon()
	: mLastBalloon( nil )
{
	StartRepeating();
}

/*	---------------------------------------------------------------------------------------------
	ABalloonHelperBalloon::SpendTime
	
	This function determines which ABalloon attachment, if any, should
	show its balloon.
	---------------------------------------------------------------------------------------------
*/
void	ABalloonHelperBalloon::SpendTime( const EventRecord	&inMacEvent )
{
	static UInt32	sPrevWhen = 0;
	static Point	sPrevWhere = {-32000, -32000};
	static bool		sPrevControlKeyDown = false;
	static bool		sControlKeyBalloonMode = false;
	static bool		sOverBalloonPane = false;
	
	// I would have thought that looking at inMacEvent.modifiers
	// would suffice to detect the control key, but it seems to
	// be wrong for mouse-moved events.
	KeyMap			theKeyMap;
	::GetKeys( theKeyMap );
	bool			curControlKeyDown = ((theKeyMap[1] & 0x0008) != 0);
	
	// If there has been no change in mouse position or control key
	// state since the last time we checked, and at that time the
	// mouse was not over a pane with an ABalloon, then let's not
	// keep calling FindBalloonPane.
	if ( (inMacEvent.where.h != sPrevWhere.h) ||
		(inMacEvent.where.v != sPrevWhere.v) ||
		(curControlKeyDown != sPrevControlKeyDown) ||
		sOverBalloonPane )
	{
		WindowPtr	macWindowP;
		::FindWindow( inMacEvent.where, &macWindowP );	// is the point over a window?

		ABalloonBase* theABalloon = nil;
		if (macWindowP != nil)
		{
			theABalloon = FindBalloonPane( inMacEvent.where, macWindowP );
		}
		sOverBalloonPane = (theABalloon != nil);
		
		if (sControlKeyPop)
		{
			if ( curControlKeyDown && (theABalloon != nil) )
			{
				if ( !::HMGetBalloons() )
				{
					::HMSetBalloons(true);
					sControlKeyBalloonMode = true;
				}
			}
			else	// control key not down or not over a ballooned area
			{
				if (sControlKeyBalloonMode)
				{
					::HMSetBalloons(false);
					sControlKeyBalloonMode = false;
					sPrevWhen = inMacEvent.when;	// reset autopop timer
				}
			}
		}
		
		if (theABalloon)
		{
			if (::HMGetBalloons())	// balloons turned on manually?
			{
				DoBalloon( theABalloon );
			}
			else // balloons aren't on, but we may need to show one automatically
			{
				if ( sAutoPop || theABalloon->GetAutomatic() )
				{
					// If the mouse has moved, reset the timer.
					if ( (sPrevWhere.h != inMacEvent.where.h) ||
					 	 (sPrevWhere.v != inMacEvent.where.v) ||
					 	 (inMacEvent.what != nullEvent)
					 )
					{
						sPrevWhen = inMacEvent.when;
					}
					
					if ( inMacEvent.when >= sPrevWhen + sAutoPopTicks )
					{
						// Show a balloon, and keep showing it until
						// something happens.
						::HMSetBalloons(true);
						StopRepeating();	// prevent recursion
						DoBalloon( theABalloon );
						EventRecord theEvent;
						do
						{
							// Check for any events, but only dispatch null
							// events.  Note that this does give background
							// applications a little time.
							LEventDispatcher*	dispatcher =
								LEventDispatcher::GetCurrentEventDispatcher();
							
							Boolean	gotEvent = ::EventAvail( everyEvent, &theEvent );
							
							if (!gotEvent)
							{
								if (dispatcher->ExecuteAttachments(msg_Event, &theEvent))
								{
									dispatcher->UseIdleTime( theEvent );
								}
								
								LPeriodical::DevoteTimeToRepeaters( theEvent );
							}
						}
						while (	(theEvent.what == nullEvent) &&
								(sPrevWhere.h == theEvent.where.h) &&
				 	 			(sPrevWhere.v == theEvent.where.v) );
						sPrevWhere.v = 0x7FFF; //it will be a total reset
						::HMSetBalloons(false);
						StartRepeating();
					}
				}
			}
		}
	}
	sPrevControlKeyDown = curControlKeyDown;
	sPrevWhere = inMacEvent.where;
}

/*	---------------------------------------------------------------------------------------------
	ABalloonHelperBalloon::ShowBalloon
	---------------------------------------------------------------------------------------------
*/
void	ABalloonHelperBalloon::ShowBalloon(
									ABalloonBase*			inABalloon,
									LPane*					inHostPane,
									ConstStringPtr			inHelpText,
									const Rect&				inAltRect )
{
	if (::HMIsBalloon() &&
		(mLastBalloon == inABalloon) &&
		(mLastText == inHelpText) )
	{
		// already showing it, nothing to do
	}
	else
	{
		HMMessageRecord		theMessageRec;
		theMessageRec.hmmHelpType = khmmString;
		LString::CopyPStr( inHelpText, theMessageRec.u.hmmString );
		
		mLastText = inHelpText;
		mLastBalloon = inABalloon;

		Point	theTip;
		BalloonVariant	theVariant;
		ComputeBalloonTip( inHostPane, inAltRect, theTip, theVariant );
		
		::HMShowBalloon( &theMessageRec,
			theTip,
			const_cast<Rect*>(&inAltRect),
			nil, 0,
			theVariant,
			kHMRegularWindow );
		
		// I'm not sure of the ideal method parameter to pass to HMShowBalloon.
		// Inside Mac says that kHMRegularWindow should normally be used.
		// Back in '98 I observed that is a balloon is removed by the appearance
		// of a modal dialog, and that dialog does not handle update events for
		// other windows, then the area that was covered by the balloon will remain
		// in need of an update until the dialog goes away.  I dealt with the
		// problem by switching to kHMSaveBitsWindow, but I think now that may
		// have been the wrong decision.
		
		// There's definitely a problem with kHMSaveBitsNoWindow: Suppose balloon
		// help is turned on, and the mouse is over a radio button in the off state,
		// displaying a balloon.  If you click on the radio button, then the visual
		// state of the window changes, and also the balloon changes.  When the
		// off-state balloon goes away, the Help Manager restores bits that are out
		// of date.  Furthermore, bits are saved and restored for the whole bounding
		// rectangle of the balloon, so you may see some dirty bits even if a new
		// balloon appears immediately.
		
		// kHMSaveBitsWindow seems like it ought to be the safest option, like
		// keeping your pants up using a belt AND suspenders, but it has a fatal flaw.
		// The Help Manager saves and restores bits for the entire bounding rectangle
		// of the balloon, whereas the disappearance of the balloon generates an
		// update event only for the irregular balloon shape.
	}
}

/*	---------------------------------------------------------------------------------------------
	ABalloonHelperBalloon::ComputeBalloonTip
	
	Determine the balloon variant and the location of the balloon tip.
	---------------------------------------------------------------------------------------------
*/
void	ABalloonHelperBalloon::ComputeBalloonTip(
									LPane*					inHostPane,
									const Rect&				inAltRect,
									Point&					outTip,
									BalloonVariant&			outBalloonType )
{
	
	
	if ( (nil != dynamic_cast<LRadioButton*>(inHostPane)) ||
		(nil != dynamic_cast<LCheckBox*>(inHostPane))
	)
	{
		outBalloonType = kBottomRightTipPointsDownVariant;
		
		outTip.v = static_cast<short>((inAltRect.top + inAltRect.bottom) / 2);
		outTip.h = static_cast<short>(inAltRect.left + 6);
	}
	else if ( (nil != dynamic_cast<LCheckBoxGroupBox*>(inHostPane)) ||
		(nil != dynamic_cast<LPopupGroupBox*>(inHostPane))
	)
	{
		outBalloonType = kBottomRightTipPointsDownVariant;
		
		outTip.v = static_cast<short>(inAltRect.top + 10);
		outTip.h = static_cast<short>(inAltRect.left + 16);
	}
	else
	{
		outBalloonType = kTopLeftTipPointsLeftVariant;
		
		outTip.v = static_cast<short>((inAltRect.top + inAltRect.bottom) / 2);
		outTip.h = static_cast<short>(inAltRect.right - 4);
	}
	
}

/*	---------------------------------------------------------------------------------------------
	ABalloonHelperBalloon::DoBalloon
	---------------------------------------------------------------------------------------------
*/
void	ABalloonHelperBalloon::DoBalloon( ABalloonBase*	inABalloon )
{
	LPane*	thePane = dynamic_cast<LPane*>( inABalloon->GetOwnerHost() );
	if (thePane != nil)
	{
		Rect	theFrame;
		
		CalcPortHotRect( thePane, theFrame );
		thePane->PortToGlobalPoint( topLeft(theFrame) );
		thePane->PortToGlobalPoint( botRight(theFrame) );
		
		Str255	helpText;
		inABalloon->GetHelpString( thePane, helpText );
		
		ShowBalloon( inABalloon, thePane, helpText, theFrame );
	}
}


#pragma mark -

ABalloonBase::ABalloonBase( LStream *inStream )
	: LAttachment( inStream ),
	mAutomatic( false )
{
	// I don't want to have to bother filling this out in
	// Constructor every time.
	mMessage = msg_Nothing;
		
	ABalloonHelper::Initialize();
	ABalloonHelper::sTheHelper->AddABalloon( this );
}

ABalloonBase::~ABalloonBase()
{
	ABalloonHelper::sTheHelper->RemoveABalloon( this );
}

/*	---------------------------------------------------------------------------------------------
	ABalloonBase::GetAutomatic
	---------------------------------------------------------------------------------------------
*/
bool	ABalloonBase::GetAutomatic() const
{
	return mAutomatic;
}

/*	---------------------------------------------------------------------------------------------
	ABalloonBase::SetAutomatic
	---------------------------------------------------------------------------------------------
*/
void	ABalloonBase::SetAutomatic( bool inAutoMode )
{
	mAutomatic = inAutoMode;
}

/*	---------------------------------------------------------------------------------------------
	ABalloonBase::EnableAutoPop				[static]
	---------------------------------------------------------------------------------------------
*/
void	ABalloonBase::EnableAutoPop( void )
{
	ABalloonHelper::sAutoPop = true;
}

/*	---------------------------------------------------------------------------------------------
	ABalloonBase::DisableAutoPop			[static]
	---------------------------------------------------------------------------------------------
*/
void	ABalloonBase::DisableAutoPop( void )
{
	ABalloonHelper::sAutoPop = false;
}

/*	---------------------------------------------------------------------------------------------
	ABalloonBase::SetAutoPopDelay			[static]
	---------------------------------------------------------------------------------------------
*/
void	ABalloonBase::SetAutoPopDelay( UInt32 inDelayTicks )
{
	ABalloonHelper::sAutoPopTicks = inDelayTicks;
}

/*	---------------------------------------------------------------------------------------------
	ABalloonBase::EnableControlKeyPop			[static]
	---------------------------------------------------------------------------------------------
*/
void	ABalloonBase::EnableControlKeyPop( void )
{
	if (!ABalloonHelper::sControlKeyPop)
	{
		ABalloonHelper::sControlKeyPop = true;
		if (ABalloonHelper::sTheHelper != nil)
		{
			ABalloonHelper::sTheHelper->StartWatchingControlKey();
		}
	}
}

/*	---------------------------------------------------------------------------------------------
	ABalloonBase::DisableControlKeyPop			[static]
	---------------------------------------------------------------------------------------------
*/
void	ABalloonBase::DisableControlKeyPop( void )
{
	if (ABalloonHelper::sControlKeyPop)
	{
		ABalloonHelper::sControlKeyPop = false;
		if (ABalloonHelper::sTheHelper != nil)
		{
			ABalloonHelper::sTheHelper->StopWatchingControlKey();
		}
	}
}

#pragma mark ====== ABalloon code

// ABalloon::ABalloon	stream constructor.
ABalloon::ABalloon( LStream	*inStream)
	: ABalloonBase( inStream )
{
	*inStream >> mStringListID;
	*inStream >> mOffDisabledIndex;
	*inStream >> mOffEnabledIndex;
	*inStream >> mOnDisabledIndex;
	*inStream >> mOnEnabledIndex;
}

/*	---------------------------------------------------------------------------------------------
	ABalloon::GetHelpString
	
	Choose which string to display, according to the state of the pane.
	---------------------------------------------------------------------------------------------
*/
void	ABalloon::GetHelpString(
							LPane*			inPane,
							StringPtr		outString )
{
	SInt16	whichString;
	if (inPane->GetValue() == 0)
	{
		if (inPane->IsEnabled())
		{
			whichString = mOffEnabledIndex;
		}
		else
		{
			whichString = mOffDisabledIndex;
		}
	}
	else
	{
		if (inPane->IsEnabled())
		{
			whichString = mOnEnabledIndex;
		}
		else
		{
			whichString = mOnDisabledIndex;
		}
	}
	::GetIndString( outString, mStringListID, whichString );
}
