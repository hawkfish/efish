/*
	File:		RegistrationDemo.cp

	Contains:	Implementation of the beta Registration singleton.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    11/19/01    rmgw    Hacked from RegistrationBeta.
*/

#include "Registration.h"

#include "ERegistrationFile.h"

enum RegStrings {
	kRegStringsIllegalIndex = 0,
	
	kUnregisteredSNIndex,
	
	strn_Registration = 1300
	};

static const unsigned char
sRegFileName [] = "\pFinder EFK DB";


#pragma mark DemoDialog

//	=== Constants ===

const	ResIDT		PPob_RegistrationDialog		= 1300;

const	PaneIDT		pane_OK						= 'ok  ';
const	PaneIDT		pane_Download				= 'down';

const	MessageT	msg_Download				= -1301;

#include "EURLDialogHandler.h"

class DemoDialog 	: public EURLDialogHandler 

	{

	protected:
	
		void					SetupGUI		(void);

	public:
		
								DemoDialog		(LCommander*		inSuper,
												 short				inEventMask = everyEvent);
		virtual					~DemoDialog		(void);
		
		virtual	Boolean			Run				(void);
	};

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ DemoDialog
// ---------------------------------------------------------------------------

DemoDialog::DemoDialog (
	
	LCommander*		inSuper,
	short			inEventMask)
	
	: EURLDialogHandler (PPob_RegistrationDialog, inSuper, inEventMask)
	
	{ // begin DemoDialog		

		UReanimator::LinkListenerToBroadcasters (this, GetDialog (), PPob_RegistrationDialog);

		SetupURL (pane_Download);
		
	} // end DemoDialog
	
// ---------------------------------------------------------------------------
//		¥ ~DemoDialog
// ---------------------------------------------------------------------------

DemoDialog::~DemoDialog (void)

	{ // begin ~DemoDialog
		
	} // end ~DemoDialog
	
// ---------------------------------------------------------------------------
//		¥ Run
// ---------------------------------------------------------------------------

Boolean	
DemoDialog::Run (void)
	
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

		//	Find the registration file
		ERegistrationFile	reg (sRegFileName);
		
		//	Get the times
		UInt32				regSecs = reg.GetRegTime ();
		UInt32				useSecs = reg.GetUseTime ();
		
		//	Add 30 days
		UInt32				expireSecs = regSecs + 30L * 24L * 60L * 60L;
		
		return (expireSecs <= useSecs);	

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

	StringPtr	outSerial)
	
	{ // begin GetSerialNumber
		
		::GetIndString (outSerial, strn_Registration, kUnregisteredSNIndex);
		return true;
		
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
		
		return DemoDialog (inSuper, inEventMask).Run ();
		
	} // end DoStartupDialog

// ---------------------------------------------------------------------------
//		¥ DoPurchaseDialog
// ---------------------------------------------------------------------------

Boolean
Registration::DoPurchaseDialog (

	LCommander*	inSuper)
	
	{ // begin DoPurchaseDialog		
		
		return DemoDialog (inSuper).Run ();
		
	} // end DoPurchaseDialog

// ---------------------------------------------------------------------------
//		¥ DoUpdateDialog
// ---------------------------------------------------------------------------

Boolean
Registration::DoUpdateDialog (

	LCommander*		)
	
	{ // begin DoUpdateDialog		
		
		return false;
		
	} // end DoUpdateDialog
