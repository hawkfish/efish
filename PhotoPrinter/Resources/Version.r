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
#define versFix				0
#define versBuild			4

#define versKind			release
#define versKindChar		"r"

#define versMajorBCD		((versMajor/10)*0x10 + (versMajor%10))
#define versMinorBCD		(versMinor*0x10 + versFix)

#if versFix
	#define versShortString		$$Format("%d.%d.%d", versMajor, versMinor, versFix)
#else
	#define versShortString		$$Format("%d.%d", versMajor, versMinor)
#endif

//#define versShortString		$$Format("%d.%d" versKindChar "%d", versMajor, versMinor, versBuild)

resource 'vers' (1) {
	versMajorBCD, versMinorBCD, versKind, versBuild, verUS,
	versShortString,
	versShortString " ©2001 Electric Fish, Inc."
};

resource 'vers' (2) {
	versMajorBCD, versMinorBCD, versKind, versBuild, verUS,
	versShortString,
	"Electric Fish PhotoPress " versShortString
};

