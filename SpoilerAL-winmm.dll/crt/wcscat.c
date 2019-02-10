#include <string.h>

#ifndef _M_IX86
wchar_t * __cdecl wcscat(wchar_t *string1, const wchar_t *string2)
{
	wcscpy(string1 + wcslen(string1), string2);
	return string1;
}
#else
__declspec(naked) wchar_t * __cdecl wcscat(wchar_t *string1, const wchar_t *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		mov     eax, dword ptr [string1]
		push    ebx
		mov     ebx, eax
		push    eax
		call    wcslen
		mov     ecx, dword ptr [string2 + 8]
		lea     eax, [ebx + eax * 2]
		mov     dword ptr [esp], ecx
		push    eax
		call    wcscpy
		add     esp, 8
		mov     eax, ebx
		pop     ebx
		ret

		#undef string1
		#undef string2
	}
}
#endif
