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

		push    esi
		push    edi
		mov     edi, dword ptr [count + 8]
		xor     eax, eax
		test    edi, edi
		jz      L4
		mov     esi, dword ptr [string1 + 8]

		align   16
	L1:
		mov     al, byte ptr [esi]
		inc     esi
		test    al, al
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		mov     cl, byte ptr [esi]
		inc     esi
		test    cl, cl
		jz      L3
		xor     eax, eax
	L2:
		dec     edi
		jnz     L1
	L3:
		mov     eax, dword ptr [string1 + 8]
		mov     ecx, dword ptr [string2 + 8]
		pop     edi
		sub     esi, eax
		push    esi
		push    ecx
		push    esi
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
		pop     esi
		ret

		align   16
	L4:
		pop     edi
		pop     esi
		ret

		#undef string1
		#undef string2
		#undef count
	}
}
#endif
