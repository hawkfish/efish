#ifndef MSWindows
#include "AE_General.r"
#endif

resource 'PiPL' (16000) {
	{	/* array properties: 7 elements */
		/* [1] */
		Kind {
			AEGP
		},
		/* [2] */
		Name {
			"Color Pal"
		},
		/* [3] */
		Category {
			"General Plugin"
		},
		/* [4] */
		Version {
			196608
		},
		/* [5] */
#ifdef MSWindows
		CodeWin32X86 {
			"GPMain_Color_Pal"
		},
#else	
		CodePowerPC {
					0,
					0,
					""
		},
#endif
	}
};
