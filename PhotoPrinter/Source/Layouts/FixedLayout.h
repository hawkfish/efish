/*
	File:		FixedLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.
				FixedLayout has Fixed copies of one or more images (and is subclassed).

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		18 Jan 2001		drd		CommitOptionsDialog returns value and has new arg
		07 Sep 2000		drd		Override GetName
		07 sep 2000		dml		add TryToFillFirstEmpty, override AdjustDocumentOrientation
		17 Aug 2000		drd		Added MakeNewImage
		15 Aug 2000		drd		Added HasOptions; changed ancestry from Layout to GridLayout;
								override AddItem
		09 Aug 2000		drd		Moved mImageCount from SchoolLayout; changes for dialog
		26 Jun 2000		drd		GetNameIndex
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "GridLayout.h"

class FixedLayout : public GridLayout
{
public:
	enum {
		PPob_FixedOptions = 1103
	};

						FixedLayout(HORef<PhotoPrintModel>& inModel);
	virtual 			~FixedLayout();

	virtual	void		AddItem(PhotoItemRef inItem);
	virtual void		AdjustDocumentOrientation(SInt16 numPages);

	virtual bool		TryToFillFirstEmpty(PhotoItemRef inItem);
	virtual	bool		CanAddToBackground(const UInt16 inCount);
	virtual	bool		CommitOptionsDialog(EDialog& inDialog, const bool inDoLayout);
	virtual	ResIDT		GetDialogID() const							{ return PPob_FixedOptions; }
	virtual	LStr255		GetName() const;
	virtual	SInt16		GetNameIndex() const						{ return 3; }
	virtual	bool		HasOptions() const							{ return true; }
	virtual	void		Initialize();
	virtual	void		SetupOptionsDialog(EDialog& inDialog);

	virtual	PhotoPrintItem*	MakeNewImage();
	virtual	void		SetImageCount(const UInt32 inCount);

protected:
	UInt32		mImageCount;
};
