/*
	File:		SingleLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000-2001 by Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		29 Jul 2001		drd		248 ImagesAreDuplicated
		23 Jul 2001		rmgw	Add doc and type to constructor.
		15 Sep 2000		drd		4 Override GetDialogID
		15 Aug 2000		drd		Override HasOptions
		27 Jun 2000		drd		Initialize
		26 Jun 2000		drd		Now a subclass of MultipleLayout
		26 Jun 2000		drd		GetNameIndex
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "MultipleLayout.h"

class SingleLayout : public MultipleLayout
{
public:
						SingleLayout(PhotoPrintDoc*				inDoc, 
								     HORef<PhotoPrintModel>&	inModel,
								     LayoutType 				inType = kSingle);
	virtual 			~SingleLayout();

	virtual	bool		CanAddToBackground(const UInt16 inCount);
	virtual	ResIDT		GetDialogID() const							{ return PPob_BackgroundOptions; }
	virtual	SInt16		GetNameIndex() const						{ return 2; }
	virtual	bool		HasOptions() const							{ return false; }
	virtual	void		Initialize();
	virtual	bool		ImagesAreDuplicated() const					{ return false; }
};
