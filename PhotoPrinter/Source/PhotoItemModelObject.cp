/*
	File:		PhotoItemModelObject.cp

	Contains:	Implementation of the photo item model object.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved

	Change History (most recent first):

         <1>      7/2/01    rmgw    first checked in.
*/


#include "PhotoItemModelObject.h"

// ---------------------------------------------------------------------------------
//	¥ PhotoItemModelObject											[public, virtual]
// ---------------------------------------------------------------------------------

PhotoItemModelObject::PhotoItemModelObject (
	
	LModelObject*			inSuperModel,
	const	PhotoItemRef&	inItem,
	DescType				inKind)
	
	: LModelObject (inSuperModel, inKind)
	
	, mItem (inItem)
	
	{ // begin PhotoItemModelObject
		
		Assert_(mItem);
		
	} // end PhotoItemModelObject

// ---------------------------------------------------------------------------------
//	¥ ~PhotoItemModelObject											[public, virtual]
// ---------------------------------------------------------------------------------

PhotoItemModelObject::~PhotoItemModelObject (void)

	{ // begin ~PhotoItemModelObject
		
	} // end ~PhotoItemModelObject

#pragma mark -

// ---------------------------------------------------------------------------------
//	¥ GetModelName												[public, virtual]
// ---------------------------------------------------------------------------------

StringPtr
PhotoItemModelObject::GetModelName (

	Str255			outModelName) const

	{ // begin GetModelName
		
		GetPhotoItem ()->GetName (outModelName);
		
		return outModelName;
		
	} // end GetModelName

// ---------------------------------------------------------------------------
//	¥ GetAEProperty
// ---------------------------------------------------------------------------
//	Return a descriptor for the specified Property
//
//	Subclasses which have Properties must override this function

void
PhotoItemModelObject::GetAEProperty (

	DescType		inProperty,
	const AEDesc&	inRequestedType,
	AEDesc			&outPropertyDesc) const
	
	{ // begin GetAEProperty
	
		switch (inProperty) {
			case pFile:
				{
				FSSpec		spec (*(GetPhotoItem ()->GetFileSpec ()));
				ThrowIfOSErr_(::AECreateDesc(typeFSS, &spec, sizeof (spec), &outPropertyDesc));
				break;
				}
			
			case pName: 
				{
				Str255	modelName;
				GetModelName (modelName);
				ThrowIfOSErr_(::AECreateDesc(typeChar, modelName + 1, modelName[0], &outPropertyDesc));
				break;
				} // case
	
			default:
				LModelObject::GetAEProperty (inProperty, inRequestedType, outPropertyDesc);
				break;
			} // switch
			
	} // end GetAEProperty

// ---------------------------------------------------------------------------
//	¥ SetAEProperty
// ---------------------------------------------------------------------------
//	Set a Property using data from a descriptor
//
//	Subclasses which have modifiable Properties must override this function

void
PhotoItemModelObject::SetAEProperty (

	DescType		inProperty,
	const AEDesc&	inValue,
	AEDesc&			outAEReply)

	{ // begin SetAEProperty
	
		switch (inProperty) {
			default:
				LModelObject::SetAEProperty (inProperty, inValue, outAEReply);
				break;
			} // switch
			
	} // end SetAEProperty

// ---------------------------------------------------------------------------
//	¥ AEPropertyExists
// ---------------------------------------------------------------------------
//	Subclasses should override to return true for properties supported
//	in overrides of GetAEProperty and SetAEProperty

bool
PhotoItemModelObject::AEPropertyExists(
	
	DescType	inProperty) const
	
	{ // begin AEPropertyExists
	
		bool	exists = false;
	
		switch (inProperty) {
			case pName: 
			case pFile:
				exists = true;
				break;
			
			default:
				exists = LModelObject::AEPropertyExists (inProperty);
				break;
			} // switch
	
		return exists;
	
	} // end AEPropertyExists

// ---------------------------------------------------------------------------
//	¥ GetImportantAEProperties
// ---------------------------------------------------------------------------

void
PhotoItemModelObject::GetImportantAEProperties (

	AERecord &outRecord) const

	{ // begin GetImportantAEProperties
	
		StAEDescriptor		reqType;
		
			StAEDescriptor		contents;
			GetAEProperty (pFile, reqType, contents);
			UAEDesc::AddKeyDesc (&outRecord, pFile, contents);

	} // end GetImportantAEProperties

