/*
	File:		ESellerateEngine.h

	Contains:	Interface to the eSellerate engine.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

*/

#pragma once

#include "esellerate.h"
#include "validate.h"

class ESellerateEngine
{

	ResultData   	resultData;

public:
	
	enum {
		kPurchase = true,
		kUpdate = false
		};
		
					ESellerateEngine	(bool					inPurchase,
										 InputString			inPublisherID,
										 InputString			inPurchaseRefNum,
										 InputString			inPreviewCertificate = nil,
										 InputString			inErrorURL = nil,
										 InputString			inExtraData = nil);
	virtual			~ESellerateEngine	(void);

	ArraySize		IndexSerialNumber 	(ArraySize    			arraySize,
										 SerialNumber 			serialNumberArray[],
										 ProductID    			productIDArray[] = nil,
										 NameBasedKey 			nameBasedKeyArray[] = nil);
	ArraySize		IndexSerialNumber 	(SerialNumber& 			serialNumberArray);
	
	ArraySize		IndexFileLocation 	(ArraySize    			arraySize,
										 FileLocation 			fileLocationArray[]);
	ArraySize		IndexFileLocation 	(FileLocation& 			fileLocation);
};
