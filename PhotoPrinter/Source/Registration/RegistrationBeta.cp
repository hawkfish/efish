/*
	File:		RegistrationDialog.cp

	Contains:	Implementation of the Registration dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright �1999 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>      2/4/00    rmgw    Expire 1 April.
         <2>    12/15/99    rmgw    Expire 2/1/00.
         <1>     12/9/99    rmgw    first checked in.
        <1+>     12/9/99    rmgw    Factor registration from dialog.
         <1>     12/8/99    rmgw    first checked in.
*/


#include "Registration.h"

#include "MResFile.h"

#pragma mark RegistrationDialog

//	=== Constants ===

const	ResIDT		PPob_RegistrationDialog		= 1300;

const	PaneIDT		pane_OK						= 'ok  ';
const	PaneIDT		pane_Download				= 'down';

const	MessageT	msg_Download				= -1301;

#include "EURLDialogHandler.h"

class RegistrationDialog 	: public EURLDialogHandler 
	{

	protected:
	
		void					SetupGUI				(void);

	public:
		
								RegistrationDialog		(LCommander*		inSuper,
														 short				inEventMask = everyEvent);
		virtual					~RegistrationDialog		(void);
		
		virtual	Boolean			Run						(void);
	};

#pragma mark -

// ---------------------------------------------------------------------------
//		� RegistrationDialog
// ---------------------------------------------------------------------------

RegistrationDialog::RegistrationDialog (
	
	LCommander*		inSuper,
	short			inEventMask)
	
	: EURLDialogHandler (PPob_RegistrationDialog, inSuper, inEventMask)
	
	{ // begin RegistrationDialog		

		UReanimator::LinkListenerToBroadcasters (this, GetDialog (), PPob_RegistrationDialog);

		SetupURL (pane_Download);
		
	} // end RegistrationDialog
	
// ---------------------------------------------------------------------------
//		� ~RegistrationDialog
// ---------------------------------------------------------------------------

RegistrationDialog::~RegistrationDialog (void)

	{ // begin ~RegistrationDialog
		
	} // end ~RegistrationDialog
	
// ---------------------------------------------------------------------------
//		� Run
// ---------------------------------------------------------------------------

Boolean	
RegistrationDialog::Run (void)
	
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
//		� IsRegistered
// ---------------------------------------------------------------------------

Boolean
Registration::IsRegistered (void)
	
	{ // begin IsRegistered
		
		typedef	unsigned	long	t_secs;
		
		t_secs						nowSecs;
		t_secs						expireSecs;
		DateTimeRec					expire;
		
		//	Check the times
		::GetDateTime (&nowSecs);

		expire.year = 2000;
		expire.month = 1;
		expire.day = 1;
		expire.hour = 0;
		expire.minute = 0;
		expire.second = 0;
		::DateToSeconds (&expire, &expireSecs);
		
		return (nowSecs < expireSecs);
		
	} // end IsRegistered
	
// ---------------------------------------------------------------------------
//		� RegisterSerialNumber
// ---------------------------------------------------------------------------
//	Nop

void
Registration::RegisterSerialNumber (

	StringPtr	)
	
	{ // begin RegisterSerialNumber
		
	} // end RegisterSerialNumber
	
// ---------------------------------------------------------------------------
//		� RunDialog
// ---------------------------------------------------------------------------

Boolean
Registration::RunDialog (
	
	LCommander*		inSuper,
	UInt32			/* inNotYetTicks */,
	short			inEventMask)
	
	{ // begin RunDialog		
		
		if (IsRegistered ()) return true;
		
		return RegistrationDialog (inSuper, inEventMask).Run ();
		
	} // end RunDialog