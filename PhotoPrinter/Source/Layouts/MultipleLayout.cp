/*
	File:		MultipleLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.
				MultipleLayout has multiple copies of a single image.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		19 Jun 2000		drd		Created
*/

#include "MultipleLayout.h"

/*
MultipleLayout
*/
MultipleLayout::MultipleLayout(PhotoPrintModel* inModel)
	: FixedLayout(inModel)
{
} // MultipleLayout

/*
~MultipleLayout
*/
MultipleLayout::~MultipleLayout()
{
} // ~MultipleLayout

/*
CanAddToBackground {OVERRIDE}
*/
bool
MultipleLayout::CanAddToBackground(const UInt16 inCount)
{
	return (inCount == 1);
} // CanAddToBackground
