/*
	File:		ERegistrationFile.h

	Contains:	Interface to a Serial Number Registration file.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    11/1/01		rmgw    Created from old RegistrationSerial.cp.
*/

#pragma once

#include <Files.h>
#include <MacTypes.h>

class ERegistrationFile

{

	FSSpec				mRegSpec;
	
public:

						ERegistrationFile	(ConstStr255Param	inRegName);
						ERegistrationFile	(const	FSSpec&		inRegSpec);
	
	const	FSSpec&		GetRegSpec			(void) const {return mRegSpec;};
	void				SetRegSpec			(const	FSSpec&		inRegSpec);
	
	UInt32				GetRegTime			(void) const;
	void				SetRegTime			(UInt32				inRegTime);
	
	StringPtr			GetRegString		(StringPtr			outRegString) const;
	void				SetRegString		(ConstStr255Param	inRegString);

};

