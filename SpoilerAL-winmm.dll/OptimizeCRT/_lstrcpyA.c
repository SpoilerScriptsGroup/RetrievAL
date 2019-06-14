#include <string.h>

#pragma function(strcpy)

__declspec(naked) char * __stdcall _lstrcpyA(char *lpString1, const char *lpString2)
{
	__asm
	{
		#define lpString1 (esp + 4)
		#define lpString2 (esp + 8)

		mov     edx, dword ptr [lpString2]
		mov     ecx, dword ptr [lpString1]
		push    edx
		push    ecx
		call    strcpy
		add     esp, 8
		ret     8

		#undef lpString1
		#undef lpString2
	}
}
