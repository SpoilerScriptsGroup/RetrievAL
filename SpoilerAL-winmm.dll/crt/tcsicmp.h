#include <windows.h>

#ifdef _MBCS
#define _tcsicmp _mbsicmp
#elif defined(_UNICODE)
#define _tcsicmp _wcsicmp
#else
#define _tcsicmp _stricmp
#endif

#pragma warning(disable:4028)

#ifndef _M_IX86
#ifndef _MBCS
int __cdecl _tcsicmp(const TCHAR *string1, const TCHAR *string2)
{
	for (; ; )
	{
#ifdef _UNICODE
		wchar_t c1, c2;
#else
		unsigned char c1, c2;
#endif

		c1 = *(string1++);
		c2 = *(string2++);
		if (!(c1 -= c2))
			if (c2)
				continue;
			else
				break;
		else
		{
			if (c1 == 'A' - 'a')
			{
				if ((c2 -= 'a') <= 'z' - 'a')
					continue;
				c2 += 'a';
			}
			else if (c1 == 'a' - 'A')
			{
				if ((c2 -= 'A') <= 'Z' - 'A')
					continue;
				c2 += 'A';
			}
			c1 += c2;
		}
		return (int)c1 - (int)c2;
	}
	return 0;
}
#else
int __cdecl _mbsicmp(const unsigned char *string1, const unsigned char *string2)
{
	int ret;

	ret = CompareStringA(GetThreadLocale(), NORM_IGNORECASE, string1, -1, string2, -1);
	return ret ? ret - CSTR_EQUAL : _NLSCMPERROR;
}
#endif
#else
#ifndef _MBCS
__declspec(naked) int __cdecl _tcsicmp(const TCHAR *string1, const TCHAR *string2)
{
#ifdef _UNICODE
	#define tchar_ptr    word ptr
	#define inc_tchar(r) add r, 2
	#define t(r)         r##x
#else
	#define tchar_ptr    byte ptr
	#define inc_tchar(r) inc r
	#define t(r)         r##l
#endif

	__asm
	{
		#define string1 (esp + 4)
		#define string2 (esp + 8)

		push    ebx
		push    esi
		mov     ebx, dword ptr [string1 + 8]
		mov     esi, dword ptr [string2 + 8]
		sub     esi, ebx

		align   16
	L1:
		mov     t(c), tchar_ptr [ebx]
		xor     eax, eax
		mov     t(a), tchar_ptr [ebx + esi]
		inc_tchar(ebx)
		sub     t(c), t(a)
		jnz     L3
		test    t(a), t(a)
		jnz     L1
		pop     esi
		pop     ebx
		ret

		align   16
	L3:
		cmp     t(c), 'a' - 'A'
		jne     L4
		sub     t(a), 'A'
		cmp     t(a), 'Z' - 'A'
		jbe     L1
		add     t(a), 'A'
		jmp     L5

		align   16
	L4:
		cmp     t(c), 'A' - 'a'
		jne     L5
		sub     t(a), 'a'
		cmp     t(a), 'z' - 'a'
		jbe     L1
		add     t(a), 'a'
	L5:
		add     t(c), t(a)
		sbb     eax, eax
		pop     esi
		or      eax, 1
		pop     ebx
		ret

		#undef string1
		#undef string2
	}

	#undef tchar_ptr
	#undef inc_tchar
	#undef t
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
#endif
