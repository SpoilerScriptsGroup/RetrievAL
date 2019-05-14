#include <windows.h>

#pragma warning(disable:4414)

#ifdef _MBCS
#define _tcschr _mbschr
extern unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c);
#elif defined(_UNICODE)
#define _tcschr wcschr
extern wchar_t * __cdecl wcschr(const wchar_t *string, wint_t c);
#else
#define _tcschr strchr
extern char * __cdecl strchr(const char *string, int c);
#endif

#pragma warning(disable:4102)

#ifndef _M_IX86
#ifndef _MBCS
#ifdef _UNICODE
wchar_t * __cdecl wcsrchr(const wchar_t *string, wint_t c)
#else
char * __cdecl strrchr(const char *string, int c)
#endif
{
	TCHAR *find, *p;

	if (!(TCHAR)c)
		return _tcschr(string, c);
	find = NULL;
	if (p = _tcschr(string, c))
		do
			find = p;
		while (p = _tcschr(p + 1, c));
	return find;
}
#else
unsigned char * __cdecl _mbsrchr(const unsigned char *string, unsigned int c)
{
	unsigned char *find;

	if (!c)
		return _mbschr(string, c);
	find = NULL;
	do	// do { ... } while (0);
	{
		size_t        n;
		unsigned char *p;

		if (!(c & ~0xFF))
			n = 1;
		else if (!(c & ~0xFFFF))
			n = 2;
		else
			break;
		if (!(p = _mbschr(string, c)))
			break;
		do
			find = p;
		while (p = _mbschr(p + n, c));
	} while (0);
	return find;
}
#endif
#else
#ifndef _MBCS
#ifdef _UNICODE
__declspec(naked) wchar_t * __cdecl wcsrchr(const wchar_t *string, wint_t c)
#else
__declspec(naked) char * __cdecl strrchr(const char *string, int c)
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

		mov     eax, dword ptr [string]
		mov     ecx, dword ptr [c]
		test    t(c), t(c)
		jz      _tcschr
		push    ebx
		push    esi
		push    ecx
		push    eax
		mov     ebx, ecx
		xor     esi, esi
		jmp     L2

		align   16
	L1:
		mov     esi, eax
		inc_tchar(eax)
		mov     dword ptr [esp    ], eax
		mov     dword ptr [esp + 4], ebx
	L2:
		call    _tcschr
		test    eax, eax
		jnz     L1
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
__declspec(naked) unsigned char * __cdecl _mbsrchr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ecx, dword ptr [string]
		mov     eax, dword ptr [c]
		test    eax, eax
		jz      _mbschr
		push    ebx
		push    esi
		push    edi
		push    eax
		push    ecx
		mov     ebx, eax
		xor     esi, esi
		mov     edi, 1
		test    eax, not 0FFH
		jz      L2
		test    eax, not 0FFFFH
		jnz     L3
		inc     edi
		jmp     L2

		align   16
	L1:
		mov     esi, eax
		add     eax, edi
		mov     dword ptr [esp    ], eax
		mov     dword ptr [esp + 4], ebx
	L2:
		call    _mbschr
		test    eax, eax
		jnz     L1

		align   16
	L3:
		add     esp, 8
		mov     eax, esi
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
