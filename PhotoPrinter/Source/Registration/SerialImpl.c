/*
	File:		SerialImpl.c

	Contains:	Implementation of the PhotoPress serial number algorithms.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <3>     10/30/01   rmgw    PhotoPress uses a different algorithm and 16 chars.
         <2>     3/16/99    rmgw    Add SN debugging for Kagi.
         <1>     12/8/99    rmgw    first checked in.
*/


#include "SerialNumber.h"

#include <MacTypes.h>

//	=== Constants ===

const	unsigned	long	kSplitMask = 0x0596A;
const	unsigned	short	kSerialLength = 16;

// ---------------------------------------------------------------------------
//		¥ CheckFromSeed
// ---------------------------------------------------------------------------
//	The CheckFromSeed operation produces a checksum string from seed string,
//	using the SerialNumber package.

static void
CheckFromSeed (

	StringPtr			outCheck,
	ConstStr255Param	inSeed)
	
	{ // begin CheckFromSeed
		
		Str255	seed;
		Normalize (seed, inSeed);
		Normalize (outCheck, inSeed);
		
		RotateLeft (outCheck);		//	outCheck <<= 1;
		Multiply (outCheck, 3);		//	outCheck *= 3;
		RotateLeft (outCheck);		//	outCheck <<= 1;
		Add (outCheck, seed);		//	outCheck += seed;
		RotateLeft (seed);			//	seed <<= 1;
		Add (outCheck, seed);		//	outCheck += seed;
		RotateLeft (outCheck);		//	outCheck <<= 1;
		Multiply (outCheck, 7);		//	outCheck *= 7;
		
		DeNormalize (outCheck, outCheck);
		
	} // end CheckFromSeed
	
// ---------------------------------------------------------------------------
//		¥ MakeSerial
// ---------------------------------------------------------------------------
/*
The MakeSerial operation constructs a serial number from a seed.  The seed is a 6
character string, preferably identifying the customer.  CheckFromSeed is used to
construct a checksum and the seed and checksum are merged with MergeSerial using
the mask 0x00A56.  The result is a twelve character string containing uppercase
characters and digits.
*/

void
MakeSerial (
	
	StringPtr			outSerial,
	ConstStr255Param	inSeed)
	
	{ // begin MakeSerial
		
		Str255	check;
		
		CheckFromSeed (check, inSeed);
		MergeSerial (outSerial, inSeed, check, kSplitMask);
		
	} // end MakeSerial
	
// ---------------------------------------------------------------------------
//		¥ TestSerial
// ---------------------------------------------------------------------------

Boolean
TestSerial (
	
	ConstStr255Param	inSerial)
	
	{ // begin TestSerial
		
		Str255		seed;
		Str255		check1;
		Str255		check2;
		
		Size		i = 0;
		
		if (inSerial[0] != kSerialLength) return 0;
		
		SplitSerial (seed, check1, inSerial, kSplitMask);
		CheckFromSeed (check2, seed);
		
		for (i = 1; i <= check2[0]; ++i)
			if (check1[i] != check2[i]) return 0;
		
		return 1;
		
	} // end TestSerial
	
