/*
	File:		SerialNumber.c

	Contains:	Serial number generation algorithms.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

				05 Oct 00	drd		Include size_t.h
         <1>     12/8/99    rmgw    first checked in.
         <3>     3/16/99    rmgw    Add SN debugging for Kagi.
         <2>    12/12/97    rmgw    Fix operations that were ignoring the first position.
*/


#include "SerialNumber.h"

#include <ctype.h>

//	=== Constants ===

const	short				kRadix = 36;

// ---------------------------------------------------------------------------
//		¥ Normalize
// ---------------------------------------------------------------------------
/*
The Normalize operation converts any string to a base 36 representation.  The digits
'0'-'9' are mapped to the values 0-9 respectively; alphabetic characters 'A'-'Z' and
'a'-'z' are mapped to the values 10-35 respectively; all other characters are taken
modulo 36 (e.g. ASCII SP maps to 32, '@' maps to 28).
*/

SerialStr
Normalize (
	
	SerialStr			outStr,
	ConstSerialStr	inStr)
	
	{ // begin Normalize
		
		size_t		i = 0;
		outStr[i] = inStr[i];
		for (++i; i <= inStr[0]; ++i) {
			if (isdigit (inStr[i])) 
				outStr[i] = inStr[i] - '0';
			
			else if (isalpha (inStr[i]))
				outStr[i] = toupper (inStr[i]) - 'A' + 10;
			
			else outStr[i] = inStr[i] % kRadix;
			} // for
		
		return outStr;
		
	} // end Normalize
	
// ---------------------------------------------------------------------------
//		¥ DeNormalize
// ---------------------------------------------------------------------------
/*
The Denormalize operation converts a Normalized string back into a printable
string.  The values 0-9 are mapped to the digits '0'-'9' respectively; the values
10-35 are mapped to the uppercase letters 'A'-'Z' respectively.  All values are
taken modulo 36 to avoid undefined cases.
*/

SerialStr
DeNormalize (
	
	SerialStr			outStr,
	ConstSerialStr	inStr)
	
	{ // begin DeNormalize
		
		size_t	i = 0;
		outStr[i] = inStr[i];
		for (++i; i <= inStr[0]; ++i) {
			unsigned	char	c = inStr[i];
			c %= kRadix;
			if (c < 10)
				c += '0';
			else c += 'A' - 10;
			outStr[i] = c;
			} // for
		
		return outStr;
		
	} // end DeNormalize
	
// ---------------------------------------------------------------------------
//		¥ RotateLeft
// ---------------------------------------------------------------------------
/*
The RotateLeft operation moves each charater in a string one position to the left.
The first character is moved to the old end of the string.
*/

SerialStr
RotateLeft (
	
	SerialStr	outStr)
	
	{ // begin RotateLeft
		
		size_t				i = 1;
		unsigned	char	c = outStr[i];
		
		for (; i < outStr[0]; ++i) 
			outStr[i] = outStr[i + 1];
		outStr[i] = c;
		
		return outStr;

	} // end RotateLeft

// ---------------------------------------------------------------------------
//		¥ Multiply
// ---------------------------------------------------------------------------
/*
The Multiply operation multiplies each character by a fixed value and reduces it
modulo 36.  No carry is performed.  The results are all calculated by unsigned 
arithmetic, so no negative intermediate values should result.
*/

SerialStr
Multiply (
	
	SerialStr	outStr,
	short		inAmount)
	
	{ // begin Multiply
		
		size_t				i = 0;
		
		for (++i; i <= outStr[0]; ++i) {
			size_t	c = outStr[i];
			c *= inAmount;
			c %= kRadix;
			outStr[i] = c;
			} // for
			
		return outStr;

	} // end Multiply

// ---------------------------------------------------------------------------
//		¥ Add
// ---------------------------------------------------------------------------
/*
The Add operation adds one string to another.  All operations are performed on the first string
which will contain the final result. Each pair of characters is added one at a time from
left to right.  The result of this addition divided by 36 is added to the next position of the first
string and the result modulo 36 is retained in the current position of the first string.
The two strings are assumed to be the same length and carries off the end are ignored.
*/

SerialStr
Add (
	
	SerialStr			outStr,
	ConstSerialStr	inAmount)
	
	{ // begin Add
		
		size_t				i = 0;
		
		for (++i; i <= outStr[0]; ++i) {
			size_t	c = outStr[i];
			c += inAmount[i];
			outStr[i + 1] += (c / kRadix);
			c %= kRadix;
			outStr[i] = c;
			} // for
			
		return outStr;

	} // end Add

// ---------------------------------------------------------------------------
//		¥ SplitSerial
// ---------------------------------------------------------------------------

void
SplitSerial (
	
	SerialStr			outSeed,
	SerialStr			outCheck,
	const	char*		inSerial,
	unsigned	long	inMask)
	
	{ // begin SplitSerial
		
		size_t				i;

		outSeed[0] = outCheck[0] = 0;
		for (i = 0; inSerial[i]; ++i) {
			if (inMask & (0x01 << i))
				outSeed [++outSeed[0]] = inSerial[i];
			else outCheck[++outCheck[0]] = inSerial[i];
			} // if
			
	} // end SplitSerial
	
// ---------------------------------------------------------------------------
//		¥ MergeSerial
// ---------------------------------------------------------------------------
/*
The MergeSerial operation takes a seed, its calculated checksum and a mask describing
how to merge the characters.  It then builds a new string from the other two
by advancing through them from left to right under control of the bits of a mask that is read from
right to left.  If a bit is set in the mask, it indicates that the next seed character should be
copied; otherwise a checksum character should be copied.
*/

char*
MergeSerial (
	
	char*				outSerial,
	ConstSerialStr		inSeed,
	ConstSerialStr		inCheck,
	unsigned	long	inMask)
	
	{ // begin MergeSerial
		
		size_t				i = 0;
		size_t				count = inSeed[0] + inCheck[0];
		inSeed++; inCheck++;
		for (i = 0; i < count; ++i) {
			if (inMask & (0x01 << i))
				outSerial[i] = *inSeed++;
			else outSerial[i] = *inCheck++;
			} // if
		
		outSerial[i] = 0;
			
		return outSerial;

	} // end MergeSerial

// ---------------------------------------------------------------------------
//		¥ XorSerial
// ---------------------------------------------------------------------------

char*
XorSerial (
	
	char*				ioSerial,
	unsigned	char	inMask)
	
	{ // begin XorSerial
		
		char*		s = ioSerial;

		for (s = ioSerial; *s; ++s)
			*s ^= inMask;
			
		return ioSerial;

	} // end XorSerial
	
