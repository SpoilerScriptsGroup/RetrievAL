#include <windows.h>

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

#pragma warning(disable:4102)

#ifndef _M_IX86
#ifndef _MBCS
#ifdef _UNICODE
wchar_t * __cdecl _wcsrichr(const wchar_t *string, wint_t c)
#else
char * __cdecl _strrichr(const char *string, int c)
#endif
{
	TCHAR *find, *p;

	if (!(TCHAR)c)
		return _tcsichr(string, c);
	find = NULL;
	if (p = _tcsichr(string, c))
		do
			find = p;
		while (p = _tcsichr(p + 1, c));
	return find;
}
#else
unsigned char * __cdecl _mbsrichr(const unsigned char *string, unsigned int c)
{
	unsigned char *find;

	if (!c)
		return _mbsichr(string, c);
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
		if (!(p = _mbsichr(string, c)))
			break;
		do
			find = p;
		while (p = _mbsichr(p + n, c));
	} while (0);
	return find;
}
#endif
#else
#ifndef _MBCS
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

		mov     eax, dword ptr [string]
		mov     ecx, dword ptr [c]
		test    t(c), t(c)
		jz      _tcsichr
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
		call    _tcsichr
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
__declspec(naked) unsigned char * __cdecl _mbsrichr(const unsigned char *string, unsigned int c)
{
	__asm
	{
		#define string (esp + 4)
		#define c      (esp + 8)

		mov     ecx, dword ptr [string]
		mov     eax, dword ptr [c]
		test    eax, eax
		jz      _mbsichr
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
		call    _mbsichr
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
