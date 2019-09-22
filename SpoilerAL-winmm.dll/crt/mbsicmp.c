#include <windows.h>

#ifndef _M_IX86
int __cdecl _mbsicmp(const unsigned char *string1, const unsigned char *string2)
{
	int ret;

	ret = CompareStringA(GetThreadLocale(), NORM_IGNORECASE, string1, -1, string2, -1);
	return ret ? ret - CSTR_EQUAL : _NLSCMPERROR;
}
#else
__declspec(naked) int __cdecl _mbsicmp(const unsigned char *string1, const unsigned char *string2)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		call    GetThreadLocale
		mov     ecx, dword ptr [string1]
		mov     edx, dword ptr [string2]
		push    -1
		push    edx
		push    -1
		push    ecx
		push    NORM_IGNORECASE
		push    eax
		call    CompareStringA
		xor     ecx, ecx
		sub     eax, 1
		adc     ecx, -1
		and     eax, 7FFFFFFFH
		add     eax, ecx
		ret

		#undef string1
		#undef string2
	}
}
#endif
