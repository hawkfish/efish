/*
	File:		RegistrationStorage.h

	Contains:	Interface to a Serial Number Registration storage class.

	Written by:	Richard Wesley

	Copyright:	Copyright ©1999-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

         <1>    11/6/01		rmgw    Created from old ERegistrationFile.
*/

#pragma once

#include "adm_persist.h"

#include <ctime>

using namespace std;

class RegistrationStorage

{

	mutable	ADM::Persist	mPersist;
	const	ASInt32			mCurrentVersion;
	
public:

		//	Class Variables
	static	const char	sHostKey[];
	static	const char	sTrialVersion[];
	static	const char	sTrialDate[];
	static	const char	sRegKey[];

						RegistrationStorage	(const	std::string&	inSection,
											 const	ASInt32&		inCurrentVersion);
	
	bool				IsCorrectHost		(void) const;
	void				DeleteHost			(void);
	
	ASInt32				GetTrialVersion		(void) const;
	void				SetTrialVersion		(ASInt32				inTrialVersion);
	void				DeleteTrialVersion	(void);
	
	time_t				GetRegTime			(void) const;
	void				SetRegTime			(time_t					inRegTime);
	void				DeleteRegTime		(void);
	
	bool				GetRegString		(A_u_long				inBufSize,
									 		 char*					outValue) const;
	void				SetRegString		(const	std::string&	inRegString);
	void				DeleteRegString		(void);

};

