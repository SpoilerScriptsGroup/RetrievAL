#include <windows.h>
#include <tchar.h>

#pragma warning(disable:4414)

#ifdef _MBCS
#define _tcsichr _mbsichr
extern unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c);
#elif defined(_UNICODE)
#define _tcsichr _wcsichr
extern wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c);
#else
#define _tcsichr _strichr
extern char * __cdecl _strichr(const char *string, int c);
#endif

#ifndef _M_IX86
#ifndef _MBCS
#ifdef _UNICODE
wchar_t * __cdecl _wcsrichr(const wchar_t *string, wint_t c)
#else
char * __cdecl _strrichr(const char *string, int c)
#endif
{
	const TCHAR *p;

	if (!(TCHAR)c)
		return (TCHAR *)string + _tcslen(string);
	p = NULL;
	for (; string = _tcsichr(string, c); string++)
		p = string;
	return (TCHAR *)p;
}
#else
unsigned char * __cdecl _mbsrichr(const unsigned char *string, unsigned int c)
{
	const unsigned char *p;
	size_t              n;

	if (!c)
		return _mbschr(string, c);
	p = NULL;
	if (!(c & ~0xFFFF))
		for (n = c <= 0xFF ? 1 : 2; string = _mbsichr(string, c); string += n)
			p = string;
	return (unsigned char *)p;
}
#endif
#else
#ifndef _MBCS
#pragma function(_tcslen)
#ifdef _UNICODE
__declspec(naked) wchar_t * __cdecl _wcsrichr(const wchar_t *string, wint_t c)
#else
__declspec(naked) char * __cdecl _strrichr(const char *string, int c)
#endif
{
#ifdef _UNICODE
	#define t(r)         r##x
	#define inc_tchar(r) add r, 2
#else
	#define t(r)         r##l
	#define inc_tchar(r) inc r
#endif

	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		push    esi
		mov     ebx, dword ptr [c + 8]
		mov     esi, dword ptr [string + 8]
		push    ebx
		push    esi
		test    t(b), t(b)
		jz      L1
		xor     esi, esi
		jmp     L3

		align   16
	L1:
		call    _tcslen
		add     esp, 8
#ifdef _UNICODE
		lea     eax, [esi + eax * 2]
#else
		add     eax, esi
#endif
		pop     esi
		pop     ebx
		ret

		align   16
	L2:
		mov     esi, eax
		inc_tchar(eax)
		mov     dword ptr [esp    ], eax
		mov     dword ptr [esp + 4], ebx
	L3:
		call    _tcsichr
		test    eax, eax
		jnz     L2
		add     esp, 8
		mov     eax, esi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}

	#undef t
	#undef inc_tchar
}
#else
__declspec(naked) unsigned char * __cdecl _mbsrichr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		push    ebx
		push    esi
		mov     ebx, dword ptr [c + 8]
		mov     esi, dword ptr [string + 8]
		test    ebx, ebx
		jz      L1
		push    edi
		xor     edi, edi
		test    ebx, not 0FFFFH
		jnz     L4
		push    ebx
		push    esi
		mov     esi, 2
		cmp     ebx, 100H
		sbb     esi, 0
		jmp     L3

		align   16
	L1:
		pop     esi
		pop     ebx
		jmp     _mbschr

		align   16
	L2:
		mov     edi, eax
		add     eax, esi
		mov     dword ptr [esp    ], eax
		mov     dword ptr [esp + 4], ebx
	L3:
		call    _mbsichr
		test    eax, eax
		jnz     L2
		add     esp, 8

		align   16
	L4:
		mov     eax, edi
		pop     edi
		pop     esi
		pop     ebx
		ret

		#undef string
		#undef c
	}
}
#endif
#endif
