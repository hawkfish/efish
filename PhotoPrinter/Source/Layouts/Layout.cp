/*
	File:		Layout.cp

	Contains:	Implementation of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		19 Jun 2000		drd		Added mRows, mColumns, mGutter
		19 Jun 2000		drd		Created
*/

#include "Layout.h"

/*
Layout
*/
Layout::Layout(PhotoPrintModel* inModel)
	: mModel(inModel)
	, mRows(1)
	, mColumns(1)
	, mGutter(kDefaultGutter)
{
	mDocument = mModel->GetDocument();
} // Layout

/*
~Layout
*/
Layout::~Layout()
{
} // ~Layout
