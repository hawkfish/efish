#include <Types.r>

#define kReleaseBuild		0

#define versMajor			1
#define versMinor			0
#define versRevision		0
#define versBuild			3

#define versMajorBCD		((versMajor/10)*0x10 + (versMajor%10))
#define versMinorBCD		(versMinor*0x10 + versRevision)

#if kReleaseBuild
	#define versKind			final

	#if versRevision
		#define versShortString		$$Format("%d.%d.%d", versMajor, versMinor, versRevision)
	#else
		#define versShortString		$$Format("%d.%d", versMajor, versMinor)
	#endif
#else
	#define versKind			development
	#define versKindChar		"d"

	#if versRevision
		#define versShortString		$$Format("%d.%d.%d" versKindChar "%d", versMajor, versMinor, versRevision, versBuild)
	#else
		#define versShortString		$$Format("%d.%d" versKindChar "%d", versMajor, versMinor, versBuild)
	#endif
#endif

#define dayBuild			1			
#define dayBuildString		$$Format("Build %02d%02d%02d.%d", $$Year - 2000, $$Month, $$Day, dayBuild)

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
	"Electric Fish Color Pal"
};

