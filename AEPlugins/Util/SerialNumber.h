/*
	File:		SerialNumber.h

	Contains:	Serial number generation routines.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997-2001 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>     12/9/99    rmgw    first checked in.
         <3>     3/16/99    rmgw    Add SN debugging for Kagi.
         <2>     9/22/98    rmgw    Convert to CWP3.1.
         <1>     12/8/97    rmgw    Split out serial numbers.
*/


#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef	unsigned	char	SerialChar;
typedef SerialChar*			SerialStr;
typedef	const	SerialChar*	ConstSerialStr;

SerialStr
Normalize (
	
	SerialStr			outStr,
	ConstSerialStr	inStr);

SerialStr
DeNormalize (
	
	SerialStr			outStr,
	ConstSerialStr	inStr);

SerialStr
RotateLeft (
	
	SerialStr			outStr);

SerialStr
Multiply (
	
	SerialStr			outStr,
	short				inAmount);

SerialStr
Add (
	
	SerialStr			outStr,
	ConstSerialStr		inAmount);

void
SplitSerial (
	
	SerialStr			outSeed,
	SerialStr			outCheck,
	const	char*		inSerial,
	unsigned	long	inMask);
	
char*
MergeSerial (
	
	char*				outSerial,
	ConstSerialStr		inSeed,
	ConstSerialStr		inCheck,
	unsigned	long	inMask);

char*
XorSerial (
	
	char*				ioSerial,
	unsigned	char	inMask);

//	Application specific

char*
MakeSerial (
	
	char*				outSerial,
	ConstSerialStr		inSeed);

int
TestSerial (
	
	const	char*		inSerial);

void
DebugSerial (
	
	const	char*		inTag,
	ConstSerialStr		inSerial);

#ifdef __cplusplus
}
#endif
