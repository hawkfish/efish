/*
	File:		EUtil.

	Contains:	utility routines

	Written by:	Dav Lion and David Dunham

	Copyright:	Copyright ©2000 by Electric Fish, Inc.  All Rights reserved.

	Change History (most recent first):

	
	21 june 2000	dml		added 'okToExpand' to BestFit
*/

class EUtil {
	public:
	
		static	void 	BestFit				(SInt32&	outWidth, 
											 SInt32&	outHeight,
											 const	SInt32		boundingWidth,
											 const	SInt32		boundingHeight,
											 const	SInt32		objectWidth,
											 const	SInt32		objectHeight,
													bool		okToExpand = true);
	};//end class EUtil