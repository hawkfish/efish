/*
	File:		ColorPal.cpp

	Contains:	Implementation of the ColorPal plugin class.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#include "ColorPal.h"

#include "ColorPalette.h"
#include "Registration.h"

//	===	Class Variables ===

const	ASInt32					
Registration::sCurrentVersion = 0x010000;

const char
Registration::sSection [] = "Color Pal";

// ---------------------------------------------------------------------------
//		¥ ColorPal
// ---------------------------------------------------------------------------

ColorPal::ColorPal (

	SPBasicSuite	*inSp, 
	AEGP_PluginID	inPluginID,
	const A_char*	inResPath)
	
	: ADM::Host (inSp)
	
	, mDialog (0)
	
	{ // begin ColorPal
		
		Registration::Initialize (inResPath);
		
	} // end ColorPal

// ---------------------------------------------------------------------------
//		¥ ~ColorPal
// ---------------------------------------------------------------------------

ColorPal::~ColorPal (void) 

	{ // begin ~ColorPal
	
		delete mDialog;
		
	} // end ~ColorPal

// ---------------------------------------------------------------------------
//		¥ Command
// ---------------------------------------------------------------------------

void
ColorPal::Command (void)
	
	{ // begin Command
		
		if (!mDialog) {
			if (!Registration::DoRegistrationDialog ()) return;
			
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

