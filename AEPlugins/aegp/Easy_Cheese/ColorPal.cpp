/*
	File:		ColorPal.cpp

	Contains:	Implementation of the ColorPal plugin class.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#include "ColorPal.h"

#include "ColorPalette.h"

// ---------------------------------------------------------------------------
//		¥ ColorPal
// ---------------------------------------------------------------------------

ColorPal::ColorPal (

	SPBasicSuite	*inSp, 
	AEGP_PluginID	inPluginID)
	
	: ADM::Host (inSp)
	
	, mDialog (0)

	{ // begin ColorPal
	
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
		
		if (!mDialog) mDialog = new ColorPalette ("ColorPal");
		
		if (mDialog->IsVisible()) {
			mDialog->Activate (FALSE);
			mDialog->Hide ();
			} // if
			
		else {
			mDialog->Show ();
			mDialog->Move(100, 100);
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

