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

bool 
SortedFilePredicate::CreatedComparator::operator () (const FullFileInfoRef& a, const FullFileInfoRef& b) const {
	if ((a->first->GetFileSpec() != nil) && (b->first->GetFileSpec() != nil))
		return (a->second->hFileInfo.ioFlCrDat < b->second->hFileInfo.ioFlCrDat);		
	else
		return false;
	}//end operator()


bool 
SortedFilePredicate::ModifiedComparator::operator () (const FullFileInfoRef& a, const FullFileInfoRef& b) const {
	if ((a->first->GetFileSpec() != nil) && (b->first->GetFileSpec() != nil))
		return (a->second->hFileInfo.ioFlMdDat < b->second->hFileInfo.ioFlMdDat);		
	else
		return false;
	}//end operator()

	
bool 
SortedFilePredicate::NameComparator::operator () (const FullFileInfoRef& a, const FullFileInfoRef& b) const {
	if ((a->first->GetFileSpec() != nil) && (b->first->GetFileSpec() != nil)) {
		return (::RelString(a->second->hFileInfo.ioNamePtr, b->second->hFileInfo.ioNamePtr, false, false) == -1);
		}//endif
	else
		return false;
	}//end operator()bool 

