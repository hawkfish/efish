/*
	File:		SchoolLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.
				SchoolLayout has multiple copies of a single image, in varying sizes

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		29 jun 2000		dml		override AdjustDocumentOrientation
		27 Jun 2000		drd		Override Initialize
		26 Jun 2000		drd		GetNameIndex
		23 Jun 2000		drd		Use HORef<PhotoPrintModel> in constructor
		19 Jun 2000		drd		Created
*/

#include "MultipleLayout.h"

class SchoolLayout : public MultipleLayout
{
public:
						SchoolLayout(HORef<PhotoPrintModel>& inModel);
	virtual 			~SchoolLayout();

	virtual	SInt16		GetNameIndex() const				{ return 5; }
	virtual	void		Initialize();
	virtual	void		AdjustDocumentOrientation();

};
