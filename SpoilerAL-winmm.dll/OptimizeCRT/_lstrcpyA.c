#include <string.h>

#pragma function(strcpy)

__declspec(naked) char * __stdcall _lstrcpyA(char *lpString1, const char *lpString2)
{
	__asm
	{
		mov     edx, dword ptr [esp + 8]
		mov     ecx, dword ptr [esp + 4]
		push    edx
		push    ecx
		call    strcpy
		add     esp, 8
		ret     8
	}
}
