/*
	File:		CollageLayout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		23 Jul 2001		rmgw	Add doc and type to constructor.
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "CollageLayout.h"

/*
CollageLayout
*/
CollageLayout::CollageLayout(

	PhotoPrintDoc*			inDoc, 
	HORef<PhotoPrintModel>& inModel,
	LayoutType				inType)
	
	: Layout(inDoc, inModel, inType)

{
} // CollageLayout

/*
~CollageLayout
*/
CollageLayout::~CollageLayout()
{
} // ~CollageLayout
