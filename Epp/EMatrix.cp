/*
	File:		EMatrix.cp

	Contains:	Useful Matrix routines

	Written by:	Dav Lion

	Copyright:	Copyright ©2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

	21 aug 2001		dml		created
	
*/
#include "EMatrix.h"
#include <FixMath.h>

void
EMatrix::Transform(SPoint32& ioPoint) {
	double sum;
	SInt32 vec[] = {ioPoint.h, ioPoint.v, 0};
	SInt32 tmp[3];
	
    for (int i=0; i<3; i++)
    {
        sum = 0.0;
        //        Vector<T> coli=A.col(i);
        for (int j=0; j<3; j++) {
        	double intermediate (::Fix2X(this->matrix[j][i]));
            sum = sum + intermediate * vec[j];
			}//for
			
        tmp[i] = sum; 
    }//for
    
    ioPoint.h = tmp[0];
    ioPoint.v = tmp[1];
	}//end Transform SPoint32



void
EMatrix::Transform(ERect32& ioRect, SPoint32* p)
{
#pragma unused(p)
	Transform(ioRect.TopLeft());
	Transform(ioRect.BotRight());
	}//end Transform SPoint32