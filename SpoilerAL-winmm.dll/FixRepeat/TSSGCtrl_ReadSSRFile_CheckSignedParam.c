#include <windows.h>
#include "intrinsic.h"
#define USING_NAMESPACE_BCB6_STD
#include "bcb6_std_vector_string.h"

EXTERN_C BOOL __fastcall TSSGCtrl_ReadSSRFile_CheckSignedParam(vector_string *tmpV)
{
	if (vector_size(tmpV) >= 5)
	{
		char *p = ((string *)tmpV->_M_start + 4)->_M_start;
		while (__intrinsic_isspace(*p))
			p++;
		size_t length = ((string *)tmpV->_M_start + 4)->_M_finish - p;
		if (length >= 6)
		{
			while (__intrinsic_isspace(*(p + length - 1)) && --length >= 6);
			if (length == 6 && *(LPDWORD)p == BSWAP32('sign') && *(LPWORD)(p + 4) == BSWAP16('ed'))
				return TRUE;
		}
	}
	return FALSE;
}
