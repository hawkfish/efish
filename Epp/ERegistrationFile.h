/*
	File:		ERegistrationFile.h

	Contains:	Interface to a Serial Number Registration file.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

				16 Dec 01	drd		Added "where" args to constructor
         <2>    11/14/01	rmgw    Add UseTime attribute.
         <1>    11/1/01		rmgw    Created from old RegistrationSerial.cp.
*/

#pragma once

#include <Files.h>
#include <MacTypes.h>

class ERegistrationFile

{

	FSSpec				mRegSpec;
	
public:

						ERegistrationFile	(
											ConstStr255Param	inRegName,
											const OSType		inFolderType,	// e.g. kPreferencesFolderType or kApplicationSupportFolderType
											const SInt16		inDiskOrDomain = kLocalDomain);
						ERegistrationFile	(const	FSSpec&		inRegSpec);
	
	const	FSSpec&		GetRegSpec			(void) const {return mRegSpec;};
	void				SetRegSpec			(const	FSSpec&		inRegSpec);
	
	UInt32				GetRegTime			(void) const;
	void				SetRegTime			(UInt32				inRegTime);
	
	UInt32				GetUseTime			(void) const;
	void				SetUseTime			(UInt32				inUseTime);
	
	StringPtr			GetRegString		(StringPtr			outRegString) const;
	void				SetRegString		(ConstStr255Param	inRegString);

};

