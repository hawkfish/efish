/*
	File:		SingleLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		19 Jun 2000		drd		Created
*/

#include "Layout.h"

class SingleLayout : public Layout
{
public:
						SingleLayout(PhotoPrintModel* inModel);
	virtual 			~SingleLayout();

	virtual	bool		CanAddToBackground(const UInt16 inCount);
};
