/*
	File:		Version.r

	Contains:	Version resources.

	Written by:	Richard Wesley

	Copyright:	Copyright ©2001 Electric Fish, Inc.  All Rights Reserved.

	Change History (most recent first):

		14 Nov 2001		rmgw	Version 1.0.1.
		 2 Nov 2001		rmgw	Handle all debug and release variants.
		29 Jun 2001		rmgw	Created.
*/

#include <Types.r>

#define kReleaseBuild		1

#define versMajor			1
#define versMinor			0
#define versFix				1
#define versBuild			0

#define versMajorBCD		((versMajor/10)*0x10 + (versMajor%10))
#define versMinorBCD		(versMinor*0x10 + versFix)

#if kReleaseBuild
	#define versKind			release
	
	#if versFix
		#define versShortString		$$Format("%d.%d.%d", versMajor, versMinor, versFix)
	#else
		#define versShortString		$$Format("%d.%d", versMajor, versMinor)
	#endif
#else
	#define versKind			development
	#define versKindChar		"d"

	#if versFix
		#define versShortString		$$Format("%d.%d.%d" versKindChar "%d", versMajor, versMinor, versFix, versBuild)
	#else
		#define versShortString		$$Format("%d.%d" versKindChar "%d", versMajor, versMinor, versBuild)
	#endif
#endif

#define dayBuild				1			
#define dayBuildString			$$Format("Build %02d%02d%02d.%d", $$Year - 2000, $$Month, $$Day, dayBuild)

resource 'vers' (1, purgeable) {
	versMajorBCD, versMinorBCD, versKind, versBuild, verUS,
	versShortString,
	versShortString " ©2001 Electric Fish, Inc."
#if !kReleaseBuild
	"\n" dayBuildString
#endif
};

resource 'vers' (2, purgeable) {
	versMajorBCD, versMinorBCD, versKind, versBuild, verUS,
	versShortString,
	"Electric Fish PhotoPress " versShortString
};

