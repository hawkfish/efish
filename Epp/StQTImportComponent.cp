#include "StQTImportComponent.h"


/*
	File:		StQTImportComponent.cp

	Contains:	object which manages a QTImportComponent
	
	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first)

	19 feb 2001 dml	add inTryWithoutValidation option to ct, for faster loads on well behaved files
	21 aug 2000	dml	if GetGraphicsImporterForFile fails, throw error (doh!)

*/

// ---------------------------------------------------------------------------
// StQTImportComponent opens the quicktime component
// ---------------------------------------------------------------------------
StQTImportComponent::StQTImportComponent(const MFileSpec* inSpec, bool inTryWithoutValidationFirst)
	: mGI (nil)
 {
	ComponentResult		res;
	
	if (inTryWithoutValidationFirst)
		res = ::GetGraphicsImporterForFileWithFlags((const FSSpec*)inSpec, &mGI, kDontUseValidateToFindGraphicsImporter);

	if (mGI == nil)
		res = ::GetGraphicsImporterForFile((const FSSpec*)inSpec, &mGI);

	if ((mGI == nil) || (res != noErr))
		Throw_(res);
}//end fileSpec ct



// ---------------------------------------------------------------------------
// StQTImportComponent opens the quicktime component
// ---------------------------------------------------------------------------
StQTImportComponent::StQTImportComponent(Handle dataRef, OSType MIMEType) {

	mGI = OpenDefaultComponent(	GraphicsImporterComponentType,
											MIMEType);
	ThrowIfNil_(mGI);										
	ThrowIfOSErr_(::GraphicsImportSetDataHandle(mGI, dataRef));

// broken.  see dejanews.  above is workaround	
//	ThrowIfOSErr_(::GetGraphicsImporterForDataRef(	dataRef,
//													MIMEType,
//													&mGI));

}//end dataRef ct



// ---------------------------------------------------------------------------
// ~StQTImportComponent opens the quicktime component
// ---------------------------------------------------------------------------
StQTImportComponent::~StQTImportComponent() {
	::CloseComponent(mGI);
}//end dt


