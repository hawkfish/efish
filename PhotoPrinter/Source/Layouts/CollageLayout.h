/*
	File:		CollageLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		23 Jul 2001		rmgw	Add doc and type to constructor.
		26 Jun 2000		drd		GetNameIndex
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "Layout.h"

class CollageLayout : public Layout
{
public:
						CollageLayout	(PhotoPrintDoc*				inDoc, 
								    	 HORef<PhotoPrintModel>&	inModel,
								    	 LayoutType 				inType = kCollage);
	virtual 			~CollageLayout();

	virtual	bool		CanAddToBackground(const UInt16 /*inCount*/)	{ return true; }
	virtual	SInt16		GetNameIndex() const				{ return 6; }
};
