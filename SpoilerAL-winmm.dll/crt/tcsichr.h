#include <windows.h>

#ifdef _MBCS
#define _tcsichr _mbsichr
#define _tcschr _mbschr
extern unsigned char * __cdecl _mbschr(const unsigned char *string, unsigned int c);
#elif defined(_UNICODE)
#define _tcsichr _wcsichr
#define _tcschr wcschr
extern wchar_t * __cdecl wcschr(const wchar_t *string, wint_t c);
#else
#define _tcsichr _strichr
#define _tcschr strchr
extern char * __cdecl strchr(const char *string, int c);
#endif

#pragma warning(disable:4102)

#ifndef _M_IX86
#ifdef _MBCS
unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c)
#elif defined(_UNICODE)
wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c)
#else
char * __cdecl _strichr(const char *string, int c)
#endif
{
	TCHAR c2, c3, c4, *p;

#ifdef _MBCS
	if (c > USHRT_MAX)
		goto RETURN_NULL;
	if (c > UCHAR_MAX)
		goto TCSCHR;
#endif

#ifdef _UNICODE
	if ((short)c < L'A')
#else
	if ((char)c < 'A')
#endif
		goto TCSCHR;
#ifdef _UNICODE
	if (c <= L'Z')
#else
	if ((unsigned char)c <= 'Z')
#endif
	{
		c2 = (TCHAR)c;
		c3 = (TCHAR)c + ('a' - 'A');
	}
	else
	{
#ifdef _UNICODE
		if (c < L'a' || c > L'z')
#else
		if ((unsigned char)c < 'a' || (unsigned char)c > 'z')
#endif
			goto TCSCHR;
		c2 = (TCHAR)c;
		c3 = (TCHAR)c - ('a' - 'A');
	}

	p = (TCHAR *)string;
	do
		if ((c4 = *(p++)) == c2 || c4 == c3)
			return p - 1;
#ifdef _MBCS
	while (c4 && (!IsDBCSLeadByteEx(CP_THREAD_ACP, c4) || *(p++)));
#else
	while (c4);
#endif

RETURN_NULL:
	return NULL;

TCSCHR:
	return _tcschr(string, c);
}
#else
#ifdef _MBCS
__declspec(naked) unsigned char * __cdecl _mbsichr(const unsigned char *string, unsigned int c)
#elif defined(_UNICODE)
__declspec(naked) wchar_t * __cdecl _wcsichr(const wchar_t *string, wint_t c)
#else
__declspec(naked) char * __cdecl _strichr(const char *string, int c)
#endif
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
		#define string (esp + 4)
		#define c      (esp + 8)
#ifdef _MBCS
		#define c2     bl
		#define c3     bh
		#define c4     al
		#define p      esi
#elif defined(_UNICODE)
		#define c2     ax
		#define c3     bx
		#define c4     dx
		#define p      ecx
#else
		#define c2     al
		#define c3     ah
		#define c4     dl
		#define p      ecx
#endif

#ifdef _MBCS
		mov     eax, dword ptr [c]
		test    eax, 0FFFFFF00H
		jnz     L5
#else
		mov     t(a), tchar_ptr [c]
#endif
		sub     t(a), 'a'
		jb      L1
		cmp     t(a), 'z' - 'a'
		jbe     L2
		jmp     L6
	L1:
		add     t(a), 'a' - 'A'
		cmp     t(a), 'Z' - 'A'
		ja      L6
	L2:
#ifdef _MBCS
		push    ebx
		push    esi
		mov     ah, al
		mov     esi, dword ptr [string + 8]
		lea     ebx, [eax + 'Aa']
#elif defined(_UNICODE)
		push    ebx
		mov     bx, ax
		mov     ecx, dword ptr [string + 4]
		add     ax, 'a'
		add     bx, 'A'
#else
		mov     ah, al
		mov     ecx, dword ptr [string]
		add     ax, 'Aa'
#endif

		align   16
	L3:
		mov     c4, tchar_ptr [p]
		inc_tchar(p)
		cmp     c4, c2
		je      L7
		cmp     c4, c3
		je      L7
#ifdef _MBCS
		and     eax, 0FFH
		jz      L4
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L3
		mov     c4, tchar_ptr [p]
		inc_tchar(p)
		test    c4, c4
		jnz     L3
	L4:
		pop     esi
		pop     ebx
#elif defined(_UNICODE)
		test    c4, c4
		jnz     L3
		pop     ebx
#else
		test    c4, c4
		jnz     L3
#endif
		xor     eax, eax
		ret

#ifdef _MBCS
		align   16
	L5:
		test    eax, 0FFFF0000H
		jz      L6
		xor     eax, eax
		ret
#endif

		align   16
	L6:
		jmp     _tcschr

		align   16
	L7:
#ifdef _MBCS
		lea     eax, [p - 1]
		pop     esi
		pop     ebx
#elif defined(_UNICODE)
		pop     ebx
		lea     eax, [p - 2]
#else
		lea     eax, [p - 1]
#endif
		ret

		#undef string
		#undef c
		#undef c2
		#undef c3
		#undef c4
		#undef p
	}

	#undef tchar_ptr
	#undef inc_tchar
	#undef t
}
#endif
