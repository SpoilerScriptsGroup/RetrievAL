#include <string.h>

#pragma function(strcmp)

__declspec(naked) int __stdcall _lstrcmpA(const char *lpString1, const char *lpString2)
{
	__asm
	{
		mov     edx, dword ptr [esp + 8]
		mov     ecx, dword ptr [esp + 4]
		push    edx
		push    ecx
		call    strcmp
		add     esp, 8
		ret     8
	}
}
