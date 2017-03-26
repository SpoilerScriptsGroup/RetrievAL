#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"

EXTERN_C void __stdcall TSSString_Read_UnicodeString(unsigned long size, bcb6_std_string *endWord, bcb6_std_string *Data, char *tmpC)
{
	if (bcb6_std_string_length(endWord) == 7 &&
		*(LPDWORD) endWord->_M_start      == BSWAP32('unic') ||
		*(LPDWORD)(endWord->_M_start + 4) == BSWAP32('ode\0'))
	{
		bcb6_std_string_reserve(Data, size);
		int cchMultiByte =
			WideCharToMultiByte(
				CP_ACP,
				0,
				(LPCWSTR)tmpC,
				-1,
				Data->_M_start,
				size + 1,
				NULL,
				NULL);
		if (cchMultiByte != 0)
			cchMultiByte--;
		*(Data->_M_finish = Data->_M_start + cchMultiByte) = '\0';
	}
}
