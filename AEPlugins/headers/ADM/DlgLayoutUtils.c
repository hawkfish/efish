#include <stdlib.h>
#include "DlgLayoutUtils.h"

//parsing out the layout constant
//if it fails, it will return 0
//and the caller will have a chance to put in 
//a default value.
long	ParseLayoutStr(const char *strZ)
{
	long	numL = 0, str_lenL = 0;
	char	num_strAC[256];
	
	if (strZ)	{
		while (*strZ != LAYOUT_TAG && *strZ)	{
			strZ++;
		}
		if (*strZ == LAYOUT_TAG)	{
			strZ++;
			while (*strZ)	{
				num_strAC[str_lenL++] = *strZ++;
			}
		}
		num_strAC[str_lenL] = 0;
		if (str_lenL)	{
			numL = atol(num_strAC);
		}
	}
	
	return numL;
}