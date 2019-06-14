#include <string.h>

#pragma function(strlen)

__declspec(naked) int __stdcall _lstrlenA(const char *lpString)
{
	__asm
	{
		#define lpString (esp + 4)

		pop     eax
		pop     ecx
		push    eax
		push    ecx
		call    strlen
		pop     ecx
		ret

		#undef lpString
	}
}
