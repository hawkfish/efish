/*
	File:		Layout.h

	Contains:	Definition of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		19 Jun 2000		drd		Created
*/

#pragma once

#include "PhotoPrintDoc.h"
#include "PhotoPrintModel.h"

class Layout {
public:
						Layout(PhotoPrintModel* inModel);
	virtual 			~Layout();

	virtual	bool		CanAddToBackground(const UInt16 /*inCount*/)	{ return false; }

	virtual	void		LayoutImages()									{}

protected:
	HORef<PhotoPrintDoc>		mDocument;
	HORef<PhotoPrintModel>		mModel;
};
