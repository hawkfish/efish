/*
	File:		ColorPal.cpp

	Contains:	Implementation of the ColorPal plugin class.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#include "ColorPal.h"

#include "ColorPalette.h"
#include "RegistrationDialog.h"
#include "RegistrationStorage.h"

#include "SerialNumber.h"

#include <string.h>

using namespace std;

//	=== Constants ===

const	unsigned	long	kSplitMask = 0x093C5;
const	unsigned	short	kSerialLength = 16;
const	unsigned	short	kBufferLength = kSerialLength * 2;

typedef SerialChar	buffer_t [kBufferLength];

// ---------------------------------------------------------------------------
//		¥ CheckFromSeed
// ---------------------------------------------------------------------------
//	The CheckFromSeed operation produces a checksum string from seed string,
//	using the SerialNumber package.

static void
CheckFromSeed (

	SerialStr		outCheck,
	ConstSerialStr	inSeed)
	
	{ // begin CheckFromSeed
		
		buffer_t	seed;
		Normalize (seed, inSeed);
		Normalize (outCheck, inSeed);
		
		RotateLeft (outCheck);		//	outCheck <<= 1;
		Multiply (outCheck, 7);		//	outCheck *= 7;
		RotateLeft (outCheck);		//	outCheck <<= 1;
		Add (outCheck, seed);		//	outCheck += seed;
		RotateLeft (seed);			//	seed <<= 1;
		Multiply (seed, 11);		//	seed *= 11;
		Add (outCheck, seed);		//	outCheck += seed;
		RotateLeft (outCheck);		//	outCheck <<= 1;
		Multiply (outCheck, 5);		//	outCheck *= 5;
		
		DeNormalize (outCheck, outCheck);
		
	} // end CheckFromSeed
	
// ---------------------------------------------------------------------------
//		¥ MakeSerial
// ---------------------------------------------------------------------------
/*
The MakeSerial operation constructs a serial number from a seed.  The seed is a kSerialLength/2
character string, preferably identifying the customer.  CheckFromSeed is used to
construct a checksum and the seed and checksum are merged with MergeSerial using
the mask kSplitMask.  The result is a kSerialLength character string containing uppercase
characters and digits.
*/

char*
MakeSerial (
	
	char*			outSerial,
	ConstSerialStr	inSeed)
	
	{ // begin MakeSerial
		
		buffer_t	check;
		
		CheckFromSeed (check, inSeed);
		return MergeSerial (outSerial, inSeed, check, kSplitMask);
		
	} // end MakeSerial
	
// ---------------------------------------------------------------------------
//		¥ TestSerial
// ---------------------------------------------------------------------------

int
TestSerial (
	
	const	char*	inSerial)
	
	{ // begin TestSerial
		
		buffer_t	seed;
		buffer_t	check1;
		buffer_t	check2;
		
		size_t		i = 0;
		
		if (strlen (inSerial) != kSerialLength) return 0;
		
		SplitSerial (seed, check1, inSerial, kSplitMask);
		CheckFromSeed (check2, seed);
		
		for (i = 1; i <= check2[0]; ++i)
			if (check1[i] != check2[i]) return 0;
		
		return 1;
		
	} // end TestSerial

#pragma mark -

//	===	Class Variables ===

#define UNREGISTER 1

const	ASInt32					
ColorPal::sCurrentVersion = 0x010000;

const char
ColorPal::sSection [] = "Color Pal";

// ---------------------------------------------------------------------------
//		¥ ColorPal
// ---------------------------------------------------------------------------

ColorPal::ColorPal (

	SPBasicSuite	*inSp, 
	AEGP_PluginID	inPluginID)
	
	: ADM::Host (inSp)
	
	, mDialog (0)

	{ // begin ColorPal

#if UNREGISTER
		RegistrationStorage		reg (sSection, sCurrentVersion);
		
		reg.DeleteTrialVersion ();
		reg.DeleteRegTime ();
		reg.DeleteRegString ();
		reg.DeleteHost ();
#endif

	} // end ColorPal

