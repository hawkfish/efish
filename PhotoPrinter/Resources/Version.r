/*
	File:		Version.r

	Contains:	Version resources.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		29 Jun 2001		rmgw	Created.
*/

#include <Types.r>

#define versMajor			1
#define versMinor			0
#define versBuild			5

#define versKind			alpha
#define versKindChar		"a"

#define versMajorBCD		((versMajor/10)*0x10 + (versMajor%10))
#define versShortString		$$Format("%d.%d" versKindChar "%d", versMajor, versMinor, versBuild)

resource 'vers' (1) {
	versMajorBCD, versMinor, versKind, versBuild, verUS,
	versShortString,
	versShortString " ©2001 Electric Fish, Inc."
};

resource 'vers' (2) {
	versMajorBCD, versMinor, versKind, versBuild, verUS,
	versShortString,
	versShortString
};

