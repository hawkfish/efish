/*
	File:		BetaDialog.cp

	Contains:	Implementation of the beta Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

        <19>    11/14/01    rmgw    Soup up a la Color Pal.
        <18>     11/1/01	rmgw    Add Initialize.
        <17>    10/29/01    rmgw    Flip IsTimeLimited to IsExpired.
				15 Oct 2001	drd		Debug build expires 30 Nov, release build doesn't expire
				14 Sep 2001	drd		Expires 31 Oct 2001
				07 Sep 2001	drd		Expires 1 Oct 2001
				20 Aug 2001	drd		Expires 15 Sep 2001
				31 Jul 2001	drd		Expires 31 Aug 2001
				21 Jun 2001	drd		Expires 31 Jul 2001
				23 may 2001 dml		expires 6/30/2001
				30 Apr 2001	drd		Expires 31 May 01
				4 feb 2001	dml		expires 4/30/00
				1 feb 2001  dml		expire 2/31/01
				01 dec 2000 dml		expire 1/31/00
				14 sep 2000 dml		expire 12/1/00
         <3>      2/4/00    rmgw    Expire 1 April.
         <2>    12/15/99    rmgw    Expire 2/1/00.
         <1>     12/9/99    rmgw    first checked in.
        <1+>     12/9/99    rmgw    Factor registration from dialog.
         <1>     12/8/99    rmgw    first checked in.
*/

#include "Registration.h"

#include "MResFile.h"

#pragma mark BetaDialog

//	=== Constants ===

const	ResIDT		PPob_RegistrationDialog		= 1300;

const	PaneIDT		pane_OK						= 'ok  ';
const	PaneIDT		pane_Download				= 'down';

const	MessageT	msg_Download				= -1301;

#include "EURLDialogHandler.h"

class BetaDialog 	: public EURLDialogHandler 
	{

	protected:
	
		void					SetupGUI				(void);

	public:
		
								BetaDialog		(LCommander*		inSuper,
														 short				inEventMask = everyEvent);
		virtual					~BetaDialog		(void);
		
		virtual	Boolean			Run						(void);
	};

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ BetaDialog
// ---------------------------------------------------------------------------

BetaDialog::BetaDialog (
	
	LCommander*		inSuper,
	short			inEventMask)
	
	: EURLDialogHandler (PPob_RegistrationDialog, inSuper, inEventMask)
	
	{ // begin BetaDialog		

		UReanimator::LinkListenerToBroadcasters (this, GetDialog (), PPob_RegistrationDialog);

		SetupURL (pane_Download);
		
	} // end BetaDialog
	
// ---------------------------------------------------------------------------
//		¥ ~BetaDialog
// ---------------------------------------------------------------------------

BetaDialog::~BetaDialog (void)

	{ // begin ~BetaDialog
		
	} // end ~BetaDialog
	
// ---------------------------------------------------------------------------
//		¥ Run
// ---------------------------------------------------------------------------

Boolean	
BetaDialog::Run (void)
	
	{ // begin Run
		
		//	Show the dialog
		GetDialog ()->Show ();
		
		for (;;) {
			switch (DoDialog ()) {
				case msg_Cancel:
				case cmd_Close:
				case msg_OK:
					return false;
					
				case msg_Download:
					OnURL (pane_Download);
					break;
				} // switch
			} // for
			
	} // end Run

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ Initialize
// ---------------------------------------------------------------------------

Boolean
Registration::Initialize (void) 
	
	{ // begin Initialize
		
		return true;
		
	} // end Initialize
	
// ---------------------------------------------------------------------------
//		¥ IsExpired
// ---------------------------------------------------------------------------

Boolean
Registration::IsExpired (void) 

	{ // begin IsExpired

		typedef	unsigned	long	t_secs;
		
		t_secs						nowSecs;
		t_secs						expireSecs;
		DateTimeRec					expire;
		
		//	Check the times
		::GetDateTime (&nowSecs);

		expire.year = 2001;
		expire.month = 11;
		expire.day = 30;
		expire.hour = 0;
		expire.minute = 0;
		expire.second = 0;
		::DateToSeconds (&expire, &expireSecs);
		
		return (nowSecs >= expireSecs);

	} // end IsExpired
	
// ---------------------------------------------------------------------------
//		¥ IsRegistered
// ---------------------------------------------------------------------------

Boolean
Registration::IsRegistered (void)
	
	{ // begin IsRegistered
		
		return true;
		
	} // end IsRegistered

// ---------------------------------------------------------------------------
//		¥ TestSerialNumber
// ---------------------------------------------------------------------------

Boolean
Registration::TestSerialNumber (

	StringPtr	)
	
	{ // begin TestSerialNumber
		
		return true;
		
	} // end TestSerialNumber
	
// ---------------------------------------------------------------------------
//		¥ RegisterSerialNumber
// ---------------------------------------------------------------------------
//	Nop

void
Registration::RegisterSerialNumber (

	StringPtr	)
	
	{ // begin RegisterSerialNumber
		
	} // end RegisterSerialNumber
	
// ---------------------------------------------------------------------------
//		¥ GetSerialNumber
// ---------------------------------------------------------------------------

Boolean
Registration::GetSerialNumber (

	StringPtr	)
	
	{ // begin GetSerialNumber
		
		return false;
		
	} // end GetSerialNumber
	
// ---------------------------------------------------------------------------
//		¥ DoStartupDialog
// ---------------------------------------------------------------------------

Boolean
Registration::DoStartupDialog (
	
	LCommander*		inSuper,
	UInt32			/* inNotYetTicks */,
	short			inEventMask)
	
	{ // begin DoStartupDialog		
		
		if (!IsExpired ()) return true;
		
		return BetaDialog (inSuper, inEventMask).Run ();
		
	} // end DoStartupDialog

// ---------------------------------------------------------------------------
//		¥ DoPurchaseDialog
// ---------------------------------------------------------------------------

Boolean
Registration::DoPurchaseDialog (

	LCommander*)
	
	{ // begin DoPurchaseDialog		
		
		return IsRegistered ();
		
	} // end DoPurchaseDialog
