/*
	File:		SingleLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

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
						SingleLayout(HORef<PhotoPrintModel>& inModel);
	virtual 			~SingleLayout();

	virtual	bool		CanAddToBackground(const UInt16 inCount);
	virtual	SInt16		GetNameIndex() const						{ return 2; }
	virtual	bool		HasOptions() const							{ return false; }
	virtual	void		Initialize();
};
