/*
	File:		EMatrix.h

	Contains:	Useful Matrix routines

	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

	21 aug 2001		dml		created
	
*/
#ifndef MatrixTransforms_INC
#define MatrixTransforms_INC

#include "MMatrixRecord.h"
#include "ERect32.h"

class EMatrix : public MMatrixRecord {
	public:
							EMatrix	(void) : MMatrixRecord () {};
							EMatrix	(const Fixed* 			source,
									const Fixed* 			dest) : MMatrixRecord(source, dest) {};
							EMatrix	(const Rect& 			srcRect,
									 const Rect& 			dstRect) : MMatrixRecord (srcRect, dstRect) {};

							EMatrix	(const MatrixRecord&	other) : MMatrixRecord (other) {};


		void Transform(SPoint32& ioPoint);
		void Transform(ERect32& ioRect, SPoint32* p = nil);
		

	}; //end class EMatrix


#endif