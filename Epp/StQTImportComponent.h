#pragma once

#include "MFileSpec.h"

/*******************************
* StQTImportComponent
*	utility class to open/close an import component for a file
*	usage ensures that the QT component is closed if object destructs (even if user's ct fails)
********************************/
class StQTImportComponent {

	GraphicsImportComponent	mGI;

	public:
		StQTImportComponent(const MFileSpec* inSpec);
		StQTImportComponent(Handle dataRef, OSType MIMEType);
		
		virtual ~StQTImportComponent();

		operator GraphicsImportComponent (void)
			{ return mGI; };
	}; //end class StQTImportComponent
