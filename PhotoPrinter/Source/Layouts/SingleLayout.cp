/*
	File:		SingleLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		19 Jun 2000		drd		Created
*/

#include "SingleLayout.h"

/*
SingleLayout
*/
SingleLayout::SingleLayout(PhotoPrintModel* inModel)
	: Layout(inModel)
{
} // SingleLayout

/*
~SingleLayout
*/
SingleLayout::~SingleLayout()
{
} // ~SingleLayout

/*
CanAddToBackground {OVERRIDE}
*/
bool
SingleLayout::CanAddToBackground(const UInt16 inCount)
{
	return (inCount == 1);
} // CanAddToBackground
