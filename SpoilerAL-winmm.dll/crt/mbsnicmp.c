#include <windows.h>

#ifndef _M_IX86
int __cdecl _mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	const unsigned char *p, c;
	int                 ret;

	if (!count)
		return 0;
	p = string1;
	do
		if (!(c = *(p++)))
			break;
		else if (IsDBCSLeadByteEx(CP_THREAD_ACP, c) && !*(p++))
			break;
	while (--count);
	count = p - string1;
	ret = CompareStringA(GetThreadLocale(), NORM_IGNORECASE, string1, (int)count, string2, (int)count);
	return ret ? ret - CSTR_EQUAL : _NLSCMPERROR;
}
#else
__declspec(naked) int __cdecl _mbsnicmp(const unsigned char *string1, const unsigned char *string2, size_t count)
{
	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)
		#define count   (esp + 12)

		push    ebx
		push    esi
		mov     esi, dword ptr [count + 8]
		xor     eax, eax
		test    esi, esi
		jz      L4
		mov     ebx, dword ptr [string1 + 8]

		align   16
	L1:
		mov     al, byte ptr [ebx]
		inc     ebx
		test    al, al
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		mov     cl, byte ptr [ebx]
		inc     ebx
		test    cl, cl
		jz      L3
		xor     eax, eax
	L2:
		dec     esi
		jnz     L1
	L3:
		mov     eax, dword ptr [string1 + 8]
		mov     ecx, dword ptr [string2 + 8]
		pop     esi
		sub     ebx, eax
		push    ebx
		push    ecx
		push    ebx
		push    eax
		call    GetThreadLocale
		push    NORM_IGNORECASE
		push    eax
		call    CompareStringA
		xor     ecx, ecx
		sub     eax, 1
		adc     ecx, -1
		and     eax, 7FFFFFFFH
		add     eax, ecx
		pop     ebx
		ret

		align   16
	L4:
		pop     esi
		pop     ebx
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
