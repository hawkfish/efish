#include "StQTImportComponent.h"


// ---------------------------------------------------------------------------
// StQTImportComponent opens the quicktime component
// ---------------------------------------------------------------------------
StQTImportComponent::StQTImportComponent(const MFileSpec* inSpec) {
	ComponentResult		res;
	
	res = ::GetGraphicsImporterForFile((const FSSpec*)inSpec, &mGI);
	ThrowIfNil_(mGI);
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


