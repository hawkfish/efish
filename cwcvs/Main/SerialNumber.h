/*
	File:		SerialNumber.h

	Contains:	Serial number generation routines.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <2>     9/22/98    rmgw    Convert to CWP3.1.
         <1>     12/8/97    rmgw    Split out serial numbers.
*/


#pragma once

#include <MacTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void
Normalize (
	
	StringPtr			outStr,
	ConstStr255Param	inStr);

void
DeNormalize (
	
	StringPtr			outStr,
	ConstStr255Param	inStr);

void
RotateLeft (
	
	StringPtr			outStr);

void
Multiply (
	
	StringPtr			outStr,
	short				inAmount);

void
Add (
	
	StringPtr			outStr,
	ConstStr255Param	inAmount);

void
SplitSerial (
	
	StringPtr			outSeed,
	StringPtr			outCheck,
	ConstStr255Param	inSerial,
	unsigned	long	inMask);
	
void
MergeSerial (
	
	StringPtr			outSerial,
	ConstStr255Param	inSeed,
	ConstStr255Param	inCheck,
	unsigned	long	inMask);

void
XorSerial (
	
	StringPtr			ioSerial,
	unsigned	char	inMask);

//	Application specific

void
MakeSerial (
	
	StringPtr			outSerial,
	ConstStr255Param	inSeed);

Boolean
TestSerial (
	
	ConstStr255Param	inSerial);

#ifdef __cplusplus
}
#endif
