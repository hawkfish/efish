#include "EUtil.h"

//-----------------------------------------------------
//BestFit
//-----------------------------------------------------
void 
EUtil::BestFit 		(	SInt32&	outWidth, 
						SInt32&	outHeight,
						const	SInt32		fitWidth,
						const	SInt32		fitHeight,
						const	SInt32		propWidth,
						const	SInt32		propHeight)

	{ // begin BestFit
		
		if (propWidth > 0) {
			outWidth = fitWidth ;
			outHeight = (outWidth *  propHeight ) / propWidth;
			
			if (outHeight <= fitHeight ) return;
			} // if
		
		if (propHeight > 0) {
			outHeight = fitHeight ;
			outWidth = (outHeight *  propWidth ) / propHeight ;
			} // if
			
		else outWidth = outHeight = 0;
	
	} // end BestFit
	
	

