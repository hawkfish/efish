/*
	File:		ESortedFileList.h

	Contains:	utility class to sort a list of files

	Written by:	Richard Wesley and Dav Lion

	Copyright:	Copyright �2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		26 jul 2000		dml		initial check-in
*/

#pragma once

#include "HORef.h"
#include "MFileSpec.h"
#include <vector>

typedef HORef<MFileSpec>				FileRef;
typedef std::vector<FileRef>			FileRefVector;
typedef HORef<CInfoPBRec>				CInfoRef;
typedef std::pair<FileRef, CInfoRef>	FullFileInfo;
typedef	HORef<FullFileInfo>				FullFileInfoRef;
typedef	std::vector<FullFileInfoRef>	FullFileList;

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
			(*i)->GetCatInfo (*info);
			
			outList.insert (outList.end (), new FullFileInfo (*i, info));
			} // for
			
		std::sort (outList.begin (), outList.end (), inCompare);

	}; // end 
	

namespace SortedFilePredicate {

	struct Predicate {

		virtual	bool	Compare	(const	FullFileInfoRef& a, const FullFileInfoRef& b) const = 0;
		
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