/*
	File:		TabbedPalette.cp

	Contains:	Implementation of a Tabbed Palette dialog.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#include "TabbedPalette.h"

#include "adm_suites.h"
#include "ADMDialogGroup.h"

// ---------------------------------------------------------------------------
//		¥ TabbedPalette
// ---------------------------------------------------------------------------

TabbedPalette::TabbedPalette (

	const std::string&	name,
	ASInt32 			id, 
	ASInt32 			options) 
	
	: ADM::Dialog (name, id, kADMTabbedResizingFloatingDialogStyle, options)
	
	{ // begin TabbedPalette
		
		switch (GetStyle ()) {
			case kADMTabbedFloatingDialogStyle:
			case kADMTabbedResizingFloatingDialogStyle:
				ADM::Suites::dialog_group ()->SetDialogGroupInfo (*this,
																	"GP1",  
																	327937 & kADM_DPDockVisibleMask & kADM_DPFrontTabMask);
				break;
			} // switch
			
		Hide ();

	} // end TabbedPalette
	
// ---------------------------------------------------------------------------
//		¥ ~TabbedPalette
// ---------------------------------------------------------------------------

TabbedPalette::~TabbedPalette (void) 
	
	{ // begin ~TabbedPalette
	
	} // end ~TabbedPalette
	
