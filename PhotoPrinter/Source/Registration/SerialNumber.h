/*
	File:		SerialNumber.h

	Contains:	Serial number generation routines.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1997 Electric Fish Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>     12/9/99    rmgw    first checked in.
         <3>     3/16/99    rmgw    Add SN debugging for Kagi.
         <2>     9/22/98    rmgw    Convert to CWP3.1.
         <1>     12/8/97    rmgw    Split out serial numbers.
*/


#pragma once

#include <MacTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

StringPtr
Normalize (
	
	StringPtr			outStr,
	ConstStr255Param	inStr);

StringPtr
DeNormalize (
	
	StringPtr			outStr,
	ConstStr255Param	inStr);

StringPtr
RotateLeft (
	
	StringPtr			outStr);

StringPtr
Multiply (
	
	StringPtr			outStr,
	short				inAmount);

StringPtr
Add (
	
	StringPtr			outStr,
	ConstStr255Param	inAmount);

void
SplitSerial (
	
	StringPtr			outSeed,
	StringPtr			outCheck,
	ConstStr255Param	inSerial,
	unsigned	long	inMask);
	
StringPtr
MergeSerial (
	
	StringPtr			outSerial,
	ConstStr255Param	inSeed,
	ConstStr255Param	inCheck,
	unsigned	long	inMask);

StringPtr
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

void
DebugSerial (
	
	const	char*		inTag,
	ConstStr255Param	inSerial);

#ifdef __cplusplus
}
#endif
