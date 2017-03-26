#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"

EXTERN_C BOOL __fastcall TSSGCtrl_ReadSSRFile_CheckSignedParam(bcb6_std_vector *tmpV)
{
	if (bcb6_std_vector_size(tmpV, bcb6_std_string) >= 5)
	{
		char *p = ((bcb6_std_string *)tmpV->_M_start + 4)->_M_start;
		while (__intrinsic_isspace(*p))
			p++;
		size_t length = ((bcb6_std_string *)tmpV->_M_start + 4)->_M_finish - p;
		if (length >= 6)
		{
			while (__intrinsic_isspace(*(p + length - 1)) && --length >= 6);
			if (length == 6 && *(LPDWORD)p == BSWAP32('sign') && *(LPWORD)(p + 4) == BSWAP16('ed'))
				return TRUE;
		}
	}
	return FALSE;
}
