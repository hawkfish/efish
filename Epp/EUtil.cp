#include "EUtil.h"
#include <algorithm.h>
//-----------------------------------------------------
//BestFit
//-----------------------------------------------------
void 
EUtil::BestFit 		(	SInt32&	outWidth, 
						SInt32&	outHeight,
						const	SInt32		boundingWidth,
						const	SInt32		boundingHeight,
						const	SInt32		objectWidth,
						const	SInt32		objectHeight,
								bool 		okToExpand)

	{ // begin BestFit
		
	do {	
		if (objectWidth > 0) {
			outWidth = okToExpand ? max(boundingWidth, objectWidth) : min(objectWidth, boundingWidth);
			outHeight = (outWidth *  objectHeight ) / objectWidth;
			
			if (outHeight <= boundingHeight ) 
				break;
			} // if
		
		if (objectHeight > 0) {
			outHeight = okToExpand ? max(boundingHeight, objectHeight) : min(boundingHeight, objectHeight) ;
			outWidth = (outHeight *  objectWidth ) / objectHeight ;
			break;
			} // if
			
		outWidth = outHeight = 0;
		} while (false);
		
		
	} // end BestFit
	
	

