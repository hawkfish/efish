/*
	File:		GridLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				GridLayout keeps multiple images in a best-fit grid

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		19 Jun 2000		drd		Created
*/

#include "GridLayout.h"

/*
GridLayout
*/
GridLayout::GridLayout(PhotoPrintModel* inModel)
	: Layout(inModel)
{
} // GridLayout

/*
~GridLayout
*/
GridLayout::~GridLayout()
{
} // ~GridLayout

/*
LayoutImages {OVERRIDE}
*/
void
GridLayout::LayoutImages()
{
} // LayoutImages