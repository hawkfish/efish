/*
	File:		Version.r

	Contains:	Version resources.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		 2 Nov 2001		rmgw	Handle all debug and release variants.
		29 Jun 2001		rmgw	Created.
*/

#include <Types.r>

#define versMajor			1
#define versMinor			0
#define versFix				0
#define versBuild			0

#define versKind			release
//#define versKindChar		"r"

#define versMajorBCD		((versMajor/10)*0x10 + (versMajor%10))
#define versMinorBCD		(versMinor*0x10 + versFix)

#ifdef versKindChar
	#if versFix
		#define versShortString		$$Format("%d.%d.%d" versKindChar "%d", versMajor, versMinor, versFix, versBuild)
	#else
		#define versShortString		$$Format("%d.%d" versKindChar "%d", versMajor, versMinor, versBuild)
	#endif
	
	#define dayBuild				1			
	#define dayBuildString			$$Format("Build %02d%02d%02d.%d", $$Year - 2000, $$Month, $$Day, dayBuild)
#else
	#if versFix
		#define versShortString		$$Format("%d.%d.%d", versMajor, versMinor, versFix)
	#else
		#define versShortString		$$Format("%d.%d", versMajor, versMinor)
	#endif
#endif

resource 'vers' (1) {
	versMajorBCD, versMinorBCD, versKind, versBuild, verUS,
	versShortString,
	versShortString " ©2001 Electric Fish, Inc."
#ifdef dayBuildString
	"\n" dayBuildString
#endif
};

resource 'vers' (2) {
	versMajorBCD, versMinorBCD, versKind, versBuild, verUS,
	versShortString,
	"Electric Fish PhotoPress " versShortString
};

