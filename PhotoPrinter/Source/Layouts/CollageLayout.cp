/*
	File:		CollageLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "CollageLayout.h"

/*
CollageLayout
*/
CollageLayout::CollageLayout(HORef<PhotoPrintModel>& inModel)
	: Layout(inModel)
{
	mType  = kCollage;
} // CollageLayout

/*
~CollageLayout
*/
CollageLayout::~CollageLayout()
{
} // ~CollageLayout
