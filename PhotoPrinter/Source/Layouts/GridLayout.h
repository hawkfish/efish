/*
	File:		GridLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.
				GridLayout keeps multiple images in a best-fit grid

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "Layout.h"

class GridLayout : public Layout
{
public:
						GridLayout(HORef<PhotoPrintModel>& inModel);
	virtual 			~GridLayout();

	virtual	bool		CanAddToBackground(const UInt16 /*inCount*/)	{ return true; }
	virtual	void		LayoutImages();
};
