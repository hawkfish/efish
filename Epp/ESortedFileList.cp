/*
	File:		ESortedFileList.cp

	Contains:	Implementation of a dialog.

	Written by:	Richard Wesley and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		16 aug 2000		dml		clean up code 
		15 aug 2000		dml		use RelString
		15 aug 2000		dml		allow placeholders to be sorted
		03 aug 2000		dml		fix names sort behavior (using second overkill, important fix is parens)
		28 jul 2000		dml		negate names sort behavior
		26 jul 2000		dml		initial check-in
*/

#include "ESortedFileList.h"
#include <Packages.h>
#include <assert.h>

const unsigned char emptyPascalString[] = "\p";

FullFileInfo::FullFileInfo(FileSpecProvider* inProvider) 
	: mProvider (inProvider)
	, mCreated (0)
	, mModified (0)
	, mValid (false)
{
	HORef<MFileSpec>	theSpec (inProvider->GetFileSpec());
	
	if (theSpec != nil) {
		CInfoPBRec catInfo;
		theSpec->GetCatInfo(catInfo);
		std::memcpy(mName, catInfo.hFileInfo.ioNamePtr, sizeof(mName));
		mCreated = catInfo.hFileInfo.ioFlCrDat;
		mModified = catInfo.hFileInfo.ioFlMdDat;
		mValid = true;
		}//endif able to get the spec
	else
		std::memcpy(mName, emptyPascalString, sizeof(emptyPascalString));
		
};//end FullFileInfo ct




bool 
SortedFilePredicate::CreatedComparator::operator () (const FullFileInfoRef a, const FullFileInfoRef b) const {

	if ((a.Valid()) && (b.Valid())) 
		return (a.GetCreated() < b.GetCreated());		
	else
		return a.Valid();
	}//end operator()


bool 
SortedFilePredicate::ModifiedComparator::operator () (const FullFileInfoRef a, const FullFileInfoRef b) const {

	if ((a.Valid()) && (b.Valid())) 
		return (a.GetModified() < b.GetModified());		
	else
		return a.Valid();
	}//end operator()

	
bool 
SortedFilePredicate::NameComparator::operator () (const FullFileInfoRef a, const FullFileInfoRef b) const {

	if ((a.Valid()) && (b.Valid())) 
		return (::RelString(a.GetName(), b.GetName(), false, false) != -1);
	else
		return a.Valid();
	}//end operator()bool 


