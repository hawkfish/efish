#pragma once

/*
	File:		StQTImportComponent.h

	Contains:	object which manages a QTImportComponent
	
	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first)

	19 feb 2001 dml	add inTryWithoutValidation option to ct, for faster loads on well behaved files

*/

#include "MFileSpec.h"

/*******************************
* StQTImportComponent
*	utility class to open/close an import component for a file
*	usage ensures that the QT component is closed if object destructs (even if user's ct fails)
********************************/
class StQTImportComponent {

	GraphicsImportComponent	mGI;

	public:
		StQTImportComponent(const MFileSpec* inSpec, bool inTryWithoutValidationFirst = true);
		StQTImportComponent(Handle dataRef, OSType MIMEType);
		
		virtual ~StQTImportComponent();

		operator GraphicsImportComponent (void)
			{ return mGI; };
	}; //end class StQTImportComponent
