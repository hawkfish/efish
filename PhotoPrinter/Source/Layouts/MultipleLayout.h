/*
	File:		MultipleLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.
				MultipleLayout has multiple copies of a single image.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		19 Jun 2000		drd		Created
*/

#include "FixedLayout.h"

class MultipleLayout : public FixedLayout
{
public:
						MultipleLayout(PhotoPrintModel* inModel);
	virtual 			~MultipleLayout();

	virtual	bool		CanAddToBackground(const UInt16 inCount);
};
