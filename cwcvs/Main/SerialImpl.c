#include "SerialNumber.h"

//	=== Constants ===

const	unsigned	long	kSplitMask = 0x00A56;

// ---------------------------------------------------------------------------
//		¥ CheckFromSeed
// ---------------------------------------------------------------------------
/*
The CheckFromSeed operation produces a checksum string from seed string.  The steps
are as follows:

Normalize a copy of the the seed;
Copy it to the checksum;
RotateLeft the checkum;
Multiply the checksum by 5;
RotateLeft the checkum;
Add the normalized copy of the seed to the checksum;
RotateLeft the copy of the seed;
Add the copy of the seed to the checksum;
RotateLeft the checkum;
Multiply the checksum by 3;
DeNormalize the checksum

The checksum has now been calculated.
*/

static void
CheckFromSeed (

	StringPtr			outCheck,
	ConstStr255Param	inSeed)
	
	{ // begin CheckFromSeed
		
		Str255	seed;
		Normalize (seed, inSeed);
		Normalize (outCheck, inSeed);
		
		RotateLeft (outCheck);
		Multiply (outCheck, 5);
		RotateLeft (outCheck);
		Add (outCheck, seed);
		RotateLeft (seed);
		Add (outCheck, seed);
		RotateLeft (outCheck);
		Multiply (outCheck, 3);
		
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
		
		if (inSerial[0] != 12) return false;
		
		SplitSerial (seed, check1, inSerial, kSplitMask);
		CheckFromSeed (check2, seed);
		
		for (i = 1; i <= check2[0]; ++i)
			if (check1[i] != check2[i]) return false;
		
		return true;
		
	} // end TestSerial
	
