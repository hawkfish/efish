/*
	File:		StQTImportComponent.cp

	Contains:	object which manages a QTImportComponent
	
	Written by:	Dav Lion

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first)

		21 Aug 2001 	rmgw	Add inline members.
		10 Jul 2001 	rmgw	Stick the file name in any error from the constructor.
		19 feb 2001 	dml		add inTryWithoutValidation option to ct, for faster loads on well behaved files
		21 aug 2000		dml		if GetGraphicsImporterForFile fails, throw error (doh!)

*/

#include "StQTImportComponent.h"

// ---------------------------------------------------------------------------
//	¥ CheckComponentResult
// ---------------------------------------------------------------------------

void
StQTImportComponent::CheckComponentResult (

	ComponentResult		r)

	{ // begin CheckComponentResult
		
		ThrowIfOSErr_(r);
		
	} // end CheckComponentResult

// ---------------------------------------------------------------------------
//	¥ StQTImportComponent opens the quicktime component
// ---------------------------------------------------------------------------

StQTImportComponent::StQTImportComponent(

	const FSSpec&	inSpec, 
	bool 			inTryWithoutValidationFirst)
	
	: mGI (nil)
 
	 { // begin StQTImportComponent

		ComponentResult		res;
		
		if (inTryWithoutValidationFirst)
			res = ::GetGraphicsImporterForFileWithFlags(&inSpec, &mGI, kDontUseValidateToFindGraphicsImporter);

		if (mGI == nil)
			res = ::GetGraphicsImporterForFile(&inSpec, &mGI);

		if ((mGI == nil) || (res != noErr)) {
			try {
				Throw_(res);
				} // try
				
			catch (LException& e) {
				//	Put the file name in the error
				e.SetErrorString (inSpec.name);
				throw;
				} // catch
			} // if
	
	} // end StQTImportComponent (fileSpec)

// ---------------------------------------------------------------------------
//	¥ StQTImportComponent opens the quicktime component
// ---------------------------------------------------------------------------

StQTImportComponent::StQTImportComponent (

	Handle 	dataRef, 
	OSType 	MIMEType) 
	
	: mGI (::OpenDefaultComponent (GraphicsImporterComponentType, MIMEType))
		
	{ // begin StQTImportComponent

		ThrowIfNil_(mGI);										
		
		SetDataHandle (dataRef);

		// broken.  see dejanews.  above is workaround	
		//	ThrowIfOSErr_(::GetGraphicsImporterForDataRef(	dataRef,
		//													MIMEType,
		//													&mGI));

	} // end StQTImportComponent (dataRef)

// ---------------------------------------------------------------------------
//	¥ StQTImportComponent adopts the quicktime component
// ---------------------------------------------------------------------------

StQTImportComponent::StQTImportComponent (

	GraphicsImportComponent	gi)

	: mGI (gi)
	
	{ // begin StQTImportComponent
		
	} // end StQTImportComponent

// ---------------------------------------------------------------------------
// 	¥ ~StQTImportComponent opens the quicktime component
// ---------------------------------------------------------------------------

StQTImportComponent::~StQTImportComponent (void) 

	{ // begin ~StQTImportComponent
	
		if (mGI) ::CloseComponent (mGI);
		mGI = nil;
		
	} // end ~StQTImportComponent


