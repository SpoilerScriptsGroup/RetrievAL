#include <windows.h>
#include <assert.h>

#if (!defined(_MSC_VER) || _MSC_VER < 1200) && !defined(__assume)
#define __assume(expression)
#endif

#ifndef _M_IX86
int __cdecl _mbsicmp(const unsigned char *string1, const unsigned char *string2)
{
	int ret;

	assert(CSTR_LESS_THAN    == 1);
	assert(CSTR_EQUAL        == 2);
	assert(CSTR_GREATER_THAN == 3);
	assert(_NLSCMPERROR      == 0x7FFFFFFF);

	ret = CompareStringA(GetThreadLocale(), NORM_IGNORECASE, string1, -1, string2, -1);
	__assume(ret >= 0 && ret <= 3);
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
		add     eax, -1
		sbb     ecx, ecx
		and     eax, 7FFFFFFFH
		add     eax, ecx
		ret

		#undef string1
		#undef string2
	}
}
#endif
