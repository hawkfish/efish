/*
	File:		Layout.h

	Contains:	Definition of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		27 Sep 2000		rmgw	Change ItemIsAcceptable to DragIsAcceptable.
		19 Sep 2000		drd		ResizeImage
		18 Sep 2000		drd		CanEditImages
		07 Sep 2000		drd		Added GetName
		21 Aug 2000		drd		Removed HasOptions (all layouts now have them again)
		14 aug 2000		dml		add GetDistinctImages
		13 Jul 2000		drd		Removed kDefaultGutter (now in prefs)
		13 jul 2000		dml		add orientation field
		11 jul 2000		dml		add numPages to AdjustDocumentOrientation
		05 Jul 2000		drd		CommitOptionsDialog no longer declared empty
		30 Jun 2000		drd		Added GetCellBounds, dialog methods
		28 jun 2000		dml		add mType, GetType()
		27 Jun 2000		drd		AdjustDocumentOrientation, CountOrientation
		26 Jun 2000		drd		GetNameIndex; AddItem, Initialize
		23 Jun 2000		drd		Don't use HORef for mDocument (so it's not deleted inadvertently)
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		21 Jun 2000		drd		ItemIsAcceptable
		19 Jun 2000		drd		Added mRows, mColumns, mGutter
		19 Jun 2000		drd		Created
*/

#pragma once

#include "PhotoPrintDoc.h"
#include "PhotoPrintModel.h"

class EDialog;

class Layout {
public:

	enum LayoutType {
		kUnspecified = 'none',
		kGrid = 'grid',
		kSingle = 'sing',
		kFixed = '2fix',
		kMultiple = '2dup',
		kSchool = 'mult',
		kCollage = 'coll',
		kFnordLayout = 'bad '
	};

	enum {
		PPob_BackgroundOptions = 1100,
		str_LayoutNames = 500
	};

						Layout(HORef<PhotoPrintModel>& inModel);
	virtual 			~Layout();

	// Accessors
			SInt16		GetColumns() const							{ return mColumns; }
			SInt16		GetGutter() const							{ return mGutter; }
	virtual	LStr255		GetName() const			{ return LStr255(str_LayoutNames, this->GetNameIndex()); }
	virtual	SInt16		GetNameIndex() const = 0;
			SInt16		GetRows() const								{ return mRows; }
			OSType		GetType() const								{ return mType; }
			OSType		GetOrientation() const						{ return mOrientation;}
			
	virtual	void		AddItem(PhotoItemRef inItem);
	virtual	void		AdjustDocumentOrientation(SInt16 numPages = 1);
	virtual	bool		CanAddToBackground(const UInt16 /*inCnt*/)	{ return false; }
	virtual	bool		CanEditImages() const						{ return true; }
			UInt32		CountOrientation(const OSType inType) const;
	virtual SInt16		GetDistinctImages(void);
	virtual	Boolean		DragIsAcceptable(DragReference inDragRef);

	virtual	void		GetCellBounds(const UInt32 /*inI*/, MRect& /*outB*/)	{}
	virtual	void		Initialize()	{} // = 0 !!!
	virtual	void		LayoutImages()								{ this->AdjustDocumentOrientation(); }
	virtual	bool		ResizeImage(const OSType /*inCode*/,
									const FitT /*inFit*/,
									PhotoItemRef /*ioItemRef*/)		{ return false; }	// = 0; !!!

	virtual	void		CommitOptionsDialog(EDialog& inDlog);
	virtual	ResIDT		GetDialogID() const							{ return PPob_BackgroundOptions; }
	virtual	void		SetupOptionsDialog(EDialog& inDialog);

protected:
	OSType						mType;
	PhotoPrintDoc*				mDocument;
	HORef<PhotoPrintModel>		mModel;

	SInt16						mRows;
	SInt16						mColumns;
	SInt16						mGutter;		// Minimum separation, in pixels
	
	OSType						mOrientation;
};
