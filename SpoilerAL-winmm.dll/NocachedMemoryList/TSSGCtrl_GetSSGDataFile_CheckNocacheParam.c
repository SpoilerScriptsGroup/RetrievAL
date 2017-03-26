#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"

EXTERN_C BOOL __fastcall TSSGCtrl_GetSSGDataFile_CheckNocacheParam(bcb6_std_vector *tmpV)
{
	if (bcb6_std_vector_size(tmpV, bcb6_std_string) >= 8)
	{
		char *p = ((bcb6_std_string *)tmpV->_M_start + 7)->_M_start;
		while (__intrinsic_isspace(*p))
			p++;
		size_t length = ((bcb6_std_string *)tmpV->_M_start + 7)->_M_finish - p;
		if (length >= 7)
		{
			while (length >= 7 && __intrinsic_isspace(*(p + length - 1)))
				length--;
			if (length == 7 && *(LPDWORD)p == BSWAP32('noca') && *(LPWORD)(p + 4) == BSWAP16('ch') && *(p + 6) == 'e')
				return TRUE;
		}
	}
	return FALSE;
}
