/*
	File:		Layout.h

	Contains:	Definition of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		21 Jun 2000		drd		ItemIsAcceptable
		19 Jun 2000		drd		Added mRows, mColumns, mGutter
		19 Jun 2000		drd		Created
*/

#pragma once

#include "PhotoPrintDoc.h"
#include "PhotoPrintModel.h"

class Layout {
public:
	enum {
		kDefaultGutter = 72 / 8					// 1/8 inch
	};

						Layout(HORef<PhotoPrintModel>& inModel);
	virtual 			~Layout();

	// Accessors
			SInt16		GetColumns() const								{ return mColumns; }
			SInt16		GetGutter() const								{ return mGutter; }
			SInt16		GetRows() const									{ return mRows; }

	virtual	bool		CanAddToBackground(const UInt16 /*inCount*/)	{ return false; }
	virtual	bool		ItemIsAcceptable(DragReference inDragRef, ItemReference inItemRef, FlavorType& outFlavor);

	virtual	void		LayoutImages()									{}

protected:
	HORef<PhotoPrintDoc>		mDocument;
	HORef<PhotoPrintModel>		mModel;

	SInt16						mRows;
	SInt16						mColumns;
	SInt16						mGutter;		// Minimum separation, in pixels
};
