#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"
#include "bcb6_std_vector.h"
#include "TProcessAccessElementBase.h"
#include "TProcessAccessElementData.h"

#define AT_DATA 4

EXTERN_C void __stdcall TSSString_Write_UnicodeString(bcb6_std_string *endWord, bcb6_std_vector *CompareVec, bcb6_std_string *Str)
{
	if (bcb6_std_string_length(endWord) == 7 &&
		*(LPDWORD) endWord->_M_start      == BSWAP32('unic') &&
		*(LPDWORD)(endWord->_M_start + 4) == BSWAP32('ode\0'))
	{
		for (LPVOID *it = (LPVOID *)CompareVec->_M_start; it != CompareVec->_M_finish; it++)
		{
			if (TProcessAccessElementBase_GetType(*it) != AT_DATA)
				continue;
			bcb6_std_vector *data = TProcessAccessElementData_GetData(*it);
			size_t size = strlen((LPCSTR)data->_M_start);
			bcb6_std_string_assign_cstr_with_length(Str, (LPCSTR)data->_M_start, size);
			bcb6_std_vector_resize(data, ++size * 2);
			size = MultiByteToWideChar(
				CP_ACP,
				0,
				Str->_M_start,
				size,
				(LPWSTR)data->_M_start,
				size);
			data->_M_finish = (LPWSTR)data->_M_start + size;
		}
	}
}
