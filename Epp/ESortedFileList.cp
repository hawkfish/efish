/*
	File:		ESortedFileList.cp

	Contains:	Implementation of a dialog.

	Written by:	Richard Wesley and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		26 jul 2000		dml		initial check-in
*/

#include "ESortedFileList.h"
#include <Packages.h>

bool 
SortedFilePredicate::CreatedComparator::operator () (const FullFileInfoRef& a, const FullFileInfoRef& b) const {
	return (a->second->hFileInfo.ioFlCrDat < b->second->hFileInfo.ioFlCrDat);		
	}//end operator()


bool 
SortedFilePredicate::ModifiedComparator::operator () (const FullFileInfoRef& a, const FullFileInfoRef& b) const {
	return (a->second->hFileInfo.ioFlMdDat < b->second->hFileInfo.ioFlMdDat);		
	}//end operator()

	
bool 
SortedFilePredicate::NameComparator::operator () (const FullFileInfoRef& a, const FullFileInfoRef& b) const {
	return (::CompareString(a->first->Name(), b->first->Name(), NULL) > 0);		
	}//end operator()bool 

