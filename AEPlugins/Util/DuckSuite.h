#include "A.h"
#include <SPTypes.h>

#if MSWindows
#include <windows.h>
#endif

#define kDuckSuite1				"AEGP Duck Suite"
#define kDuckSuiteVersion1		1

typedef struct DuckSuite1 {

	SPAPI A_Err	(*Quack)( int				times);					


} DuckSuite1;