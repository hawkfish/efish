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
										 InputString			inPreviewCertificate = 0,
										 InputString			inErrorURL = 0,
										 InputString			inExtraData = 0);
	virtual			~ESellerateEngine	(void);

	ArraySize		IndexSerialNumber 	(ArraySize    			arraySize,
										 SerialNumber 			serialNumberArray[],
										 ProductID    			productIDArray[] = 0,
										 NameBasedKey 			nameBasedKeyArray[] = 0);
	ArraySize		IndexSerialNumber 	(SerialNumber& 			serialNumberArray);
	
	ArraySize		IndexFileLocation 	(ArraySize    			arraySize,
										 FileLocation 			fileLocationArray[]);
	ArraySize		IndexFileLocation 	(FileLocation& 			fileLocation);
};
