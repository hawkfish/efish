/*
	File:		ESellerateEngine.cpp

	Contains:	Implementation of the eSellerate engine.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#include "ESellerateEngine.h"

// ---------------------------------------------------------------------------
//		¥ ESellerateEngine
// ---------------------------------------------------------------------------

ESellerateEngine::ESellerateEngine (

	bool				inPurchase,
	InputString			inPublisherID,
	InputString			inPurchaseRefNum,
	InputString			inPreviewCertificate,
	InputString			inErrorURL,
	InputString			inExtraData)
	
	: resultData (0)
	
	{ // begin ESellerateEngine		
		
		long 	rc;
		if (inPurchase)
			rc = ::Purchase (inPublisherID, inPurchaseRefNum, inPreviewCertificate, inErrorURL, inExtraData, &resultData);
		else rc = ::CheckForUpdate (inPublisherID, inPurchaseRefNum, inPreviewCertificate, inErrorURL, inExtraData, &resultData);
		
		if (rc) throw rc;
	
	} // end ESellerateEngine
	
// ---------------------------------------------------------------------------
//		¥ ~ESellerateEngine
// ---------------------------------------------------------------------------

ESellerateEngine::~ESellerateEngine (void)

	{ // begin ~ESellerateEngine
		
		::DeleteResultData (resultData);
		resultData = 0;
		
	} // end ~ESellerateEngine
	
// ---------------------------------------------------------------------------
//		¥ IndexSerialNumber
// ---------------------------------------------------------------------------

ArraySize
ESellerateEngine::IndexSerialNumber (

	ArraySize    arraySize,
	SerialNumber serialNumberArray[],
	ProductID    productIDArray[],
	NameBasedKey nameBasedKeyArray[])
	
	{ // begin IndexSerialNumber

		return IndexSerialNumberData (resultData, productIDArray, serialNumberArray, nameBasedKeyArray, arraySize);

	} // end IndexSerialNumber

// ---------------------------------------------------------------------------
//		¥ IndexSerialNumber
// ---------------------------------------------------------------------------

ArraySize
ESellerateEngine::IndexSerialNumber (

	SerialNumber& serialNumber)
	
	{ // begin IndexSerialNumber

		return IndexSerialNumber (1, &serialNumber);

	} // end IndexSerialNumber

// ---------------------------------------------------------------------------
//		¥ IndexFileLocation
// ---------------------------------------------------------------------------

ArraySize
ESellerateEngine::IndexFileLocation (

	ArraySize    arraySize,
	FileLocation fileLocationArray[])
	
	{ // begin IndexFileLocation

		return IndexFileLocationData (resultData, fileLocationArray, arraySize);

	} // end IndexFileLocation

// ---------------------------------------------------------------------------
//		¥ IndexFileLocation
// ---------------------------------------------------------------------------

ArraySize
ESellerateEngine::IndexFileLocation (

	FileLocation& fileLocation)
	
	{ // begin IndexFileLocation

		return IndexFileLocation (1, &fileLocation);

	} // end IndexFileLocation

