/*
	File:		ESortedFileList.h

	Contains:	utility class to sort a list of files

	Written by:	Richard Wesley and Dav Lion

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
#include <vector>

// implement this class so that you can use the sorter
class FileSpecProvider {
	public:
		virtual HORef<MFileSpec>&	GetFileSpec(void) = 0;
};//end class FileSpecProvider


typedef FileSpecProvider*					FileProvider;
typedef std::vector<FileProvider>			FileProviderVector;
typedef HORef<CInfoPBRec>					CInfoRef;
typedef std::pair<FileProvider, CInfoRef>	FullFileInfo;
typedef	HORef<FullFileInfo>					FullFileInfoRef;
typedef	std::vector<FullFileInfoRef>		FullFileList;

#define DEBUGGING_SORT 1
template<class InputIterator, class Comparator>
void
MakeSortedFileList (

	FullFileList&	outList,
	InputIterator	inBegin,
	InputIterator	inEnd,
	Comparator&		inCompare)
	
	{ // begin
		
		for (InputIterator i = inBegin; i != inEnd; ++i) {
			CInfoRef	info = new CInfoPBRec;
			HORef<MFileSpec>& pSpec = (*i)->GetFileSpec();
			if (pSpec)
				pSpec->GetCatInfo (*info);
			
			outList.push_back (new FullFileInfo (*i, info));
			} // for

		// !!! the following line won't compile with the new CodeWarrior !!!
		std::sort (outList.begin (), outList.end (), inCompare);

	}; // end 
	

namespace SortedFilePredicate {

	struct Predicate {

		// no longer an abstract class to satisfy Pro6 compiler.  
		// this default Compare operation is a name comparison
		virtual	bool	Compare	(const	FullFileInfoRef& a, const FullFileInfoRef& b) const
			{	if ((a->first->GetFileSpec() != nil) && (b->first->GetFileSpec() != nil)) {
					return (::RelString(a->second->hFileInfo.ioNamePtr, b->second->hFileInfo.ioNamePtr, false, false) == -1);
					}//endif
				else
					return false;
					};
		
		bool	operator ()	(const	FullFileInfoRef& a, const FullFileInfoRef& b) const
			{return Compare (a, b);}
		};


	struct	CreatedComparator : public Predicate {
		bool 	Compare	(const	FullFileInfoRef& a, const FullFileInfoRef& b) const 
			{return (*this)(a,b);};
		bool	operator ()	(const	FullFileInfoRef& a, const FullFileInfoRef& b) const;
		};//end FileDateComparator
		
	struct	ModifiedComparator : public Predicate {
		bool 	Compare	(const	FullFileInfoRef& a, const FullFileInfoRef& b) const 
			{return (*this)(a,b);};
		bool	operator ()	(const	FullFileInfoRef& a, const FullFileInfoRef& b) const;
		};//end FileDateComparator

	struct	NameComparator : public Predicate {
		bool 	Compare	(const	FullFileInfoRef& a, const FullFileInfoRef& b) const 
			{return (*this)(a,b);};
		bool	operator ()	(const	FullFileInfoRef& a, const FullFileInfoRef& b) const;
		};//end FileDateComparator


	struct Not : public Predicate {
		Not(Predicate* inPredicate) : mOperand (inPredicate) {};
	
		Predicate*		mOperand;
		virtual	bool	Compare	(const	FullFileInfoRef& a, const FullFileInfoRef& b) const
			{return !(*mOperand) (a, b);}
		};

	struct And : public Predicate {
		Predicate*		mOperand1;
		Predicate*		mOperand2;
		virtual	bool	Compare	(const	FullFileInfoRef& a, const FullFileInfoRef& b) const
			{return (*mOperand1) (a, b) && (*mOperand2) (a, b);}
		};

	struct Or : public Predicate {
		Predicate*		mOperand1;
		Predicate*		mOperand2;
		virtual	bool	Compare	(const	FullFileInfoRef& a, const FullFileInfoRef& b) const
			{return (*mOperand1) (a, b) || (*mOperand2) (a, b);}
		};

	}//end SortedFilePredicate namespace
