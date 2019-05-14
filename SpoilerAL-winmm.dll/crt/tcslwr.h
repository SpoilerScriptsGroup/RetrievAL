#include <windows.h>
#ifdef _MBCS
#define _tcslwr _mbslwr
#elif defined(_UNICODE)
#define _tcslwr _wcslwr
#else
#define _tcslwr _strlwr
#endif

#ifndef _M_IX86
#ifndef _MBCS
TCHAR * __cdecl _tcslwr(TCHAR *string)
{
	TCHAR *p, c;

	p = string;
	while (c = *(p++))
#ifdef _UNICODE
		if ((c -= 'A') <= 'Z' - 'A')
#else
		if ((unsigned char)(c -= 'A') <= 'Z' - 'A')
#endif
			p[-1] = c + 'a';
	return string;
}
#else
unsigned char * __cdecl _mbslwr(unsigned char *string)
{
	LCMapStringA(GetThreadLocale(), LCMAP_LOWERCASE, string, -1, string, INT_MAX);
	return string;
}
#endif
#else
#ifndef _MBCS
__declspec(naked) TCHAR * __cdecl _tcslwr(TCHAR *string)
{
#ifdef _UNICODE
	#define tchar_ptr    word ptr
	#define inc_tchar(r) add r, 2
	#define t(r)         r##x
	#define sizeof_tchar 2
#else
	#define tchar_ptr    byte ptr
	#define inc_tchar(r) inc r
	#define t(r)         r##l
	#define sizeof_tchar 1
#endif

	__asm
	{
		#define string (esp + 4)
		#define p      ecx
		#define c      t(a)

		mov     p, dword ptr [string]
		jmp     L2

		align   16
	L1:
		sub     c, 'A'
		cmp     c, 'Z' - 'A'
		ja      L2
		add     c, 'a'
		mov     tchar_ptr [p - sizeof_tchar], c
	L2:
		mov     c, tchar_ptr [p]
		inc_tchar(p)
		test    c, c
		jnz     L1
		mov     eax, dword ptr [string]
		ret

		#undef string
		#undef p
		#undef c
	}

	#undef tchar_ptr
	#undef inc_tchar
	#undef t
	#undef sizeof_tchar
}
#else
__declspec(naked) unsigned char * __cdecl _mbslwr(unsigned char *string)
{
	__asm
	{
		#define string (esp + 4)

		call    GetThreadLocale
		mov     ecx, dword ptr [string]
		push    07FFFFFFFH
		push    ecx
		push    -1
		push    ecx
		push    LCMAP_LOWERCASE
		push    eax
		call    LCMapStringA
		mov     eax, dword ptr [string]
		ret

		#undef string
	}
}
#endif
#endif
