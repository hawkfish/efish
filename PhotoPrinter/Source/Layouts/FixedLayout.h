/*
	File:		FixedLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.
				FixedLayout has Fixed copies of a single image.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		09 Aug 2000		drd		Moved mImageCount from SchoolLayout; changes for dialog
		26 Jun 2000		drd		GetNameIndex
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "Layout.h"

class FixedLayout : public Layout
{
public:
	enum {
		PPob_FixedOptions = 1103
	};

						FixedLayout(HORef<PhotoPrintModel>& inModel);
	virtual 			~FixedLayout();

	virtual	bool		CanAddToBackground(const UInt16 inCount);
	virtual	void		CommitOptionsDialog(EDialog& inDialog);
	virtual	ResIDT		GetDialogID() const					{ return PPob_FixedOptions; }
	virtual	SInt16		GetNameIndex() const				{ return 3; }
	virtual	void		SetupOptionsDialog(EDialog& inDialog);

	virtual	void		SetImageCount(const UInt32 inCount);

protected:
	UInt32		mImageCount;
};
