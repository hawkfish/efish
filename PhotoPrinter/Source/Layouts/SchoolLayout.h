/*
	File:		SchoolLayout.h

	Contains:	Definition of Layout object, which manages positioning of images.
				SchoolLayout has multiple copies of a single image, in varying sizes

	Written by:	David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

		19 Jun 2000		drd		Created
*/

#include "MultipleLayout.h"

class SchoolLayout : public MultipleLayout
{
public:
						SchoolLayout(PhotoPrintModel* inModel);
	virtual 			~SchoolLayout();
};
