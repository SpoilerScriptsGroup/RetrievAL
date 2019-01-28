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

#pragma warning(disable:4102 4414)

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
#elif !defined(_UNICODE)
	if ((unsigned int)c > UCHAR_MAX)
		goto RETURN_NULL;
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
		cmp     eax, 0FFFFH
		ja      L4
		cmp     ah, ah
		jnz     _tcschr
#elif defined(_UNICODE)
		mov     ax, word ptr [c]
#else
		mov     eax, dword ptr [c]
		cmp     eax, 0FFH
		ja      L4
#endif
		cmp     t(a), 'A'
		jl      _tcschr
		cmp     t(a), 'Z'
		ja      L1
#ifdef _MBCS
		push    ebx
		push    esi
		mov     ebx, eax
		mov     p, dword ptr [string + 8]
#elif defined(_UNICODE)
		mov     p, dword ptr [string]
		push    ebx
#else
		mov     p, dword ptr [string]
#endif
		mov     c3, c2
		add     c3, 'a' - 'A'
		jmp     L2
	L1:
		cmp     t(a), 'a'
		jb      _tcschr
		cmp     t(a), 'z'
		ja      _tcschr
#ifdef _MBCS
		push    ebx
		push    esi
		mov     ebx, eax
		mov     p, dword ptr [string + 8]
#elif defined(_UNICODE)
		mov     p, dword ptr [string]
		push    ebx
#else
		mov     p, dword ptr [string]
#endif
		mov     c3, c2
		sub     c3, 'a' - 'A'

		align   16
	L2:
		mov     c4, tchar_ptr [p]
		inc_tchar(p)
		cmp     c4, c2
		je      L5
		cmp     c4, c3
		je      L5
#ifdef _MBCS
		and     eax, 0FFH
		jz      L3
		push    eax
		push    CP_THREAD_ACP
		call    IsDBCSLeadByteEx
		test    eax, eax
		jz      L2
		mov     c4, tchar_ptr [p]
		inc_tchar(p)
		test    c4, c4
		jnz     L2
	L3:
		pop     esi
		pop     ebx
	L4:
#elif defined(_UNICODE)
		test    c4, c4
		jnz     L2
		pop     ebx
#else
		test    c4, c4
		jnz     L2
	L4:
#endif
		xor     eax, eax
		ret

		align   16
	L5:
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
