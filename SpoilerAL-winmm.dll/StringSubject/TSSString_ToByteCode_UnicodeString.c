#include <windows.h>
#include "intrinsic.h"
#include "bcb6_std_string.h"

EXTERN_C void __stdcall TSSString_ToByteCode_UnicodeString(bcb6_std_string *endWord, bcb6_std_string *Val, bcb6_std_string *tmpS)
{
	if (bcb6_std_string_length(endWord) == 7 &&
		*(LPDWORD) endWord->_M_start      == BSWAP32('unic') ||
		*(LPDWORD)(endWord->_M_start + 4) == BSWAP32('ode\0'))
	{
		bcb6_std_string_assign(tmpS, Val);
		size_t length = bcb6_std_string_length(Val);
		bcb6_std_string_reserve(Val, length * 2);
		length = MultiByteToWideChar(
			CP_ACP,
			0,
			tmpS->_M_start,
			length,
			(LPWSTR)Val->_M_start,
			length);
		*(Val->_M_finish = Val->_M_start + length * 2) = '\0';
		bcb6_std_string_clear(tmpS);
	}
}
