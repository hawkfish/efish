/*
	File:		ESortedFileList.h

	Contains:	utility class to sort a list of files

	Written by:	Richard Wesley and Dav Lion

	Copyright:	Copyright ©2000-2001  by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		26 Jun 2001		drd		88 MakeSortedFileList doesn't try to get spec of placeholder
		14 Jun 2001		rmgw	Remove reference from GetFileSpec definition.  Bug #56.
		30 nov 2000		dml		fix MakeSortedFileList to work w/ Pro6 bug 18
		11 Oct 2000		drd		Break MakeSortedFileList so we can compile with CW Pro 6
		16 aug 2000		dml		changed insert to push_back (while debugging)
		15 aug 2000		dml 	allow placeholders (empty filespecs) to be sorted
		03 aug 2000		dml		rewrite with adapter class def
		26 jul 2000		dml		initial check-in
*/

#pragma once

#include "HORef.h"
#include "MFileSpec.h"
#include "EFileSpecProvider.h"
#include <cstring>


class FullFileInfo {
	private:
		FileSpecProvider*	mProvider;
		StrFileName	mName;
		long		mCreated;
		long		mModified;
		bool		mValid;
			
	public:
		FullFileInfo(FileSpecProvider* inProvider);
		virtual ~FullFileInfo(void) {};
	
		FileSpecProvider*	GetProvider(void) {return mProvider;};
		unsigned char*	GetName() {return mName;};
		long			GetCreated() {return mCreated;};
		long			GetModified() {return mModified;};
		bool			Valid() {return mValid;};
	}; //end class FullFileInfo
	

typedef FullFileInfo&	FullFileInfoRef;	
	


	
namespace SortedFilePredicate {

	struct Predicate {

		// no longer an abstract class to satisfy Pro6 compiler.  
		// this default Compare operation is a name comparison
		virtual	bool	Compare	(const	FullFileInfoRef a, const FullFileInfoRef b) const
				{
				if ((a.Valid()) && (b.Valid())) {
					return (::RelString(a.GetName(), b.GetName(), false, false) == -1);
					}//endif
				else
					return false;
					};
		
		virtual bool	operator ()	(const	FullFileInfoRef a, const FullFileInfoRef b) const
			{return Compare (a, b);}
		};


	struct	CreatedComparator : public Predicate {
		bool 	Compare	(const	FullFileInfoRef a, const FullFileInfoRef b) const 
			{return (*this)(a,b);};
		bool	operator ()	(const	FullFileInfoRef a, const FullFileInfoRef b) const;
		};//end FileDateComparator
		
	struct	ModifiedComparator : public Predicate {
		bool 	Compare	(const	FullFileInfoRef a, const FullFileInfoRef b) const 
			{return (*this)(a,b);};
		bool	operator ()	(const	FullFileInfoRef a, const FullFileInfoRef b) const;
		};//end FileDateComparator

	struct	NameComparator : public Predicate {
		bool 	Compare	(const	FullFileInfoRef a, const FullFileInfoRef b) const 
			{return (*this)(a,b);};
		bool	operator ()	(const	FullFileInfoRef a, const FullFileInfoRef b) const;
		};//end FileDateComparator


	}//end SortedFilePredicate namespace
