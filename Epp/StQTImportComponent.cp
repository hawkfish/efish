#include "StQTImportComponent.h"


/*
	File:		StQTImportComponent.cp

	Contains:	object which manages a QTImportComponent
	
	Written by:	Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first)

		10 Jul 2001 	rmgw	Stick the file name in any error from the constructor.
		19 feb 2001 	dml		add inTryWithoutValidation option to ct, for faster loads on well behaved files
		21 aug 2000		dml		if GetGraphicsImporterForFile fails, throw error (doh!)

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

	if ((mGI == nil) || (res != noErr)) {
		try {
			Throw_(res);
			} // try
			
		catch (LException& e) {
			//	Put the file name in the error
			e.SetErrorString (inSpec->Name ());
			throw;
			} // catch
		} // if
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


