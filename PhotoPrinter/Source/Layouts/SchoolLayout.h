/*
	File:		SchoolLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.
				SchoolLayout has multiple copies of a single image, in varying sizes

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		18 Jan 2001		drd		CommitOptionsDialog returns value and has new arg
		18 Sep 2000		drd		CanEditImages
		07 Sep 2000		drd		Override GetName
		14 aug 2000		dml		add landscape orientation, for those sleeping students
		09 Aug 2000		drd		Moved mImageCount to FixedLayout
		30 Jun 2000		drd		Added GetCellBounds, SetImageCount, SetupOptionsDialog
		29 Jun 2000		drd		Override LayoutImages
		29 jun 2000		dml		override AdjustDocumentOrientation
		27 Jun 2000		drd		Override Initialize
		26 Jun 2000		drd		GetNameIndex
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "MultipleLayout.h"

class SchoolLayout : public MultipleLayout
{
protected:
	OSType mReferenceOrientation;
	
public:
	enum {
		PPob_SchoolOptions = 1105
	};

						SchoolLayout(HORef<PhotoPrintModel>& inModel);
	virtual 			~SchoolLayout();

	virtual void		AddItem(PhotoItemRef inItem);
	virtual	void		AdjustDocumentOrientation(SInt16 numPages = 1);
	virtual	bool		CanEditImages() const				{ return false; }
	virtual	bool		CommitOptionsDialog(EDialog& inDialog, const bool inDoLayout);
	virtual	void		GetCellBounds(const UInt32 inIndex, MRect& outBounds);
	virtual	ResIDT		GetDialogID() const					{ return PPob_SchoolOptions; }
	virtual	LStr255		GetName() const						{ return Layout::GetName(); }	// Just the basic version
	virtual	SInt16		GetNameIndex() const				{ return 5; }
	virtual	void		Initialize();
	virtual	void		LayoutImages();
	virtual	void		SetupOptionsDialog(EDialog& inDialog);
	
	virtual	void		SetImageCount(const UInt32 inCount);
};