// ---------------------------------------------------------------------------
//		¥ ~ColorPal
// ---------------------------------------------------------------------------

ColorPal::~ColorPal (void) 

	{ // begin ~ColorPal
	
		delete mDialog;
		
	} // end ~ColorPal

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ IsExpired
// ---------------------------------------------------------------------------

bool
ColorPal::IsExpired (void) 
	
	{ // begin IsExpired
		
		RegistrationStorage		reg (sSection, sCurrentVersion);
		
		//	Sanity check
		if (!reg.IsCorrectHost ()) {
			reg.DeleteTrialVersion ();
			reg.DeleteRegTime ();
			} // if
			
		//	Get the version
		ASInt32				stampVersion (reg.GetTrialVersion ());
		
		//	Update the time stamp
		time_t				nowSecs = time (0);
		if (stampVersion < sCurrentVersion)
			reg.SetRegTime (nowSecs);
		
		//	Get the time stamp
		time_t				stampSecs = reg.GetRegTime ();
		
		//	Add 7 days
		tm					expire = *gmtime (&stampSecs);
		expire.tm_mday += 7;
		time_t				expireSecs = mktime (&expire);
			
		//	Compare
		return difftime (nowSecs, expireSecs) > 0;	
	
	} // end IsExpired
	
// ---------------------------------------------------------------------------
//		¥ IsRegistered
// ---------------------------------------------------------------------------

bool
ColorPal::IsRegistered (void)
	
	{ // begin IsRegistered
		
		RegistrationStorage		reg (sSection, sCurrentVersion);
	
		//	Sanity check
		if (!reg.IsCorrectHost ()) return false;
		
		//	Get the registration key
		char					serial [kSerialLength + 1];
		if (!reg.GetRegString (sizeof (serial), serial)) return false;
	
		//	Test it
		return ::TestSerial (serial);
			
	} // end IsRegistered
	
// ---------------------------------------------------------------------------
//		¥ RegisterSerialNumber
// ---------------------------------------------------------------------------

void
ColorPal::RegisterSerialNumber (

	const	char*	inSerial)
	
	{ // begin RegisterSerialNumber
		
		RegistrationStorage		reg (sSection, sCurrentVersion);

		if (::TestSerial (inSerial)) reg.SetRegString (inSerial);

	} // end RegisterSerialNumber
	
// ---------------------------------------------------------------------------
//		¥ DoRegistrationDialog
// ---------------------------------------------------------------------------

bool
ColorPal::DoRegistrationDialog (

	ASInt32		inNotYetSecs)
	
	{ // begin DoRegistrationDialog
		
		if (IsRegistered ()) return true;
		
		RegistrationDialog		dlg (IsExpired () ? 0 : inNotYetSecs);
		switch (dlg.Run ()) {
			case RegistrationDialog::kID_OK:
				RegisterSerialNumber (dlg.GetSerialNumber ().c_str ());
				//	Fall throughÉ
				
			case RegistrationDialog::kID_NotYet:
				return true;
			} // switch
		
		return false;
		
	} // end DoRegistrationDialog

#pragma mark -

// ---------------------------------------------------------------------------
//		¥ Command
// ---------------------------------------------------------------------------

void
ColorPal::Command (void)
	
	{ // begin Command
		
		if (!mDialog) {
			if (!DoRegistrationDialog ()) return;
			
			mDialog = new ColorPalette ("ColorPal");
			} // if
			
		if (mDialog->IsVisible()) {
			mDialog->Activate (FALSE);
			mDialog->Hide ();
			} // if
			
		else {
			mDialog->Move(100, 100);
			mDialog->Show ();
			mDialog->Activate(TRUE);
			} // else
			
	} // end Command

// ---------------------------------------------------------------------------
//		¥ UpdateMenu
// ---------------------------------------------------------------------------

bool
ColorPal::UpdateMenu (void)
	
	{ // begin UpdateMenu
		
		return mDialog && mDialog->IsVisible ();
			
	} // end UpdateMenu

