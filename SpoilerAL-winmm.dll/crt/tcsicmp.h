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
		if (c1 == 'A' - 'a')
		{
			if ((c2 - 'a') <= 'z' - 'a')
				continue;
		}
		else if (c1 == 'a' - 'A')
		{
			if ((c2 - 'A') <= 'Z' - 'A')
				continue;
		}
		c1 += c2;
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
		xor     eax, eax
		mov     ecx, dword ptr [string1 + 4]
		mov     ebx, dword ptr [string2 + 4]
		sub     ebx, ecx

		align   16
	L1:
		xor     edx, edx
		mov     t(a), tchar_ptr [ecx]
		mov     t(d), tchar_ptr [ecx + ebx]
		inc_tchar(ecx)
		sub     eax, edx
		jnz     L2
		test    edx, edx
		jnz     L1
		pop     ebx
		ret

		align   16
	L2:
		cmp     eax, 'a' - 'A'
		jne     L3
		sub     edx, 'A'
		xor     eax, eax
		cmp     edx, 'Z' - 'A'
		jbe     L1
		add     eax, 'a' - 'A'
		pop     ebx
		ret

		align   16
	L3:
		cmp     eax, 'A' - 'a'
		jne     L4
		sub     edx, 'a'
		xor     eax, eax
		cmp     edx, 'z' - 'a'
		jbe     L1
		sub     eax, 'a' - 'A'
	L4:
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
