/*
	File:		FixedLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.
				FixedLayout has Fixed copies of a single image.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		26 Jun 2000		drd		GetNameIndex
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "Layout.h"

class FixedLayout : public Layout
{
public:
						FixedLayout(HORef<PhotoPrintModel>& inModel);
	virtual 			~FixedLayout();

	virtual	bool		CanAddToBackground(const UInt16 inCount);
	virtual	SInt16		GetNameIndex() const				{ return 3; }
};
