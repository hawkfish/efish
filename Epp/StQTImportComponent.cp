#include "StQTImportComponent.h"


/* Change History (most recent first)

	21 aug 2000	dml	if GetGraphicsImporterForFile fails, throw error (doh!)

*/

// ---------------------------------------------------------------------------
// StQTImportComponent opens the quicktime component
// ---------------------------------------------------------------------------
StQTImportComponent::StQTImportComponent(const MFileSpec* inSpec) {
	ComponentResult		res;
	
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


